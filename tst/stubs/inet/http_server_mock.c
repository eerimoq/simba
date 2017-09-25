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
#include "http_server_mock.h"

int mock_write_http_server_init(struct http_server_listener_t *listener_p,
                                struct http_server_connection_t *connections_p,
                                const char *root_path_p,
                                const struct http_server_route_t *routes_p,
                                http_server_route_callback_t on_no_route,
                                int res)
{
    harness_mock_write("http_server_init(listener_p)",
                       listener_p,
                       sizeof(*listener_p));

    harness_mock_write("http_server_init(connections_p)",
                       connections_p,
                       sizeof(*connections_p));

    harness_mock_write("http_server_init(root_path_p)",
                       root_path_p,
                       strlen(root_path_p) + 1);

    harness_mock_write("http_server_init(routes_p)",
                       routes_p,
                       sizeof(*routes_p));

    harness_mock_write("http_server_init(on_no_route)",
                       &on_no_route,
                       sizeof(on_no_route));

    harness_mock_write("http_server_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(http_server_init)(struct http_server_t *self_p,
                                                  struct http_server_listener_t *listener_p,
                                                  struct http_server_connection_t *connections_p,
                                                  const char *root_path_p,
                                                  const struct http_server_route_t *routes_p,
                                                  http_server_route_callback_t on_no_route)
{
    int res;

    harness_mock_assert("http_server_init(listener_p)",
                        listener_p,
                        sizeof(*listener_p));

    harness_mock_assert("http_server_init(connections_p)",
                        connections_p,
                        sizeof(*connections_p));

    harness_mock_assert("http_server_init(root_path_p)",
                        root_path_p,
                        sizeof(*root_path_p));

    harness_mock_assert("http_server_init(routes_p)",
                        routes_p,
                        sizeof(*routes_p));

    harness_mock_assert("http_server_init(on_no_route)",
                        &on_no_route,
                        sizeof(on_no_route));

    harness_mock_read("http_server_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_http_server_wrap_ssl(struct ssl_context_t *context_p,
                                    int res)
{
    harness_mock_write("http_server_wrap_ssl(context_p)",
                       context_p,
                       sizeof(*context_p));

    harness_mock_write("http_server_wrap_ssl(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(http_server_wrap_ssl)(struct http_server_t *self_p,
                                                      struct ssl_context_t *context_p)
{
    int res;

    harness_mock_assert("http_server_wrap_ssl(context_p)",
                        context_p,
                        sizeof(*context_p));

    harness_mock_read("http_server_wrap_ssl(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_http_server_start(int res)
{
    harness_mock_write("http_server_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(http_server_start)(struct http_server_t *self_p)
{
    int res;

    harness_mock_read("http_server_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_http_server_stop(int res)
{
    harness_mock_write("http_server_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(http_server_stop)(struct http_server_t *self_p)
{
    int res;

    harness_mock_read("http_server_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_http_server_response_write(struct http_server_connection_t *connection_p,
                                          struct http_server_request_t *request_p,
                                          struct http_server_response_t *response_p,
                                          int res)
{
    harness_mock_write("http_server_response_write(connection_p)",
                       connection_p,
                       sizeof(*connection_p));

    harness_mock_write("http_server_response_write(request_p)",
                       request_p,
                       sizeof(*request_p));

    harness_mock_write("http_server_response_write(response_p)",
                       response_p,
                       sizeof(*response_p));

    harness_mock_write("http_server_response_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(http_server_response_write)(struct http_server_connection_t *connection_p,
                                                            struct http_server_request_t *request_p,
                                                            struct http_server_response_t *response_p)
{
    int res;

    harness_mock_assert("http_server_response_write(connection_p)",
                        connection_p,
                        sizeof(*connection_p));

    harness_mock_assert("http_server_response_write(request_p)",
                        request_p,
                        sizeof(*request_p));

    harness_mock_assert("http_server_response_write(response_p)",
                        response_p,
                        sizeof(*response_p));

    harness_mock_read("http_server_response_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
