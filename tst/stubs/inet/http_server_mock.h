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

#ifndef __HTTP_SERVER_MOCK_H__
#define __HTTP_SERVER_MOCK_H__

#include "simba.h"

int mock_write_http_server_init(struct http_server_listener_t *listener_p,
                                struct http_server_connection_t *connections_p,
                                const char *root_path_p,
                                const struct http_server_route_t *routes_p,
                                http_server_route_callback_t on_no_route,
                                int res);

int mock_write_http_server_wrap_ssl(struct ssl_context_t *context_p,
                                    int res);

int mock_write_http_server_start(int res);

int mock_write_http_server_stop(int res);

int mock_write_http_server_response_write(struct http_server_connection_t *connection_p,
                                          struct http_server_request_t *request_p,
                                          struct http_server_response_t *response_p,
                                          int res);

#endif
