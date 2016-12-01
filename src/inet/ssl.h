/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __INET_SSL_H__
#define __INET_SSL_H__

#include "simba.h"

struct ssl_context_t {
};

struct ssl_socket_t {
    struct chan_t base;
    chan_t *socket_p; /* Often a TCP socket. */
};

/**
 * Initialize the ssl module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int ssl_module_init(void);

/**
 * Initialize given SSL context. A SSL context contains security
 * settings.
 *
 * @param[out] self_p SSL context to initialize.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_init(struct ssl_context_t *self_p);

/**
 * Initialize given SSL socket with given socket in given SSL context.
 *
 * @param[out] self_p SSL socket to initialize.
 * @param[in] context_p SSL context to execute in.
 * @param[in] socket_p Socket to wrap in the SSL socket.
 * @param[in] is_server Server side socket if ture(1), otherwise
 *                      client.
 *
 * @return zero(0) or negative error code.
 */
int ssl_socket_init(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    chan_t *socket_p,
                    int is_server);

/**
 * Start given SSL socket. Starting a SSL socket will perform a
 * handshake with the client/server to setup the secure connection.
 *
 * @param[in] self_p SSL socket to initialize.
 *
 * @return zero(0) or negative error code.
 */
int ssl_socket_start(struct ssl_socket_t *self_p);

/**
 * Stop given SSL socket. Stopping a SSL socket will close the
 * connection to the remote peer.
 *
 * @param[in] self_p SSL socket stop.
 *
 * @return zero(0) or negative error code.
 */
int ssl_socket_stop(struct ssl_socket_t *self_p);

/**
 * Write data to given SSL socket.
 *
 * @param[in] self_p SSL socket.
 * @param[in] buf_p Buffer to send.
 * @param[in] size Numer of bytes to send.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t ssl_socket_write(struct ssl_socket_t *self_p,
                         const void *buf_p,
                         size_t size);

/**
 * Read data from given SSL socket.
 *
 * @param[in] self_p SSL socket.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t ssl_socket_read(struct ssl_socket_t *self_p,
                        void *buf_p,
                        size_t size);

/**
 * Get the number of input bytes currently stored in the SSL
 * socket.
 *
 * @param[in] self_p SSL socket.
 *
 * @return Number of input bytes in the SSL socket.
 */
ssize_t ssl_socket_size(struct ssl_socket_t *self_p);

#endif
