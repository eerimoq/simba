/**
 * @file main.c
 * @version 0.0.1
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#include "simba.h"

#include "esp_common.h"

#if !defined(SSID) || !defined(PASSWORD)
#    error "WiFi connection variables SSID and PASSWORD must be set."
#endif

static struct http_server_t server;
static struct uart_driver_t uart;
static char rxbuf[8];
static struct shell_args_t shell_args;

THRD_STACK(listener_stack, 1024);
THRD_STACK(connection_stack, 1500);

static int request_index(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p);

static struct http_server_route_t routes[] = {
    { .path_p = "/index.html", .callback = request_index },
    { .path_p = NULL, .callback = NULL }
};

static struct http_server_listener_t listener = {
    .address_p = "192.168.1.106",
    .port = 8000,
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

    { .thrd = { .name_p = NULL } }
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
        "    Hello from Simba!"
        "  </body>\n"
        "</html>\n";
    struct http_server_response_t response;

    /* Only the GET action is supported. */
    if (request_p->action != http_server_request_action_get_t) {
        return (-1);
    }

    /* Create the response. */
    response.code = http_server_response_code_200_ok_t;
    response.content.type = http_server_content_type_text_html_t;
    response.content.u.text.html.buf_p = index_html;
    response.content.u.text.html.size =
        strlen(response.content.u.text.html.buf_p);

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
    response.content.u.text.html.buf_p = NULL;
    response.content.u.text.html.size = 0;

    return (http_server_response_write(connection_p,
                                       request_p,
                                       &response));
}

static int init()
{
    struct station_config sta_config;
    struct ip_info ip_config;

    /* Start WiFi in station mode. */
    wifi_set_opmode_current(STATION_MODE);

    memset(&sta_config, 0, sizeof(sta_config));
    std_sprintf((char *)sta_config.ssid, FSTR("%s"), STRINGIFY(SSID));
    std_sprintf((char *)sta_config.password, FSTR("%s"), STRINGIFY(PASSWORD));

    wifi_station_set_config(&sta_config);

    wifi_get_ip_info(STATION_IF, &ip_config);

    while (ip_config.ip.addr == 0) {
        wifi_get_ip_info(STATION_IF, &ip_config);
    }

    sys_start();

    uart_module_init();
    uart_init(&uart, &uart_device[0], 38400, rxbuf, sizeof(rxbuf));
    uart_start(&uart);
    sys_set_stdout(&uart.chout);

    std_printf(FSTR("Connected to AP. Got IP 0x%x\r\n"),
               ip_config.ip.addr);

    log_set_default_handler_output_channel(sys_get_stdout());

    socket_module_init();

    http_server_init(&server,
                     &listener,
                     connections,
                     NULL,
                     routes,
                     no_route);

    http_server_start(&server);

    return (0);
}

int main()
{
    init();

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_main(&shell_args);

    return (0);
}
