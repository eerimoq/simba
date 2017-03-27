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

#ifndef __INET_SSL_H__
#define __INET_SSL_H__

#include "simba.h"

/**
 * Server side socket.
 */
#define SSL_SOCKET_SERVER_SIDE                            0x1

enum ssl_protocol_t {
    ssl_protocol_tls_v1_0
};

enum ssl_verify_mode_t {
    ssl_verify_mode_cert_none_t = 0,
    ssl_verify_mode_cert_required_t = 2
};

struct ssl_context_t {
    enum ssl_protocol_t protocol;
    void *conf_p;
    int server_side;
    int verify_mode;
};

struct ssl_socket_t {
    struct chan_t base;
    void *ssl_p;
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
 * @param[in] protocol SSL protocol to use.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_init(struct ssl_context_t *self_p,
                     enum ssl_protocol_t protocol);

/**
 * Destroy given SSL context. The context may not be used after it has
 * been destroyed.
 *
 * @param[in] self_p SSL context to destroy.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_destroy(struct ssl_context_t *self_p);

/**
 * Load given certificate chain into given context.
 *
 * @param[in] self_p SSL context.
 * @param[in] self_p Certificate to load.
 * @param[in] self_p Optional key to load. May be NULL.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_load_cert_chain(struct ssl_context_t *self_p,
                                const char *cert_p,
                                const char *key_p);

/**
 * Load a set of "certification authority" (CA) certificates used to
 * validate other peers’ certificates when ``verify_mode`` is other
 * than `ssl_verify_mode_cert_none_t`.
 *
 * @param[in] self_p SSL context.
 * @param[in] ca_certs_p CA certificates.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_load_verify_location(struct ssl_context_t *self_p,
                                     const char *ca_certs_p);

/**
 * Whether to try to verify other peers’ certificates.
 *
 * Load CA certificates with `ssl_context_load_verify_location()`.
 *
 * @param[in] self_p SSL context.
 * @param[in] mode Mode to set.
 *
 * @return zero(0) or negative error code.
 */
int ssl_context_set_verify_mode(struct ssl_context_t *self_p,
                                enum ssl_verify_mode_t mode);

/**
 * Initialize given SSL socket with given socket and SSL
 * context. Performs the SSL handshake.
 *
 * @param[out] self_p SSL socket to initialize.
 * @param[in] context_p SSL context to execute in.
 * @param[in] socket_p Socket to wrap in the SSL socket.
 * @param[in] flags Give as ``SSL_SOCKET_SERVER_SIDE`` for server side
 *                  sockets. Otherwise 0.
 * @param[in] server_hostname_p The server hostname used by client
 *                              side sockets to verify the
 *                              server. Give as NULL to skip the
 *                              verification. Must be NULL for server
 *                              side sockets.
 *
 * @return zero(0) or negative error code.
 */
int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    int flags,
                    const char *server_hostname_p);

/**
 * Close given SSL socket.
 *
 * @param[in] self_p SSL socket to close.
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

/**
 * Get the hostname of the server.
 *
 * @param[in] self_p SSL socket.
 *
 * @return Server hostname or NULL.
 */
const char *ssl_socket_get_server_hostname(struct ssl_socket_t *self_p);

/**
 * Get the cipher information.
 *
 * @param[in] self_p SSL socket.
 * @param[out] cipher_pp Connection cipher.
 * @param[out] protocol_pp Connection protocol.
 * @param[out] number_of_secret_bits_p Number of secret bits.
 *
 * @return zero(0) or negative error code.
 */
int ssl_socket_get_cipher(struct ssl_socket_t *self_p,
                          const char **cipher_pp,
                          const char **protocol_pp,
                          int *number_of_secret_bits_p);

#endif
