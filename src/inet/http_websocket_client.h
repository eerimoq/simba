/**
 * @file inet/http_websocket_client.h
 * @version 3.1.0
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
