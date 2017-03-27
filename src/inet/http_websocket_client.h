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

#ifndef __INET_HTTP_WEBSOCKET_CLIENT_H__
#define __INET_HTTP_WEBSOCKET_CLIENT_H__

#include "simba.h"

struct http_websocket_client_t {
    struct {
        struct socket_t socket;
        const char *host_p;
        int port;
    } server;
    struct {
        size_t left;
    } frame;
    const char *path_p;
};

/**
 * Initialize given http.
 *
 * @param[in] self_p Http to initialize.
 * @param[in] server_p Server hostname to connect to.
 * @param[in] port Port to connect to.
 * @param[in] path_p Path.
 *
 * @return zero(0) or negative error code.
 */
int http_websocket_client_init(struct http_websocket_client_t *self_p,
                               const char *server_p,
                               int port,
                               const char *path_p);

/**
 * Connect given http to the server.
 *
 * @param[in] self_p Http to connect.
 *
 * @return zero(0) or negative error code.
 */
int http_websocket_client_connect(struct http_websocket_client_t *self_p);

/**
 * Disconnect given http from the server.
 *
 * @param[in] self_p Http to connect.
 *
 * @return zero(0) or negative error code.
 */
int http_websocket_client_disconnect(struct http_websocket_client_t *self_p);

/**
 * Read from given http.
 *
 * @param[in] self_p Http to read from.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read..
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t http_websocket_client_read(struct http_websocket_client_t *self_p,
                                   void *buf_p,
                                   size_t size);

/**
 * Write given data to given http.
 *
 * @param[in] self_p Http to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t http_websocket_client_write(struct http_websocket_client_t *self_p,
                                    int type,
                                    const void *buf_p,
                                    uint32_t size);

#endif
