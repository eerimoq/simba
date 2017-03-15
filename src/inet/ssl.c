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

#include "simba.h"

#include "mbedtls/config.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cookie.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/timing.h"

struct module_t {
    int initialized;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    int ssl_allocated;
    mbedtls_ssl_config conf;
    int conf_allocated;
    mbedtls_x509_crt cert;
    mbedtls_pk_context key;
    mbedtls_x509_crt ca_certs;
    mbedtls_timing_delay_context timer;
};

static struct module_t module;

static void *alloc_ssl(void)
{
    if (module.ssl_allocated == 1) {
        return (NULL);
    }

    module.ssl_allocated = 1;

    return (&module.ssl);
}

static void free_ssl(void *ssl_p)
{
    module.ssl_allocated = 0;
}

static void *alloc_conf(void)
{
    if (module.conf_allocated == 1) {
        return (NULL);
    }

    module.conf_allocated = 1;

    return (&module.conf);
}

static void free_conf(void *conf_p)
{
    module.conf_allocated = 0;
}

static int ssl_send(void *ctx_p,
                    const unsigned char *buf_p,
                    size_t len)
{
    return (socket_write(ctx_p, buf_p, len));
}

static int ssl_recv(void *ctx_p,
                    unsigned char *buf_p,
                    size_t len )
{
    return (socket_read(ctx_p, buf_p, len));
}

int ssl_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    mbedtls_entropy_init(&module.entropy);
    mbedtls_ctr_drbg_init(&module.ctr_drbg);

    /* Random generator seed initialization. */
    if (mbedtls_ctr_drbg_seed(&module.ctr_drbg,
                              mbedtls_entropy_func,
                              &module.entropy,
                              NULL,
                              0) != 0) {
        return (-1);
    }

    return (0);
}

int ssl_context_init(struct ssl_context_t *self_p,
                     enum ssl_protocol_t protocol)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(module.initialized == 1, EINVAL);

    self_p->protocol = protocol;

    /* Allocate the mbedTLS SSL config datastructure. */
    self_p->conf_p = alloc_conf();

    if (self_p->conf_p == NULL) {
        return (-1);
    }

    mbedtls_ssl_config_init(self_p->conf_p);
    mbedtls_ssl_conf_rng(self_p->conf_p,
                         mbedtls_ctr_drbg_random,
                         &module.ctr_drbg);

    self_p->server_side = -1;
    self_p->verify_mode = -1;

    return (0);
}

int ssl_context_destroy(struct ssl_context_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->conf_p != NULL, EINVAL);

    free_conf(self_p->conf_p);

    return (0);
}

int ssl_context_load_cert_chain(struct ssl_context_t *self_p,
                                const char *cert_p,
                                const char *key_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->conf_p != NULL, EINVAL);
    ASSERTN(cert_p != NULL, EINVAL);

    mbedtls_x509_crt_init(&module.cert);
    mbedtls_pk_init(&module.key);

    /* Parse the certificate. */
    if (mbedtls_x509_crt_parse(&module.cert,
                               (unsigned char *)cert_p,
                               strlen(cert_p) + 1) != 0) {
        return (-1);
    }

    /* Parse the key if present. */
    if (key_p != NULL) {
        if (mbedtls_pk_parse_key(&module.key,
                                 (unsigned char *)key_p,
                                 strlen(key_p) + 1,
                                 NULL,
                                 0) != 0) {
            return (-1);
        }
    }

    if (mbedtls_ssl_conf_own_cert(self_p->conf_p,
                                  &module.cert,
                                  &module.key) != 0) {
        return (-1);
    }

    return (0);
}

int ssl_context_load_verify_location(struct ssl_context_t *self_p,
                                     const char *ca_certs_p)
{
    /* Parse the CA certificate(s). */
    if (mbedtls_x509_crt_parse(&module.ca_certs,
                               (unsigned char *)ca_certs_p,
                               strlen(ca_certs_p) + 1) != 0) {
        return (-1);
    }

    mbedtls_ssl_conf_ca_chain(self_p->conf_p,
                              &module.ca_certs,
                              NULL);

    return (0);
}

int ssl_context_set_verify_mode(struct ssl_context_t *self_p,
                                enum ssl_verify_mode_t mode)
{
    /* Should be moved to a test case. */
    ASSERT(MBEDTLS_SSL_VERIFY_NONE == ssl_verify_mode_cert_none_t);
    ASSERT(MBEDTLS_SSL_VERIFY_REQUIRED == ssl_verify_mode_cert_required_t);

    self_p->verify_mode = mode;

    return (0);
}

int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    int flags,
                    const char *server_hostname_p)
{
    ASSERTN(module.initialized == 1, EINVAL);
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(context_p != NULL, EINVAL);
    ASSERTN(context_p->conf_p != NULL, EINVAL);
    ASSERTN(socket_p != NULL, EINVAL);

    int res;
    int authmode;
    int server_side;

    server_side = (flags & SSL_SOCKET_SERVER_SIDE);
    
    /* This implementation can only handle a single socket mode,
       server or client, in a context. Which mode to configure is
       first known in this function, so the context cannot be
       configured earlier. */
    if (context_p->server_side == -1) {
        if (mbedtls_ssl_config_defaults(context_p->conf_p,
                                        server_side,
                                        MBEDTLS_SSL_TRANSPORT_STREAM,
                                        MBEDTLS_SSL_PRESET_DEFAULT) != 0) {
            return (-1);
        }

        /* Use mbedTLS default values (none for servers, required for
           clients. */
        if (context_p->verify_mode != -1) {
            mbedtls_ssl_conf_authmode(context_p->conf_p, context_p->verify_mode);
        }

        context_p->server_side = server_side;
    } else if (context_p->server_side != server_side) {
        return (-1);
    }

    self_p->socket_p = socket_p;

    /* Allocate the mbedTLS datastructures. */
    self_p->ssl_p = alloc_ssl();

    if (self_p->ssl_p == NULL) {
        return (-1);
    }

    chan_init(&self_p->base,
              (chan_read_fn_t)ssl_socket_read,
              (chan_write_fn_t)ssl_socket_write,
              (chan_size_fn_t)ssl_socket_size);

    /* Inilialize the SSL session. */
    mbedtls_ssl_init(self_p->ssl_p);

    res = mbedtls_ssl_setup(self_p->ssl_p, &module.conf);

    if (res != 0) {
        goto err1;
    }

    mbedtls_ssl_set_bio(self_p->ssl_p,
                        socket_p,
                        ssl_send,
                        ssl_recv,
                        NULL);

    /* Server hostname for client side sockets. */
    if (server_side == 0) {
        if (server_hostname_p != NULL) {
            if (mbedtls_ssl_set_hostname(self_p->ssl_p,
                                         server_hostname_p) != 0) {
                goto err2;
            }
        }
    }
    
    /* Perform the handshake with the remote peer. */
    res = mbedtls_ssl_handshake(self_p->ssl_p);

    if (res != 0) {
        goto err2;
    }

    /* Verify the peer certificate if optional and present. */
    authmode = ((mbedtls_ssl_config *)context_p->conf_p)->authmode;

    if (authmode == MBEDTLS_SSL_VERIFY_OPTIONAL) {
        if (mbedtls_ssl_get_verify_result(self_p->ssl_p) != 0) {
            std_printf(FSTR("Verification of the peer certificate failed.\r\n"));
            res = -1;
            goto err2;
        }
    }

    return (0);

 err2:
    mbedtls_ssl_free(self_p->ssl_p);
 err1:
    free_ssl(self_p->ssl_p);

    return (res);
}

int ssl_socket_close(struct ssl_socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);

    mbedtls_ssl_close_notify(self_p->ssl_p);
    mbedtls_ssl_free(self_p->ssl_p);
    free_ssl(self_p->ssl_p);

    return (0);
}

ssize_t ssl_socket_write(struct ssl_socket_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (mbedtls_ssl_write(self_p->ssl_p, buf_p, size));
}

ssize_t ssl_socket_read(struct ssl_socket_t *self_p,
                        void *buf_p,
                        size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (mbedtls_ssl_read(self_p->ssl_p, buf_p, size));
}

ssize_t ssl_socket_size(struct ssl_socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);

    return (mbedtls_ssl_get_bytes_avail(self_p->ssl_p)
            + chan_size(self_p->socket_p));
}

const char *ssl_socket_get_server_hostname(struct ssl_socket_t *self_p)
{
    return (((mbedtls_ssl_context *)self_p->ssl_p)->hostname);
}

int ssl_socket_get_cipher(struct ssl_socket_t *self_p,
                          const char **cipher_pp,
                          const char **protocol_pp,
                          int *number_of_secret_bits_p)
{
    *cipher_pp = mbedtls_ssl_get_ciphersuite(self_p->ssl_p);
    *protocol_pp = mbedtls_ssl_get_version(self_p->ssl_p);
    *number_of_secret_bits_p = -1;
    
    return (0);
}
