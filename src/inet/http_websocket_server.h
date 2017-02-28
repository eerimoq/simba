/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
