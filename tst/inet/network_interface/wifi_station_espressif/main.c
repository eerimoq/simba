/**
 * @file main.c
 * @version 4.0.0
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

#if !defined(SSID)
#    pragma message "WiFi connection variable SSID is not set. Using default value MySSID"
#    define SSID MySSID
#endif

#if !defined(PASSWORD)
#    pragma message "WiFi connection variable PASSWORD is not set. Using default value MyPassword"
#    define PASSWORD MyPassword
#endif

#if !defined(ESP8266_IP)
#    pragma message "WiFi connection variable ESP8266_IP is not set. Using default value 192.168.1.103"
#    define ESP8266_IP 192.168.1.103
#endif

/* Ports. */
#define UDP_PORT   30303
#define TCP_PORT   40404

static struct network_interface_wifi_station_espressif_t wifi;

static int test_init(struct harness_t *harness_p)
{
    struct inet_ip_addr_t addr;
    char buf[20];

    network_interface_module_init();
    socket_module_init();

    std_printf(FSTR("Connectiong to SSID=%s\r\n"), STRINGIFY(SSID));

    /* Initialize WiFi in station mode with given SSID and
       password. */
    network_interface_wifi_station_espressif_module_init();
    network_interface_wifi_station_espressif_init(&wifi,
                                                  (uint8_t *)STRINGIFY(SSID),
                                                  (uint8_t *)STRINGIFY(PASSWORD));

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

static int test_udp(struct harness_t *harness_p)
{
    struct socket_t sock;
    struct inet_addr_t addr;
    char buf[16];
    char addrbuf[20];
    size_t size;

    std_printf(FSTR("UDP test\r\n"));

    std_printf(FSTR("opening socket\r\n"));
    socket_open(&sock, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_DGRAM, 0);

    std_printf(FSTR("binding to %d\r\n"), UDP_PORT);
    inet_aton(STRINGIFY(ESP8266_IP), &addr.ip);
    addr.port = UDP_PORT;
    socket_bind(&sock, &addr, sizeof(addr));

    std_printf(FSTR("recvfrom\r\n"));

    size = socket_recvfrom(&sock,
                           buf,
                           sizeof(buf),
                           0,
                           &addr,
                           sizeof(addr));
    BTASSERT(size == 9);
    buf[size] = '\0';
    std_printf(FSTR("received '%s' from %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    std_printf(FSTR("sending '%s' to %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);
    socket_sendto(&sock,
                  buf,
                  size,
                  0,
                  &addr,
                  sizeof(addr));

    std_printf(FSTR("closing socket\r\n"));
    socket_close(&sock);

    return (0);
}

static int test_tcp(struct harness_t *harness_p)
{
    struct socket_t listener;
    struct socket_t client;
    struct inet_addr_t addr;
    char buf[16];
    char addrbuf[20];
    size_t size;

    std_printf(FSTR("TCP test\r\n"));

    std_printf(FSTR("opening listener socket\r\n"));
    socket_open(&listener, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_STREAM, 0);

    std_printf(FSTR("binding to %d\r\n"), TCP_PORT);
    inet_aton(STRINGIFY(ESP8266_IP), &addr.ip);
    addr.port = TCP_PORT;
    socket_bind(&listener, &addr, sizeof(addr));

    socket_listen(&listener, 5);

    std_printf(FSTR("listening on %d\r\n"), TCP_PORT);

    socket_accept(&listener, &client, &addr, NULL);
    std_printf(FSTR("accepted client %s:%d\r\n"),
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    size = socket_read(&client, buf, 5);
    size += socket_read(&client, &buf[5], 4);
    BTASSERT(size == 9);
    buf[size] = '\0';
    std_printf(FSTR("read '%s'\r\n"), buf);

    std_printf(FSTR("writing '%s'\r\n"), buf);
    socket_write(&client, buf, size);

    std_printf(FSTR("closing client socket\r\n"));
    socket_close(&client);

    std_printf(FSTR("closing listener socket\r\n"));
    socket_close(&listener);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_udp, "test_udp" },
        { test_tcp, "test_tcp" },
        { NULL, NULL }
    };

    sys_start();
    inet_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
