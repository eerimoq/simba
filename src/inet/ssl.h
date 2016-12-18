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

enum ssl_protocol_t {
    ssl_protocol_tls_v1_0
};

enum ssl_socket_mode_t {
    ssl_socket_mode_client_t = 0,
    ssl_socket_mode_server_t
};

struct ssl_context_t {
    enum ssl_protocol_t protocol;
};

struct ssl_socket_t {
    struct chan_t base;
    void *socket_p; /* Often a TCP socket. */
};

/**
 * Initialize the SSL module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int ssl_module_init(void);

/**
 * Initialize given SSL context. A SSL context contains settings that
 * lives longer than a socket.
 *
 * @param[out] self_p SSL context to initialize.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_init(struct ssl_context_t *self_p,
                     enum ssl_protocol_t protocol);

/**
 * Load given certificate chain.
 *
 * @param[in] self_p SSL context.
 * @param[in] self_p Certificate to load.
 * @param[in] self_p Optional key to load.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_load_cert_chain(struct ssl_context_t *self_p,
                                const char *cert_p,
                                const char *key_p);

/**
 * Initialize given SSL socket with given socket and SSL
 * context. Performs the SSL handshake.
 *
 * @param[out] self_p SSL socket to initialize.
 * @param[in] context_p SSL context to execute in.
 * @param[in] socket_p Socket to wrap in the SSL socket.
 * @param[in] mode Server or client side socket mode.
 *
 * @return zero(0) or negative error code.
 */
int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    enum ssl_socket_mode_t mode);

/**
 * Close given SSL socket.
 *
 * @param[in] self_p Opened SSL socket.
 *
 * @return zero(0) or negative error code.
 */
int ssl_socket_close(struct ssl_socket_t *self_p);

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
