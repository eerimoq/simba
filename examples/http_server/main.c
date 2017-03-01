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

#define IP 192.168.0.7

static struct http_server_t server;

THRD_STACK(listener_stack, 1024);
THRD_STACK(connection_stack, 1500);

static int request_index(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p);

static struct http_server_route_t routes[] = {
    { .path_p = "/index.html", .callback = request_index },
    { .path_p = NULL, .callback = NULL }
};

static struct http_server_listener_t listener = {
    .address_p = STRINGIFY(IP),
    .port = 80,
    .thrd = {
        .name_p = "http_listener",
        .stack = {
            .buf_p = listener_stack,
            .size = sizeof(listener_stack)
        }
    }
};

static struct http_server_connection_t connections[] = {
    {
        .thrd = {
            .name_p = "http_conn_0",
            .stack = {
                .buf_p = connection_stack,
                .size = sizeof(connection_stack)
            }
        }
    },
    {
        .thrd = {
            .name_p = NULL
        }
    }
};

/**
 * Handler for the index request.
 */
static int request_index(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p)
{
    static const char index_html[] =
        "<!DOCTYPE HTML>\n"
        "<html>\n"
        "  <body>\n"
        "    Hello from Simba!\n"
        "  </body>\n"
        "</html>\n";
    struct http_server_response_t response;

    std_printf(FSTR("/index.html requested\r\n"));

    /* Only the GET action is supported. */
    if (request_p->action != http_server_request_action_get_t) {
        return (-1);
    }

    /* Create the response. */
    response.code = http_server_response_code_200_ok_t;
    response.content.type = http_server_content_type_text_html_t;
    response.content.buf_p = index_html;
    response.content.size =
        strlen(response.content.buf_p);

    return (http_server_response_write(connection_p,
                                       request_p,
                                       &response));
}

/**
 * Default page handler.
 */
static int no_route(struct http_server_connection_t *connection_p,
                    struct http_server_request_t *request_p)
{
    struct http_server_response_t response;

    /* Create the response. */
    response.code = http_server_response_code_404_not_found_t;
    response.content.type = http_server_content_type_text_html_t;
    response.content.buf_p = NULL;
    response.content.size = 0;

    return (http_server_response_write(connection_p,
                                       request_p,
                                       &response));
}

int main()
{
    sys_start();

    /* Create the HTTPS server. */
    if (http_server_init(&server,
                         &listener,
                         connections,
                         NULL,
                         routes,
                         no_route) != 0) {
        std_printf(FSTR("http_server_init() failed\r\n"));
        return (-1);
    }

    if (http_server_start(&server) != 0) {
        std_printf(FSTR("http_server_start() failed\r\n"));
        return (-1);
    }

    std_printf(FSTR("HTTP server running. "
                    "Enter URL 'http://" STRINGIFY(IP) "/index.html' "
                    "in your web browser.\r\n"));

    thrd_suspend(NULL);

    return (0);
}
