/**
 * @file main.c
 * @version 0.3.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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
 *
 * This file is part of the Simba project.
 */

#include "simba.h"
#include "inet.h"

#include "esp_common.h"

/* Ports. */
#define UDP_PORT   30303
#define TCP_PORT   40404
#define SHELL_PORT 50505

static struct uart_driver_t uart;
static struct shell_args_t shell_args;
THRD_STACK(shell_stack, 1024);

static int udp_test(void)
{
    struct socket_t sock;
    struct socket_addr_t addr;
    char buf[16];

    std_printf(FSTR("opening socket\r\n"));
    socket_open(&sock, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_DGRAM, 0);

    std_printf(FSTR("binding to %d\r\n"), UDP_PORT);
    addr.ip = 0x6701a8c0;
    addr.port = UDP_PORT;
    socket_bind(&sock, &addr, sizeof(addr));

    socket_read(&sock, buf, sizeof(buf));
    std_printf(FSTR("received '{}' from {}\r\n"), buf);

    std_printf(FSTR("sending '{}' to {}:{}\r\n"), buf);
    addr.ip = 0x6a01a8c0;
    addr.port = UDP_PORT;
    socket_sendto(&sock,
                  buf,
                  strlen(buf),
                  0,
                  &addr,
                  sizeof(addr));

    std_printf(FSTR("closing socket\r\n"));
    socket_close(&sock);

    return (0);
}

static int tcp_test(void)
{
    struct socket_t listener;
    struct socket_t client;
    struct socket_addr_t addr;
    char buf[16];

    std_printf(FSTR("opening socket\r\n"));
    socket_open(&listener, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_STREAM, 0);

    std_printf(FSTR("binding to %d\r\n"), TCP_PORT);
    addr.ip = 0x6701a8c0;
    addr.port = TCP_PORT;
    socket_bind(&listener, &addr, sizeof(addr));

    std_printf(FSTR("listening on {}\r\n"), TCP_PORT);
    socket_listen(&listener, 0);

    std_printf(FSTR("accepting '{}'\r\n"));
    socket_accept(&listener, &client, &addr, NULL);

    socket_read(&client, buf, 10);
    std_printf(FSTR("received '{}'\r\n"), buf);

    std_printf(FSTR("sending '{}'\r\n"), buf);
    socket_write(&client, buf, 10);

    std_printf(FSTR("closing socket\r\n"));
    socket_close(&client);
    socket_close(&listener);

    return (0);
}

static int shell_test(void)
{
    struct socket_t listener, client;
    struct socket_addr_t addr;

    /* Spawn the shell thread communicating over given TCP socket. */
    socket_open(&listener, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_STREAM, 0);
    addr.ip = 0x6701a8c0;
    addr.port = SHELL_PORT;
    socket_bind(&listener, &addr, sizeof(addr));
    socket_listen(&listener, 0);
    socket_accept(&listener, &client, &addr, NULL);

    shell_args.chin_p = &client;
    shell_args.chout_p = &client;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;

    shell_main(&shell_args);

    socket_close(&client);
    socket_close(&listener);

    return (0);
}

static int init()
{
    struct station_config sta_config;
    struct ip_info ip_config;

    sys_start();

    uart_module_init();
    uart_init(&uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&uart);
    sys_set_stdout(&uart.chout);

    /* Start WiFi in station mode. */
    wifi_set_opmode_current(STATION_MODE);

    memset(&sta_config, 0, sizeof(sta_config));
    std_sprintf((char *)sta_config.ssid, FSTR("%s"), STRINGIFY(SSID));
    std_sprintf((char *)sta_config.password, FSTR("%s"), STRINGIFY(PASSWORD));

    wifi_station_set_config(&sta_config);

    wifi_get_ip_info(STATION_IF, &ip_config);

    while (ip_config.ip.addr == 0) {
        thrd_usleep(50000);
        wifi_get_ip_info(STATION_IF, &ip_config);
    }

    std_printf(FSTR("Connected to AP. Got IP 0x%x\r\n"),
               ip_config.ip.addr);

    return (0);
}

int main()
{
    init();

    while (1) {
        udp_test();
        tcp_test();
        shell_test();
    }

    return (0);
}
