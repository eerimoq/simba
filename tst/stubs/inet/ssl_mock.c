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
#include "ssl_mock.h"

int mock_write_ssl_module_init(int res)
{
    harness_mock_write("ssl_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_module_init)()
{
    int res;

    harness_mock_read("ssl_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_context_init(enum ssl_protocol_t protocol,
                                int res)
{
    harness_mock_write("ssl_context_init(protocol)",
                       &protocol,
                       sizeof(protocol));

    harness_mock_write("ssl_context_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_context_init)(struct ssl_context_t *self_p,
                                                  enum ssl_protocol_t protocol)
{
    int res;

    harness_mock_assert("ssl_context_init(protocol)",
                        &protocol);

    harness_mock_read("ssl_context_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_context_destroy(int res)
{
    harness_mock_write("ssl_context_destroy(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_context_destroy)(struct ssl_context_t *self_p)
{
    int res;

    harness_mock_read("ssl_context_destroy(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_context_load_cert_chain(const char *cert_p,
                                           const char *key_p,
                                           int res)
{
    harness_mock_write("ssl_context_load_cert_chain(): return (cert_p)",
                       cert_p,
                       strlen(cert_p) + 1);

    harness_mock_write("ssl_context_load_cert_chain(): return (key_p)",
                       key_p,
                       strlen(key_p) + 1);

    harness_mock_write("ssl_context_load_cert_chain(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_context_load_cert_chain)(struct ssl_context_t *self_p,
                                                             const char *cert_p,
                                                             const char *key_p)
{
    int res;

    harness_mock_read("ssl_context_load_cert_chain(): return (cert_p)",
                      cert_p,
                      -1);

    harness_mock_read("ssl_context_load_cert_chain(): return (key_p)",
                      key_p,
                      -1);

    harness_mock_read("ssl_context_load_cert_chain(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_context_load_verify_location(const char *ca_certs_p,
                                                int res)
{
    harness_mock_write("ssl_context_load_verify_location(ca_certs_p)",
                       ca_certs_p,
                       strlen(ca_certs_p) + 1);

    harness_mock_write("ssl_context_load_verify_location(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_context_load_verify_location)(struct ssl_context_t *self_p,
                                                                  const char *ca_certs_p)
{
    int res;

    harness_mock_assert("ssl_context_load_verify_location(ca_certs_p)",
                        ca_certs_p);

    harness_mock_read("ssl_context_load_verify_location(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_context_set_verify_mode(enum ssl_verify_mode_t mode,
                                           int res)
{
    harness_mock_write("ssl_context_set_verify_mode(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("ssl_context_set_verify_mode(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_context_set_verify_mode)(struct ssl_context_t *self_p,
                                                             enum ssl_verify_mode_t mode)
{
    int res;

    harness_mock_assert("ssl_context_set_verify_mode(mode)",
                        &mode);

    harness_mock_read("ssl_context_set_verify_mode(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_socket_open(struct ssl_context_t *context_p,
                               void *socket_p,
                               int flags,
                               const char *server_hostname_p,
                               int res)
{
    harness_mock_write("ssl_socket_open(context_p)",
                       context_p,
                       sizeof(*context_p));

    harness_mock_write("ssl_socket_open(socket_p)",
                       socket_p,
                       sizeof(socket_p));

    harness_mock_write("ssl_socket_open(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("ssl_socket_open(server_hostname_p)",
                       server_hostname_p,
                       strlen(server_hostname_p) + 1);

    harness_mock_write("ssl_socket_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_socket_open)(struct ssl_socket_t *self_p,
                                                 struct ssl_context_t *context_p,
                                                 void *socket_p,
                                                 int flags,
                                                 const char *server_hostname_p)
{
    int res;

    harness_mock_assert("ssl_socket_open(context_p)",
                        context_p);

    harness_mock_assert("ssl_socket_open(socket_p)",
                        socket_p);

    harness_mock_assert("ssl_socket_open(flags)",
                        &flags);

    harness_mock_assert("ssl_socket_open(server_hostname_p)",
                        server_hostname_p);

    harness_mock_read("ssl_socket_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_socket_close(int res)
{
    harness_mock_write("ssl_socket_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_socket_close)(struct ssl_socket_t *self_p)
{
    int res;

    harness_mock_read("ssl_socket_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_socket_write(const void *buf_p,
                                size_t size,
                                ssize_t res)
{
    harness_mock_write("ssl_socket_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("ssl_socket_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("ssl_socket_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(ssl_socket_write)(struct ssl_socket_t *self_p,
                                                      const void *buf_p,
                                                      size_t size)
{
    ssize_t res;

    harness_mock_assert("ssl_socket_write(buf_p)",
                        buf_p);

    harness_mock_assert("ssl_socket_write(size)",
                        &size);

    harness_mock_read("ssl_socket_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_socket_read(void *buf_p,
                               size_t size,
                               ssize_t res)
{
    harness_mock_write("ssl_socket_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("ssl_socket_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("ssl_socket_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(ssl_socket_read)(struct ssl_socket_t *self_p,
                                                     void *buf_p,
                                                     size_t size)
{
    ssize_t res;

    harness_mock_assert("ssl_socket_read(buf_p)",
                        buf_p);

    harness_mock_assert("ssl_socket_read(size)",
                        &size);

    harness_mock_read("ssl_socket_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_socket_size(ssize_t res)
{
    harness_mock_write("ssl_socket_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(ssl_socket_size)(struct ssl_socket_t *self_p)
{
    ssize_t res;

    harness_mock_read("ssl_socket_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_socket_get_server_hostname(const char *res)
{
    harness_mock_write("ssl_socket_get_server_hostname(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(ssl_socket_get_server_hostname)(struct ssl_socket_t *self_p)
{
    const char *res;

    harness_mock_read("ssl_socket_get_server_hostname(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ssl_socket_get_cipher(const char **cipher_pp,
                                     const char **protocol_pp,
                                     int *number_of_secret_bits_p,
                                     int res)
{
    harness_mock_write("ssl_socket_get_cipher(): return (cipher_pp)",
                       cipher_pp,
                       strlen(cipher_pp) + 1);

    harness_mock_write("ssl_socket_get_cipher(): return (protocol_pp)",
                       protocol_pp,
                       strlen(protocol_pp) + 1);

    harness_mock_write("ssl_socket_get_cipher(): return (number_of_secret_bits_p)",
                       number_of_secret_bits_p,
                       sizeof(*number_of_secret_bits_p));

    harness_mock_write("ssl_socket_get_cipher(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ssl_socket_get_cipher)(struct ssl_socket_t *self_p,
                                                       const char **cipher_pp,
                                                       const char **protocol_pp,
                                                       int *number_of_secret_bits_p)
{
    int res;

    harness_mock_read("ssl_socket_get_cipher(): return (cipher_pp)",
                      cipher_pp,
                      -1);

    harness_mock_read("ssl_socket_get_cipher(): return (protocol_pp)",
                      protocol_pp,
                      -1);

    harness_mock_read("ssl_socket_get_cipher(): return (number_of_secret_bits_p)",
                      number_of_secret_bits_p,
                      -1);

    harness_mock_read("ssl_socket_get_cipher(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
