/**
 * @section License
 *
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

#ifndef __SOCKET_MOCK_H__
#define __SOCKET_MOCK_H__

#include "simba.h"

int mock_write_socket_module_init(int res);

int mock_write_socket_open_tcp(int res);

int mock_write_socket_open_udp(int res);

int mock_write_socket_open_raw(int res);

int mock_write_socket_open(int domain,
                           int type,
                           int protocol,
                           int res);

int mock_write_socket_close(int res);

int mock_write_socket_bind(const struct inet_addr_t *local_addr_p,
                           int res);

int mock_write_socket_listen(int backlog,
                             int res);

int mock_write_socket_connect(const struct inet_addr_t *remote_addr_p,
                              int res);

int mock_write_socket_connect_by_hostname(const char *hostname_p,
                                          uint16_t port,
                                          int res);

int mock_write_socket_accept(struct socket_t *accepted_p,
                             struct inet_addr_t *remote_addr_p,
                             int res);

int mock_write_socket_sendto(const void *buf_p,
                             size_t size,
                             int flags,
                             const struct inet_addr_t *remote_addr_p,
                             ssize_t res);

int mock_write_socket_recvfrom(void *buf_p,
                               size_t size,
                               int flags,
                               struct inet_addr_t *remote_addr_p,
                               ssize_t res);

int mock_write_socket_write(const void *buf_p,
                            size_t size,
                            ssize_t res);

int mock_write_socket_read(void *buf_p,
                           size_t size,
                           ssize_t res);

int mock_write_socket_size(ssize_t res);

#endif
