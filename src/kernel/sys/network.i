/**
 * @file sys/network.i
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

#if defined(ARCH_ESP)

static struct network_interface_wifi_t wifi;

/**
 * Start the ip stack.
 */
static int start_network(void)
{
    struct inet_if_ip_info_t info;
    char buf[20];
    const char ssid[] = STRINGIFY(CONFIG_START_NETWORK_INTERFACE_WIFI_SSID);
    const char password[] = STRINGIFY(CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD);
    int i;

    esp_wifi_module_init();
    inet_module_init();
    socket_module_init();
    network_interface_module_init();
    ping_module_init();

    std_printf(FSTR("Connecting to WiFi with SSID '%s'.\r\n"), ssid);

    /* Initialize WiFi in station mode with given SSID and
       password. */
    network_interface_wifi_init(&wifi,
                                "esp-wlan-sta",
                                &network_interface_wifi_driver_esp_station,
                                NULL,
                                &ssid[0],
                                &password[0]);
    network_interface_add(&wifi.network_interface);

    /* Start WiFi and connect to the Access Point with given SSID and
       password and wait for a connection. */
    network_interface_start(&wifi.network_interface);

    /* Wait for a connection, or timeout.*/
    for (i = 0; i < CONFIG_START_NETWORK_INTERFACE_WIFI_CONNECT_TIMEOUT; i++) {
        if (network_interface_is_up(&wifi.network_interface) == 1) {
            break;
        }

        std_printf(FSTR("Waiting for a connection to WiFi with SSID '%s'.\r\n"),
                   ssid);
        thrd_sleep(1);
    }

    if (i == CONFIG_START_NETWORK_INTERFACE_WIFI_CONNECT_TIMEOUT) {
        std_printf(FSTR("Timeout connecting to WiFi with SSID '%s'.\r\n"),
                   ssid);

        return (-1);
    }

    /* Get the IP address of the interface*/
    if (network_interface_get_ip_info(&wifi.network_interface, &info) != 0) {
        std_printf(FSTR("No IP address for WiFi with SSID '%s'.\r\n"), ssid);

        return (-1);
    }

    std_printf(FSTR("Connected to WiFi with SSID '%s'. Got IP address '%s'.\r\n"),
               ssid,
               inet_ntoa(&info.address, buf));

    return (0);
}

#endif
