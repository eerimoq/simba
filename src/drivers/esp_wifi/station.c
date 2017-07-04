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

#include "esp_wifi_port/station.i"

int esp_wifi_station_init(const char *ssid_p,
                          const char *password_p,
                          const uint8_t *bssid_p,
                          const struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_station_port_init(ssid_p,
                                       password_p,
                                       bssid_p,
                                       info_p));
}

int esp_wifi_station_connect(void)
{
    return (esp_wifi_station_port_connect());
}

int esp_wifi_station_disconnect(void)
{
    return (esp_wifi_station_port_disconnect());
}

int esp_wifi_station_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_station_port_set_ip_info(info_p));
}

int esp_wifi_station_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_station_port_get_ip_info(info_p));
}

int esp_wifi_station_set_reconnect_policy(int policy)
{
    return (esp_wifi_station_port_set_reconnect_policy(policy));
}

int esp_wifi_station_get_reconnect_policy(void)
{
    return (esp_wifi_station_port_get_reconnect_policy());
}

enum esp_wifi_station_status_t esp_wifi_station_get_status(void)
{
    return (esp_wifi_station_port_get_status());
}

int esp_wifi_station_dhcp_client_start(void)
{
    return (esp_wifi_station_port_dhcp_client_start());
}

int esp_wifi_station_dhcp_client_stop(void)
{
    return (esp_wifi_station_port_dhcp_client_stop());
}

enum esp_wifi_dhcp_status_t esp_wifi_station_dhcp_client_status(void)
{
    return (esp_wifi_station_port_dhcp_client_status());
}

const char *esp_wifi_station_status_as_string(enum esp_wifi_station_status_t status)
{
    switch (status) {
    case esp_wifi_station_status_idle_t:
        return "idle";
    case esp_wifi_station_status_connecting_t:
        return "connecting";
    case esp_wifi_station_status_connected_t:
        return "connected";
    case esp_wifi_station_status_auth_failure_t:
        return "auth-failure";
    case esp_wifi_station_status_no_ap_found_t:
        return "no-ap-found";
    case esp_wifi_station_status_connect_fail_t:
        return "connect-fail";
    case esp_wifi_station_status_got_ip_t:
        return "got-ip";
    default:
        return "unknown";
    }
}

#endif
