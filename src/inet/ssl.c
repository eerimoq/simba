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

mbedtls_ssl_cookie_ctx cookie_ctx;
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_ssl_context ssl;
mbedtls_ssl_config conf;
mbedtls_x509_crt cert;
mbedtls_pk_context key;
mbedtls_timing_delay_context timer;

int ssl_module_init()
{
    return (0);
}

int ssl_context_init(struct ssl_context_t *self_p,
                     enum ssl_protocol_t protocol)
{
    ASSERTN(self_p != NULL, -EINVAL);

    self_p->protocol = protocol;

    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_ssl_cookie_init(&cookie_ctx);
    mbedtls_x509_crt_init(&cert);
    mbedtls_pk_init(&key);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    
    return (0);
}

int ssl_context_load_cert_chain(struct ssl_context_t *self_p,
                                const char *cert_p,
                                const char *key_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(cert_p != NULL, -EINVAL);
    
    if (mbedtls_x509_crt_parse(&cert,
                               (unsigned char *)cert_p,
                               strlen(cert_p)) != 0) {
        return (-1);
    }

    if (key_p != NULL) {
        if (mbedtls_pk_parse_key(&key,
                                 (unsigned char *)key_p,
                                 strlen(key_p),
                                 NULL,
                                 0) != 0) {
            return (-1);
        }
    }

    return (0);
}

int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    enum ssl_socket_mode_t mode)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(context_p != NULL, -EINVAL);
    ASSERTN(socket_p != NULL, -EINVAL);

    return (0);
}

int ssl_socket_close(struct ssl_socket_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    return (0);
}

ssize_t ssl_socket_write(struct ssl_socket_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(buf_p != NULL, -EINVAL);

    return (0);
}

ssize_t ssl_socket_read(struct ssl_socket_t *self_p,
                        void *buf_p,
                        size_t size)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(buf_p != NULL, -EINVAL);

    return (0);
}

ssize_t ssl_socket_size(struct ssl_socket_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    return (0);
}
