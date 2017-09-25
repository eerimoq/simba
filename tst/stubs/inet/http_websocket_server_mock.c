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
#include "http_websocket_server_mock.h"

int mock_write_http_websocket_server_init(struct socket_t *socket_p,
                                          int res)
{
    harness_mock_write("http_websocket_server_init(socket_p)",
                       socket_p,
                       sizeof(*socket_p));

    harness_mock_write("http_websocket_server_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(http_websocket_server_init)(struct http_websocket_server_t *self_p,
                                                            struct socket_t *socket_p)
{
    int res;

    harness_mock_assert("http_websocket_server_init(socket_p)",
                        socket_p);

    harness_mock_read("http_websocket_server_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_http_websocket_server_handshake(struct http_server_request_t *request_p,
                                               int res)
{
    harness_mock_write("http_websocket_server_handshake(request_p)",
                       request_p,
                       sizeof(*request_p));

    harness_mock_write("http_websocket_server_handshake(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(http_websocket_server_handshake)(struct http_websocket_server_t *self_p,
                                                                 struct http_server_request_t *request_p)
{
    int res;

    harness_mock_assert("http_websocket_server_handshake(request_p)",
                        request_p);

    harness_mock_read("http_websocket_server_handshake(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_http_websocket_server_read(int *type_p,
                                          void *buf_p,
                                          size_t size,
                                          ssize_t res)
{
    harness_mock_write("http_websocket_server_read(): return (type_p)",
                       type_p,
                       sizeof(*type_p));

    harness_mock_write("http_websocket_server_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("http_websocket_server_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("http_websocket_server_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(http_websocket_server_read)(struct http_websocket_server_t *self_p,
                                                                int *type_p,
                                                                void *buf_p,
                                                                size_t size)
{
    ssize_t res;

    harness_mock_read("http_websocket_server_read(): return (type_p)",
                      type_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("http_websocket_server_read(buf_p)",
                        buf_p);

    harness_mock_assert("http_websocket_server_read(size)",
                        &size);

    harness_mock_read("http_websocket_server_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_http_websocket_server_write(int type,
                                           const void *buf_p,
                                           uint32_t size,
                                           ssize_t res)
{
    harness_mock_write("http_websocket_server_write(type)",
                       &type,
                       sizeof(type));

    harness_mock_write("http_websocket_server_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("http_websocket_server_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("http_websocket_server_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(http_websocket_server_write)(struct http_websocket_server_t *self_p,
                                                                 int type,
                                                                 const void *buf_p,
                                                                 uint32_t size)
{
    ssize_t res;

    harness_mock_assert("http_websocket_server_write(type)",
                        &type);

    harness_mock_assert("http_websocket_server_write(buf_p)",
                        buf_p);

    harness_mock_assert("http_websocket_server_write(size)",
                        &size);

    harness_mock_read("http_websocket_server_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
