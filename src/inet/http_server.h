/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
    http_server_request_action_get_t = 0,
    http_server_request_action_post_t = 1
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
    http_server_response_code_400_bad_request_t = 400,
    http_server_response_code_401_unauthorized_t = 401,
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
        struct {
            int present;
            char value[52];
        } content_type;
        struct {
            int present;
            long value;
        } content_length;
        struct {
            int present;
            char value[64];
        } authorization;
        struct {
            int present;
            char value[20];
        } expect;
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
#if CONFIG_HTTP_SERVER_SSL == 1
    struct ssl_socket_t ssl_socket;
#endif
    void *chan_p;
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
    struct ssl_context_t *ssl_context_p;
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
 * Wrap given HTTP server in SSL, to make it secure.
 *
 * This function must be called after `http_server_init()` and before
 * `http_server_start()`.
 *
 * @param[in] self_p Http server to wrap in SSL.
 * @param[in] context_p SSL context to wrap the server in.
 *
 * @return zero(0) or negative error code.
 */
int http_server_wrap_ssl(struct http_server_t *self_p,
                         struct ssl_context_t *context_p);

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
 * @param[in] response_p Current response. If ``buf_p`` in the
 *                       response to NULL this function will only
 *                       write the HTTP header, including the size, to
 *                       the socket. After this function returns write
 *                       the payload by calling `socket_write()`.
 *
 * @return zero(0) or negative error code.
 */
int http_server_response_write(struct http_server_connection_t *connection_p,
                               struct http_server_request_t *request_p,
                               struct http_server_response_t *response_p);

#endif
