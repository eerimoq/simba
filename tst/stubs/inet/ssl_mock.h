/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#ifndef __SSL_MOCK_H__
#define __SSL_MOCK_H__

#include "simba.h"

int mock_write_ssl_module_init(int res);

int mock_write_ssl_context_init(enum ssl_protocol_t protocol,
                                int res);

int mock_write_ssl_context_destroy(int res);

int mock_write_ssl_context_load_cert_chain(const char *cert_p,
                                           const char *key_p,
                                           int res);

int mock_write_ssl_context_load_verify_location(const char *ca_certs_p,
                                                int res);

int mock_write_ssl_context_set_verify_mode(enum ssl_verify_mode_t mode,
                                           int res);

int mock_write_ssl_socket_open(struct ssl_context_t *context_p,
                               void *socket_p,
                               int flags,
                               const char *server_hostname_p,
                               int res);

int mock_write_ssl_socket_close(int res);

int mock_write_ssl_socket_write(const void *buf_p,
                                size_t size,
                                ssize_t res);

int mock_write_ssl_socket_read(void *buf_p,
                               size_t size,
                               ssize_t res);

int mock_write_ssl_socket_size(ssize_t res);

int mock_write_ssl_socket_get_server_hostname(const char *res);

int mock_write_ssl_socket_get_cipher(const char **cipher_pp,
                                     const char **protocol_pp,
                                     int *number_of_secret_bits_p,
                                     int res);

#endif
