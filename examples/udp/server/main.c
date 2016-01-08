/**
 * @file main.c
 * @version 0.2.0
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

/* UDP ports to use. */
#define UDP_LOCAL_PORT  30303
#define UDP_REMOTE_PORT 30303

static char packet[512];
static struct uart_driver_t uart;

/* WiFi configuration. */
#define ACCESS_POINT_SSID "my_ssid"
#define ACCESS_POINT_PASSWORD "my_password"

int main()
{
    ssize_t size;
    struct socket_t udp;
    struct socket_addr_t remote_addr;
    const char ssid[] = "my_ssid";
    const char password[] = "my_password";
    struct wifi_driver_t wifi;
    char buf[32];

    sys_start();
    uart_module_init();
    wifi_module_init();

    uart_init(&uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&uart);

    /* Connect to the access point with given SSID and password. */
    wifi_init(&wifi, WIFI_MODE_STATION);
    wifi_start(&wifi);

    if (wifi_station_connect(&wifi, ssid, password) != 0) {
        std_printf(FSTR("Unable to connect to the WiFi access "
                        "point with SSID '%s'. SSID or password "
                        "incorrect?\r\n"), ssid);
    }

    /* Setup a UDP socket with given ports. */
    socket_open(&udp, AF_INET, SOCKET_TYPE_DGRAM, 0);

    while (1) {
        std_printf(FSTR("Waiting for a packet from the client.\r\n"));

        /* Wait for a packet from the client. */
        size = socket_recvfrom(&udp,
                               packet,
                               sizeof(packet),
                               0,
                               &remote_addr,
                               sizeof(remote_addr));

        std_printf(FSTR("Received a packet of size %ld from client "
                        "with ip %s\r\n"),
                   (long)size,
                   inet_ntoa(buf, &remote_addr));

        /* Send the packet back to the client. */
        socket_sendto(&udp,
                      packet,
                      size,
                      0,
                      &remote_addr,
                      sizeof(remote_addr));
    }

    return (0);
}
