/**
 * @file main.c
 * @version 0.4.0
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

#include "inet.h"

extern void socket_stub_init(void);
extern void socket_stub_accept();
extern void socket_stub_input(void *buf_p, size_t size);
extern void socket_stub_output(void *buf_p, size_t size);

static struct http_server_t foo;

THRD_STACK(listener_stack, 1024);
THRD_STACK(connection_stack, 1024);

/**
 * Handler for the index request.
 */
static int request_index(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p)
{
    struct http_server_response_t response;

    /* Only the GET action is supported. */
    if (request_p->action != http_server_request_action_get_t) {
        return (-1);
    }

    /* Create the response. */
    response.code = http_server_response_code_200_ok_t;
    response.content.type = http_server_content_type_text_plain_t;
    response.content.u.text.plain.buf_p = "Welcome!";
    response.content.u.text.plain.size = strlen(response.content.u.text.plain.buf_p);

    return (http_server_response_write(connection_p, request_p, &response));
}

/**
 * Handler for the websocket echo request. Echo all websocket messages
 * the client sends on the socket.
 */
static int request_websocket_echo(struct http_server_connection_t *connection_p,
                                  struct http_server_request_t *request_p)
{
    ssize_t res;
    struct http_websocket_server_t server;
    uint8_t buf[32];
    int type;

    if (http_websocket_server_init(&server, &connection_p->socket) != 0) {
        return (-1);
    }

    /* Handshake sequence. */
    if (http_websocket_server_handshake(&server, request_p) != 0) {
        return (-1);
    }

    /* Echo all read data. */
    while (1) {
        res = http_websocket_server_read(&server, &type, buf, sizeof(buf));

        /* Stop when the socket is closed. */
        if (res <= 0) {
            return (res);
        }

        if (http_websocket_server_write(&server, type, buf, res) != res) {
            return (-1);
        }
    }

    return (0);
}

/**
 * Handler for all requests except those in the route array.
 */
static int request_404_not_found(struct http_server_connection_t *connection_p,
                                 struct http_server_request_t *request_p)
{
    int res;
    char content[64];
    size_t size;
    struct http_server_response_t response;

    /* Create the response. */
    size = std_sprintf(content,
                       FSTR("The requested page '%s' could not be found."),
                       request_p->path);
    response.code = http_server_response_code_404_not_found_t;
    response.content.type = http_server_content_type_text_plain_t;
    response.content.u.text.plain.buf_p = NULL;
    response.content.u.text.plain.size = size;

    res = http_server_response_write(connection_p, request_p, &response);

    if (res != 0) {
        return (res);
    }

    if (socket_write(&connection_p->socket, content, size) != size) {
        return (-EIO);
    }

    return (0);
}

static int test_start(struct harness_t *harness_p)
{
    static struct http_server_route_t routes[] = {
        { .path_p = "/index.html", .callback = request_index },
        { .path_p = "/websocket/echo", .callback = request_websocket_echo },
        { .path_p = NULL, .callback = NULL }
    };

    static struct http_server_listener_t listener = {
        .address_p = "127.0.0.1",
        .port = 9000,
        .thrd = {
            .name_p = "http_listener",
            .stack = {
                .buf_p = listener_stack,
                .size = sizeof(listener_stack)
            }
        }
    };

    static struct http_server_connection_t connections[] = {
        {
            .thrd = {
                .name_p = "http_conn_0",
                .stack = {
                    .buf_p = connection_stack,
                    .size = sizeof(connection_stack)
                }
            }
        },
        {
            .thrd = {
                .name_p = "http_conn_1",
                .stack = {
                    .buf_p = NULL,
                    .size = 0
                }
            }
        },
        { .thrd = { .name_p = NULL } }
    };

    socket_stub_init();

    BTASSERT(http_server_init(&foo,
                              &listener,
                              connections,
                              NULL,
                              routes,
                              request_404_not_found) == 0);

    BTASSERT(http_server_start(&foo) == 0);

    thrd_set_log_mask(listener.thrd.id_p, LOG_UPTO(DEBUG));
    thrd_set_log_mask(connections[0].thrd.id_p, LOG_UPTO(DEBUG));

    return (0);
}

static int test_request_index(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input GET /index.html on the connection socket. */
    str_p =
        "GET /index.html HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 8\r\n"
        "\r\n"
        "Welcome!";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    return (0);
}

static int test_request_websocket(struct harness_t *harness_p)
{
    int i;
    char *str_p;
    char buf[128];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input the handshake GET /websocket/echo. */
    str_p =
        "GET /websocket/echo HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Upgrade: WebSocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the handshake response and verify it. */
    str_p =
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=\r\n"
        "\r\n";

    socket_stub_output(buf, strlen(str_p));
    BTASSERT(strcmp(buf, str_p) == 0);

    for (i = 0; i < 3; i++) {
        /* Input a websocket frame on the socket. */
        buf[0] = 0x81; /* FIN & TEXT. */
        buf[1] = 0x84; /* MASK and 1 byte payload length. */
        buf[2] = 0x00; /* Masking key 0. */
        buf[3] = 0x00; /* Masking key 1. */
        buf[4] = 0x00; /* Masking key 2. */
        buf[5] = 0x00; /* Masking key 3. */
        buf[6] = '1';  /* Payload 0. */
        buf[7] = '2';  /* Payload 1. */
        buf[8] = '3';  /* Payload 2. */
        buf[9] = '\0'; /* Payload 3. */
        socket_stub_input(buf, 10);

        /* Read the echo frame from the socket. */
        socket_stub_output(buf, 10);
        BTASSERT(strcmp(&buf[6], "123") == 0);
    }

    return (0);
}

static int test_request_no_route(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input GET /missing.html on the connection socket. */
    str_p =
        "GET /missing.html HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 54\r\n"
        "\r\n"
        "The requested page '/missing.html' could not be found.";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    return (0);
}

static int test_stop(struct harness_t *harness_p)
{
    BTASSERT(http_server_stop(&foo) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_start, "test_start" },
        { test_request_index, "test_request_index" },
        { test_request_no_route, "test_request_no_route" },
        { test_request_websocket, "test_request_websocket" },
        { test_stop, "test_stop" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
