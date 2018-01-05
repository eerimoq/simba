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

#include "simba.h"
#include "socket_mock.h"

int mock_write_socket_module_init(int res)
{
    harness_mock_write("socket_module_init()",
                       NULL,
                       0);

    harness_mock_write("socket_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_module_init)()
{
    int res;

    harness_mock_assert("socket_module_init()",
                        NULL,
                        0);

    harness_mock_read("socket_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_open_tcp(int res)
{
    harness_mock_write("socket_open_tcp(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_open_tcp)(struct socket_t *self_p)
{
    int res;

    harness_mock_read("socket_open_tcp(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_open_udp(int res)
{
    harness_mock_write("socket_open_udp(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_open_udp)(struct socket_t *self_p)
{
    int res;

    harness_mock_read("socket_open_udp(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_open_raw(int res)
{
    harness_mock_write("socket_open_raw(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_open_raw)(struct socket_t *self_p)
{
    int res;

    harness_mock_read("socket_open_raw(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_open(int domain,
                           int type,
                           int protocol,
                           int res)
{
    harness_mock_write("socket_open(domain)",
                       &domain,
                       sizeof(domain));

    harness_mock_write("socket_open(type)",
                       &type,
                       sizeof(type));

    harness_mock_write("socket_open(protocol)",
                       &protocol,
                       sizeof(protocol));

    harness_mock_write("socket_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_open)(struct socket_t *self_p,
                                             int domain,
                                             int type,
                                             int protocol)
{
    int res;

    harness_mock_assert("socket_open(domain)",
                        &domain,
                        sizeof(domain));

    harness_mock_assert("socket_open(type)",
                        &type,
                        sizeof(type));

    harness_mock_assert("socket_open(protocol)",
                        &protocol,
                        sizeof(protocol));

    harness_mock_read("socket_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_close(int res)
{
    harness_mock_write("socket_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_close)(struct socket_t *self_p)
{
    int res;

    harness_mock_read("socket_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_bind(const struct inet_addr_t *local_addr_p,
                           int res)
{
    harness_mock_write("socket_bind(local_addr_p)",
                       local_addr_p,
                       sizeof(*local_addr_p));

    harness_mock_write("socket_bind(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_bind)(struct socket_t *self_p,
                                             const struct inet_addr_t *local_addr_p)
{
    int res;

    harness_mock_assert("socket_bind(local_addr_p)",
                        local_addr_p,
                        sizeof(*local_addr_p));

    harness_mock_read("socket_bind(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_listen(int backlog,
                             int res)
{
    harness_mock_write("socket_listen(backlog)",
                       &backlog,
                       sizeof(backlog));

    harness_mock_write("socket_listen(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_listen)(struct socket_t *self_p,
                                               int backlog)
{
    int res;

    harness_mock_assert("socket_listen(backlog)",
                        &backlog,
                        sizeof(backlog));

    harness_mock_read("socket_listen(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_connect(const struct inet_addr_t *remote_addr_p,
                              int res)
{
    harness_mock_write("socket_connect(remote_addr_p)",
                       remote_addr_p,
                       sizeof(*remote_addr_p));

    harness_mock_write("socket_connect(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_connect)(struct socket_t *self_p,
                                                const struct inet_addr_t *remote_addr_p)
{
    int res;

    harness_mock_assert("socket_connect(remote_addr_p)",
                        remote_addr_p,
                        sizeof(*remote_addr_p));

    harness_mock_read("socket_connect(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_connect_by_hostname(const char *hostname_p,
                                          uint16_t port,
                                          int res)
{
    harness_mock_write("socket_connect_by_hostname(hostname_p)",
                       hostname_p,
                       strlen(hostname_p) + 1);

    harness_mock_write("socket_connect_by_hostname(port)",
                       &port,
                       sizeof(port));

    harness_mock_write("socket_connect_by_hostname(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_connect_by_hostname)(struct socket_t *self_p,
                                                            const char *hostname_p,
                                                            uint16_t port)
{
    int res;

    harness_mock_assert("socket_connect_by_hostname(hostname_p)",
                        hostname_p,
                        sizeof(*hostname_p));

    harness_mock_assert("socket_connect_by_hostname(port)",
                        &port,
                        sizeof(port));

    harness_mock_read("socket_connect_by_hostname(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_accept(struct socket_t *accepted_p,
                             struct inet_addr_t *remote_addr_p,
                             int res)
{
    harness_mock_write("socket_accept(): return (accepted_p)",
                       accepted_p,
                       sizeof(*accepted_p));

    harness_mock_write("socket_accept(): return (remote_addr_p)",
                       remote_addr_p,
                       sizeof(*remote_addr_p));

    harness_mock_write("socket_accept(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(socket_accept)(struct socket_t *self_p,
                                               struct socket_t *accepted_p,
                                               struct inet_addr_t *remote_addr_p)
{
    int res;

    harness_mock_read("socket_accept(): return (accepted_p)",
                      accepted_p,
                      sizeof(*accepted_p));

    harness_mock_read("socket_accept(): return (remote_addr_p)",
                      remote_addr_p,
                      sizeof(*remote_addr_p));

    harness_mock_read("socket_accept(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_sendto(const void *buf_p,
                             size_t size,
                             int flags,
                             const struct inet_addr_t *remote_addr_p,
                             ssize_t res)
{
    harness_mock_write("socket_sendto(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("socket_sendto(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("socket_sendto(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("socket_sendto(remote_addr_p)",
                       remote_addr_p,
                       sizeof(*remote_addr_p));

    harness_mock_write("socket_sendto(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(socket_sendto)(struct socket_t *self_p,
                                                   const void *buf_p,
                                                   size_t size,
                                                   int flags,
                                                   const struct inet_addr_t *remote_addr_p)
{
    ssize_t res;

    harness_mock_assert("socket_sendto(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("socket_sendto(size)",
                        &size,
                        sizeof(size));

    harness_mock_assert("socket_sendto(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_assert("socket_sendto(remote_addr_p)",
                        remote_addr_p,
                        sizeof(*remote_addr_p));

    harness_mock_read("socket_sendto(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_recvfrom(void *buf_p,
                               size_t size,
                               int flags,
                               struct inet_addr_t *remote_addr_p,
                               ssize_t res)
{
    harness_mock_write("socket_recvfrom(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("socket_recvfrom(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("socket_recvfrom(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("socket_recvfrom(remote_addr_p)",
                       remote_addr_p,
                       sizeof(*remote_addr_p));

    harness_mock_write("socket_recvfrom(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(socket_recvfrom)(struct socket_t *self_p,
                                                     void *buf_p,
                                                     size_t size,
                                                     int flags,
                                                     struct inet_addr_t *remote_addr_p)
{
    ssize_t res;

    harness_mock_assert("socket_recvfrom(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("socket_recvfrom(size)",
                        &size,
                        sizeof(size));

    harness_mock_assert("socket_recvfrom(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_assert("socket_recvfrom(remote_addr_p)",
                        remote_addr_p,
                        sizeof(*remote_addr_p));

    harness_mock_read("socket_recvfrom(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_write(const void *buf_p,
                            size_t size,
                            ssize_t res)
{
    harness_mock_write("socket_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("socket_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("socket_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(socket_write)(struct socket_t *self_p,
                                                  const void *buf_p,
                                                  size_t size)
{
    ssize_t res;

    harness_mock_assert("socket_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("socket_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("socket_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_read(void *buf_p,
                           size_t size,
                           ssize_t res)
{
    harness_mock_write("socket_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("socket_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("socket_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(socket_read)(struct socket_t *self_p,
                                                 void *buf_p,
                                                 size_t size)
{
    ssize_t res;

    harness_mock_assert("socket_read(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("socket_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("socket_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_socket_size(ssize_t res)
{
    harness_mock_write("socket_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(socket_size)(struct socket_t *self_p)
{
    ssize_t res;

    harness_mock_read("socket_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
