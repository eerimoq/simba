/**
 * @file inet/http_server.h
 * @version 4.1.0
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
 * MERBITSTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __INET_HTTP_SERVER_H__
#define __INET_HTTP_SERVER_H__

#include "simba.h"

/**
 * Request action types.
 */
enum http_server_request_action_t {
    http_server_request_action_get_t = 0
};

/**
 * Content type.
 */
enum http_server_content_type_t {
    http_server_content_type_text_plain_t = 0,
    http_server_content_type_text_html_t = 1
};

/**
 * Response codes.
 */
enum http_server_response_code_t {
    http_server_response_code_200_ok_t = 200,
    http_server_response_code_404_not_found_t = 404
};

/**
 * Connection state.
 */
enum http_server_connection_state_t {
    http_server_connection_state_free_t = 0,
    http_server_connection_state_allocated_t
};

/**
 * HTTP request.
 */
struct http_server_request_t {
    enum http_server_request_action_t action;
    char path[64];
    struct {
        struct {
            int present;
            char value[32];
        } sec_websocket_key;
    } headers;
};

/**
 * HTTP response.
 */
struct http_server_response_t {
    int type;
    enum http_server_response_code_t code;
    struct {
        int type;
        const char *buf_p;
        size_t size;
    } content;
};

struct http_server_connection_t;

typedef int (*http_server_route_callback_t)(struct http_server_connection_t *connection_p,
                                            struct http_server_request_t *request_p);

struct http_server_listener_t {
    const char *address_p;
    int port;
    struct {
        const char *name_p;
        struct {
            void *buf_p;
            size_t size;
        } stack;
        struct thrd_t *id_p;
    } thrd;
    struct socket_t socket;
};

struct http_server_connection_t {
    enum http_server_connection_state_t state;
    struct {
        const char *name_p;
        struct {
            void *buf_p;
            size_t size;
        } stack;
        struct thrd_t *id_p;
    } thrd;
    struct http_server_t *self_p;
    struct socket_t socket;
    struct event_t events;
};

/**
 * Call given callback for given path.
 */
struct http_server_route_t {
    const char *path_p;
    http_server_route_callback_t callback;
};

struct http_server_t {
    const char *root_path_p;
    const struct http_server_route_t *routes_p;
    http_server_route_callback_t on_no_route;
    struct http_server_listener_t *listener_p;
    struct http_server_connection_t *connections_p;
    struct event_t events;
};

/**
 * Initialize given http server with given root path and maximum
 * number of clients.
 *
 * @param[in] self_p Http server to initialize.
 * @param[in] listener_p Listener.
 * @param[in] connections_p A NULL terminated list of connections.
 * @param[in] root_path_p Working directory for the connection threads.
 * @param[in] routes_p An array of routes.
 * @param[in] on_no_route Callback called for all requests without a
 *                        matching route in route_p.
 *
 * @return zero(0) or negative error code.
 */
int http_server_init(struct http_server_t *self_p,
                     struct http_server_listener_t *listener_p,
                     struct http_server_connection_t *connections_p,
                     const char *root_path_p,
                     const struct http_server_route_t *routes_p,
                     http_server_route_callback_t on_no_route);

/**
 * Start given HTTP server.
 *
 * Spawn the threads and start listening for connections.
 *
 * @param[in] self_p Http server.
 *
 * @return zero(0) or negative error code.
 */
int http_server_start(struct http_server_t *self_p);

/**
 * Stop given HTTP server.
 *
 * Closes the listener and all open connections, and then kills the
 * threads.
 *
 * @param[in] self_p Http server.
 *
 * @return zero(0) or negative error code.
 */
int http_server_stop(struct http_server_t *self_p);

/**
 * Write given HTTP response to given connected client. This function
 * should only be called from the route callbacks to respond to given
 * request.
 *
 * @param[in] connection_p Current connection.
 * @param[in] request_p Current request.
 * @param[in] response_p Current response.
 *
 * @return zero(0) or negative error code.
 */
int http_server_response_write(struct http_server_connection_t *connection_p,
                               struct http_server_request_t *request_p,
                               struct http_server_response_t *response_p);

#endif
