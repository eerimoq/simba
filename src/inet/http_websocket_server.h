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

#ifndef __INET_HTTP_WEBSOCKET_SERVER_H__
#define __INET_HTTP_WEBSOCKET_SERVER_H__

#include "simba.h"

struct http_websocket_server_t {
    struct socket_t *socket_p;
};

/**
 * Initialize given websocket server. The server uses the http module
 * interface to communicate with the client.
 *
 * @param[in] self_p Http to initialize.
 * @param[in] socket_p Connected socket.
 *
 * @return zero(0) or negative error code.
 */
int http_websocket_server_init(struct http_websocket_server_t *self_p,
                               struct socket_t *socket_p);

/**
 * Read the handshake request from the client and send the handshake
 * response.
 *
 * @param[in] self_p Websocket server.
 * @param[in] request_p Read handshake request.
 *
 * @return zero(0) or negative error code.
 */
int http_websocket_server_handshake(struct http_websocket_server_t *self_p,
                                    struct http_server_request_t *request_p);

/**
 * Read a message from given websocket.
 *
 * @param[in] self_p Websocket to read from.
 * @param[out] type_p Read message type.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read. Longer messages will be
 *                 truncated and the leftover data dropped.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t http_websocket_server_read(struct http_websocket_server_t *self_p,
                                   int *type_p,
                                   void *buf_p,
                                   size_t size);

/**
 * Write given message to given websocket.
 *
 * @param[in] self_p Websocket to write to.
 * @param[in] type One of ``HTTP_TYPE_TEXT`` and ``HTTP_TYPE_BINARY``.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t http_websocket_server_write(struct http_websocket_server_t *self_p,
                                    int type,
                                    const void *buf_p,
                                    uint32_t size);

#endif
