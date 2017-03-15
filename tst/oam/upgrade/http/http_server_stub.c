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

struct module_t {
    const struct http_server_route_t *routes_p;
    http_server_route_callback_t on_no_route;
};

static struct module_t module;

int http_server_init(struct http_server_t *self_p,
                     struct http_server_listener_t *listener_p,
                     struct http_server_connection_t *connections_p,
                     const char *root_path_p,
                     const struct http_server_route_t *routes_p,
                     http_server_route_callback_t on_no_route)
{
    BTASSERT(self_p != NULL);
    BTASSERT(listener_p != NULL);
    BTASSERT(connections_p != NULL);
    BTASSERT(root_path_p == NULL);
    BTASSERT(routes_p != NULL);
    BTASSERT(on_no_route != NULL);

    module.routes_p = routes_p;
    module.on_no_route = on_no_route;

    return (0);
}

#if CONFIG_HTTP_SERVER_SSL == 1

int http_server_wrap_ssl(struct http_server_t *self_p,
                         struct ssl_context_t *context_p)
{
    return (-1);
}

#endif

int http_server_start(struct http_server_t *self_p)
{
    return (0);
}

int http_server_stop(struct http_server_t *self_p)
{
    return (0);
}

int http_server_response_write(struct http_server_connection_t *connection_p,
                               struct http_server_request_t *request_p,
                               struct http_server_response_t *response_p)
{
    static int count = 0;

    count++;

    switch (count) {

    case 1:
        BTASSERT(response_p->code ==
                 http_server_response_code_400_bad_request_t);
        BTASSERT(response_p->content.type == 0);
        BTASSERT(strcmp(response_p->content.buf_p,
                        "enter application failed") == 0);
        BTASSERT(response_p->content.size == 24);
        break;

    case 2:
        BTASSERT(response_p->code ==
                 http_server_response_code_400_bad_request_t);
        BTASSERT(response_p->content.type == 0);
        BTASSERT(strcmp(response_p->content.buf_p,
                        "enter bootloader failed") == 0);
        BTASSERT(response_p->content.size == 23);
        break;

    default:
        return (-1);
    }

    return (0);
}

int http_server_stub_request(const char *path_p,
                             struct http_server_connection_t *connection_p,
                             struct http_server_request_t *request_p)
{
    const struct http_server_route_t *route_p;

    route_p = module.routes_p;

    while (route_p->path_p != NULL) {
        if (strcmp(route_p->path_p, path_p) == 0) {
            return (route_p->callback(connection_p, request_p));
        }

        route_p++;
    }

    return (-1);
}
