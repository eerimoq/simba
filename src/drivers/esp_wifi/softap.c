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

#if CONFIG_ESP_WIFI == 1

#include "esp_wifi_port/softap.i"

int esp_wifi_softap_init(const char *ssid_p,
                         const char *password_p)
{
    return (esp_wifi_softap_port_init(ssid_p, password_p));
}

int esp_wifi_softap_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_softap_port_set_ip_info(info_p));
}

int esp_wifi_softap_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_softap_port_get_ip_info(info_p));
}

int esp_wifi_softap_get_number_of_connected_stations()
{
    return (esp_wifi_softap_port_get_number_of_connected_stations());
}

int esp_wifi_softap_get_station_info(struct esp_wifi_softap_station_info_t *info_p,
                                     int length)
{
    return (esp_wifi_softap_port_get_station_info(info_p,
                                                  length));
}

int esp_wifi_softap_dhcp_server_start()
{
    return (esp_wifi_softap_port_dhcp_server_start());
}

int esp_wifi_softap_dhcp_server_stop()
{
    return (esp_wifi_softap_port_dhcp_server_stop());
}

enum esp_wifi_dhcp_status_t esp_wifi_softap_dhcp_server_status()
{
    return (esp_wifi_softap_port_dhcp_server_status());
}

#endif
