/**
 * @file main.c
 * @version 3.1.1
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

#if defined(ARCH_ESP)
#    include "esp_wifi.h"
#    include "esp_sta.h"
#endif

#if !defined(SSID)
#    pragma message "WiFi connection variable SSID is not set. Using default value MySSID"
#    define SSID MySSID
#endif

#if !defined(PASSWORD)
#    pragma message "WiFi connection variable PASSWORD is not set. Using default value MyPassword"
#    define PASSWORD MyPassword
#endif

/* Ports. */
#define TCP_PORT   40404
#define SHELL_PORT 50505

static struct shell_t shell;
static struct network_interface_wifi_station_espressif_t wifi;

static int tcp_test(void)
{
    struct socket_t listener;
    struct socket_t client;
    struct inet_addr_t addr;
    char buf[16];

    std_printf(FSTR("TCP test\r\n"));

    std_printf(FSTR("opening listener socket\r\n"));
    socket_open(&listener, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_STREAM, 0);

    std_printf(FSTR("binding to %d\r\n"), TCP_PORT);
    inet_aton("192.168.1.103", &addr.ip);
    addr.port = TCP_PORT;
    socket_bind(&listener, &addr, sizeof(addr));

    std_printf(FSTR("listening on %d\r\n"), TCP_PORT);
    socket_listen(&listener, 5);

    socket_accept(&listener, &client, &addr, NULL);
    std_printf(FSTR("accepted client 0x%x:%d\r\n"), addr.ip, addr.port);

    socket_read(&client, buf, 5);
    socket_read(&client, &buf[5], 5);
    std_printf(FSTR("read '%s'\r\n"), buf);

    std_printf(FSTR("writing '%s'\r\n"), buf);
    socket_write(&client, buf, 10);

    std_printf(FSTR("closing client socket\r\n"));
    socket_close(&client);

    std_printf(FSTR("closing listener socket\r\n"));
    socket_close(&listener);

    return (0);
}

static int shell_test(void)
{
    struct socket_t listener, client;
    struct inet_addr_t addr;

    std_printf(FSTR("shell test\r\n"));

    /* Start the shell communicating over given TCP socket. */
    socket_open(&listener, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_STREAM, 0);
    inet_aton("192.168.1.103", &addr.ip);
    addr.port = SHELL_PORT;
    socket_bind(&listener, &addr, sizeof(addr));
    socket_listen(&listener, 5);
    socket_accept(&listener, &client, &addr, NULL);

    shell_init(&shell,
               console_get_input_channel(),
               console_get_output_channel(),
               NULL,
               NULL,
               NULL,
               NULL);
    shell_main(&shell);

    socket_close(&client);
    socket_close(&listener);

    return (0);
}

static int init()
{
    struct inet_ip_addr_t addr;
    char buf[20];

    sys_start();

    network_interface_module_init();
    socket_module_init();

#if defined(ARCH_ESP)

    /* Initialize WiFi in station mode with given SSID and
       password. */
    network_interface_wifi_station_espressif_module_init();
    network_interface_wifi_station_espressif_init(&wifi,
                                                  (uint8_t *)STRINGIFY(SSID),
                                                  (uint8_t *)STRINGIFY(PASSWORD));

#else
#    error "Only the ESP is supported in this example."
#endif

    network_interface_add(&wifi.network_interface);

    /* Start WiFi and connect to the Access Point with given SSID and
       password. */
    network_interface_start(&wifi.network_interface);

    network_interface_get_ip_address(&wifi.network_interface,
                                     &addr);

    std_printf(FSTR("Connected to Access Point (AP). Got IP %s.\r\n"),
               inet_ntoa(&addr, buf));

    return (0);
}

int main()
{
    init();

    while (1) {
        tcp_test();
        shell_test();
    }

    return (0);
}
