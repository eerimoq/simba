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

#if defined(ARCH_ESP) || defined(ARCH_ESP32)

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

    LOG_OBJECT_PRINT(NULL,
                     LOG_INFO,
                     OSTR("Connecting to WiFi with SSID '%s'.\r\n"),
                     ssid);

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

        LOG_OBJECT_PRINT(NULL,
                         LOG_INFO,
                         OSTR("Waiting for a connection to WiFi with "
                              "SSID '%s'.\r\n"),
                         ssid);
        thrd_sleep(1);
    }

    if (i == CONFIG_START_NETWORK_INTERFACE_WIFI_CONNECT_TIMEOUT) {
        LOG_OBJECT_PRINT(NULL,
                         LOG_INFO,
                         OSTR("Timeout connecting to WiFi with SSID '%s'.\r\n"),
                         ssid);

        return (-1);
    }

    /* Get the IP address of the interface*/
    if (network_interface_get_ip_info(&wifi.network_interface, &info) != 0) {
        LOG_OBJECT_PRINT(NULL,
                         LOG_INFO,
                         OSTR("No IP address for WiFi with SSID '%s'.\r\n"),
                         ssid);

        return (-1);
    }

    LOG_OBJECT_PRINT(NULL,
                     LOG_INFO,
                     OSTR("Connected to WiFi with SSID '%s'. Got IP address "
                          "'%s'.\r\n"),
                     ssid,
                     inet_ntoa(&info.address, buf));

    return (0);
}

#endif
