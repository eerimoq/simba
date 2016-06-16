/**
 * @file http_server.c
 * @version 0.7.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

static const FAR char ok_fmt[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %d\r\n"
    "\r\n";

static const FAR char not_found_fmt[] =
    "HTTP/1.1 404 Not Found\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %d\r\n"
    "\r\n";

static int read_initial_request_line(struct socket_t *socket_p,
                                     char *buf_p,
                                     struct http_server_request_t *request_p)
{
    char *action_p = NULL;
    char *path_p = NULL;
    char *proto_p = NULL;

    *buf_p++ = '\0';
    action_p = buf_p;

    while (1) {
        if (socket_read(socket_p,
                        buf_p,
                        sizeof(*buf_p)) != sizeof(*buf_p)) {
            return (-EIO);
        }

        buf_p++;

        /* The line ending is "\r\n". */
        if ((buf_p[-2] == '\r') && (buf_p[-1] == '\n')) {
            buf_p[-2] = '\0';
            break;
        }

        /* Action and path has ' ' as terminator. */
        if (buf_p[-1] == ' ') {
            buf_p[-1] = '\0';

            if (path_p == NULL) {
                path_p = buf_p;
            } else if (proto_p == NULL) {
                proto_p = buf_p;
            }
        }
    }

    /* A path is required. */
    if (path_p == NULL) {
        return (-1);
    }

    log_object_print(NULL,
                     LOG_DEBUG,
                     FSTR("%s %s %s\r\n"), action_p, path_p, proto_p);

    /* Save the action and path in the request struct. */
    strcpy(request_p->path, path_p);

    if (strcmp(action_p, "GET") == 0) {
        request_p->action = http_server_request_action_get_t;
    } else {
        return (-1);
    }

    return (0);
}

static int read_header_line(struct socket_t *socket_p,
                            char *buf_p,
                            char **header_pp,
                            char **value_pp)
{
    *buf_p++ = '\0';
    *header_pp = buf_p;
    *value_pp = NULL;

    while (1) {
        if (socket_read(socket_p,
                        buf_p,
                        sizeof(*buf_p)) != sizeof(*buf_p)) {
            return (-EIO);
        }

        buf_p++;

        /* The line ending is "\r\n". */
        if ((buf_p[-2] == '\r') && (buf_p[-1] == '\n')) {
            buf_p[-2] = '\0';
            break;
        }

        /* Value starts after ': '. */
        if ((buf_p[-2] == ':') && (buf_p[-1] == ' ')) {
            if (*value_pp == NULL) {
                buf_p[-2] = '\0';
                *value_pp = buf_p;
            }
        }
    }

    if (*value_pp != NULL) {
        return (0);
    } else {
        /* Empty line. */
        return (1);
    }
}

static int read_request(struct http_server_t *self_p,
                        struct http_server_connection_t *connection_p,
                        struct http_server_request_t *request_p)
{
    int res;
    char buf[128];
    char *header_p;
    char *value_p;

    /* Read the intial line in the request. */
    if (read_initial_request_line(&connection_p->socket,
                                  buf,
                                  request_p) != 0) {
        return (-EIO);
    }

    memset(&request_p->headers, 0, sizeof(request_p->headers));

    /* Read the header lines. */
    while (1) {
        res = read_header_line(&connection_p->socket,
                               buf,
                               &header_p,
                               &value_p);

        if (res == 1) {
            break;
        } else if (res < 0) {
            return (-EIO);
        }

        log_object_print(NULL, LOG_DEBUG, FSTR("%s: %s\r\n"), header_p, value_p);

        /* Save the header field in the request object. */
        if (strcmp(header_p, "Sec-WebSocket-Key") == 0) {
            request_p->headers.sec_websocket_key.present = 1;
            strcpy(request_p->headers.sec_websocket_key.value, value_p);
        }
    }

    return (0);
}

/**
 * Search for given path in the routes array and return its callback.
 */
static http_server_route_callback_t
find_route_callback(struct http_server_t *self_p,
                    const char *path_p)
{
    const struct http_server_route_t *route_p;

    route_p = self_p->routes_p;

    while (route_p->path_p != NULL) {
        if (strcmp(route_p->path_p, path_p) == 0) {
            return (route_p->callback);
        }

        route_p++;
    }

    return (NULL);
}

static int handle_request(struct http_server_t *self_p,
                          struct http_server_connection_t *connection_p)
{
    int res;
    struct http_server_request_t request;
    http_server_route_callback_t callback;

    /* Read the HTTP request. */
    res = read_request(self_p, connection_p, &request);

    if (res != 0) {
        return (res);
    }
    
    /* Find the callback for given path. */
    callback = find_route_callback(self_p, request.path);

    if (callback == NULL) {
        callback = self_p->on_no_route;
    }

    /* Call the callback and write the response if requested. */
    return (callback(connection_p, &request));
}

/**
 * The connection thread serves a client for the duration of the
 * socket lifetime.
 */
static void *connection_main(void *arg_p)
{
    struct http_server_connection_t *connection_p = arg_p;
    struct http_server_t *self_p = connection_p->self_p;
    uint32_t mask;

    /* thrd_env_init(buf, sizeof(buf)); */
    /* thrd_env_set("CWD", self_p->root_path_p); */
    thrd_set_name(connection_p->thrd.name_p);

    /* Wait for a connection from the listener. */
    while (1) {
        log_object_print(NULL,
                         LOG_DEBUG,
                         FSTR("Connection thread '%s' waiting for a new connection.\r\n"),
                         thrd_get_name());

        mask = 0x1;
        event_read(&connection_p->events, &mask, sizeof(mask));

        if (mask & 0x1) {
            handle_request(self_p, connection_p);
            socket_close(&connection_p->socket);

            /* Add thread to the free list. */
            sys_lock();
            connection_p->state = http_server_connection_state_free_t;
            sys_unlock();
            mask = 0x1;
            event_write(&self_p->events, &mask, sizeof(mask));
        }
    }

    return (NULL);
}

static int handle_accept(struct http_server_t *self_p,
                         struct http_server_connection_t *connection_p)
{
    uint32_t mask;

    mask = 0x1;
    event_write(&connection_p->events, &mask, sizeof(mask));

    return (0);
}

static struct http_server_connection_t *
allocate_connection(struct http_server_t *self_p)
{
    uint32_t mask;
    struct http_server_connection_t *connection_p;

    while (1) {
        sys_lock();

        connection_p = self_p->connections_p;

        while (connection_p->thrd.stack.buf_p != NULL) {
            if (connection_p->state == http_server_connection_state_free_t) {
                connection_p->state = http_server_connection_state_allocated_t;
                break;
            }

            connection_p++;
        }

        sys_unlock();

        /* Connection available. */
        if (connection_p->thrd.name_p != NULL) {
            break;
        }

        mask = 0x1;
        event_read(&self_p->events, &mask, sizeof(mask));
    }

    return (connection_p);
}

/**
 * The listener thread main function. The listener listens for
 * connections from clients.
 */
static void *listener_main(void *arg_p)
{
    struct http_server_t *self_p = arg_p;
    struct http_server_listener_t *listener_p;
    struct http_server_connection_t *connection_p;
    struct socket_addr_t addr;

    thrd_set_name(self_p->listener_p->thrd.name_p);

    listener_p = self_p->listener_p;

    socket_open(&listener_p->socket,
                SOCKET_DOMAIN_AF_INET,
                SOCKET_TYPE_STREAM,
                0);
    addr.ip = htonl(0xa9fe0102);
    addr.port = listener_p->port;
    socket_bind(&listener_p->socket, &addr, sizeof(addr));
    socket_listen(&listener_p->socket, 3);

    /* Wait for clients to connect. */
    while (1) {
        /* Allocate a connection. */
        connection_p = allocate_connection(self_p);

        /* Wait for a client to connect. */
        socket_accept(&listener_p->socket,
                      &connection_p->socket,
                      &addr,
                      NULL);

        handle_accept(self_p, connection_p);
    }

    return (NULL);
}

static int
response_write_text_plain(struct http_server_connection_t *connection_p,
                          struct http_server_request_t *request_p,
                          struct http_server_response_t *response_p)
{
    int res = 0;
    ssize_t size;
    char buf[128];

    /* Write the header. */
    if (response_p->code == http_server_response_code_200_ok_t) {
        size = std_sprintf(buf,
                           ok_fmt,
                           "text/plain",
                           response_p->content.u.text.plain.size);
    } else {
        size = std_sprintf(buf,
                           not_found_fmt,
                           "text/plain",
                           response_p->content.u.text.plain.size);
    }

    res = socket_write(&connection_p->socket, buf, size);

    if (res != size) {
        return (-1);
    }

    /* Write the content. */
    if (response_p->content.u.text.plain.buf_p != NULL) {
        res = socket_write(&connection_p->socket,
                           response_p->content.u.text.plain.buf_p,
                           response_p->content.u.text.plain.size);
            
        if (res != response_p->content.u.text.plain.size) {
            return (-1);
        }
    } else {
        res = 0;
    }
    
    return (res);
}

static int
response_write_text_html(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p,
                         struct http_server_response_t *response_p)
{
    int res = 0;
    ssize_t size;
    char buf[128];

    /* Write the header. */
    if (response_p->code == http_server_response_code_200_ok_t) {
        size = std_sprintf(buf,
                           ok_fmt,
                           "text/html",
                           response_p->content.u.text.html.size);
    } else {
        size = std_sprintf(buf,
                           not_found_fmt,
                           "text/html",
                           response_p->content.u.text.html.size);
    }

    res = socket_write(&connection_p->socket, buf, size);

    if (res != size) {
        return (-1);
    }

    /* Write the content. */
    if (response_p->content.u.text.html.buf_p != NULL) {
        res = socket_write(&connection_p->socket,
                           response_p->content.u.text.html.buf_p,
                           response_p->content.u.text.html.size);
            
        if (res != response_p->content.u.text.html.size) {
            return (-1);
        }
    } else {
        res = 0;
    }

    return (res);
}

int http_server_init(struct http_server_t *self_p,
                     struct http_server_listener_t *listener_p,
                     struct http_server_connection_t *connections_p,
                     const char *root_path_p,
                     const struct http_server_route_t *routes_p,
                     http_server_route_callback_t on_no_route)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(listener_p != NULL, EINVAL)
    ASSERTN(connections_p != NULL, EINVAL);
    ASSERTN(routes_p != NULL, EINVAL);
    ASSERTN(on_no_route != NULL, EINVAL);

    struct http_server_connection_t *connection_p;

    self_p->listener_p = listener_p;
    self_p->connections_p = connections_p;
    self_p->root_path_p = root_path_p;
    self_p->routes_p = routes_p;
    self_p->on_no_route = on_no_route;

    connection_p = self_p->connections_p;

    while (connection_p->thrd.name_p != NULL) {
        connection_p->state = http_server_connection_state_free_t;
        connection_p->self_p = self_p;
        event_init(&connection_p->events);

        connection_p++;
    }

    event_init(&self_p->events);

    return (0);
}

int http_server_start(struct http_server_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    struct http_server_connection_t *connection_p;

    /* Spawn the listener thread. */
    self_p->listener_p->thrd.id_p =
        thrd_spawn(listener_main,
                   self_p,
                   0,
                   self_p->listener_p->thrd.stack.buf_p,
                   self_p->listener_p->thrd.stack.size);

    connection_p = self_p->connections_p;

    /* Spawn the connection threads. */
    while (connection_p->thrd.stack.buf_p != NULL) {
        connection_p->thrd.id_p =
            thrd_spawn(connection_main,
                       connection_p,
                       0,
                       connection_p->thrd.stack.buf_p,
                       connection_p->thrd.stack.size);

        connection_p++;
    }

    return (0);
}

int http_server_stop(struct http_server_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (0);
}

int
http_server_response_write(struct http_server_connection_t *connection_p,
                           struct http_server_request_t *request_p,
                           struct http_server_response_t *response_p)
{
    ASSERTN(connection_p != NULL, EINVAL);
    ASSERTN(request_p != NULL, EINVAL);
    ASSERTN(response_p != NULL, EINVAL);

    int res = 0;

    if (response_p->content.type == http_server_content_type_text_plain_t) {
        res = response_write_text_plain(connection_p,
                                        request_p,
                                        response_p);
    } else if (response_p->content.type == http_server_content_type_text_html_t) {
        res = response_write_text_html(connection_p,
                                       request_p,
                                       response_p);
    } else {
        res = -1;
    }

    return (res);
}
