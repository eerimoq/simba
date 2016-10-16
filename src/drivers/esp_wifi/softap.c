/**
 * @file esp_wifi/softap.c
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

#include "espressif/esp_wifi.h"
#include "espressif/esp_misc.h"
#include "espressif/esp_softap.h"

static struct softap_config config;

int esp_wifi_softap_init(const char *ssid_p,
                         const char *password_p)
{
    ASSERTN(ssid_p != NULL, -EINVAL);

    if ((strlen(ssid_p) == 0)
        || (strlen(ssid_p) >= sizeof(config.ssid))) {
        return (-1);
    }

    strcpy((char *)config.ssid, ssid_p);
    config.ssid_len = strlen(ssid_p);
    config.channel = 1;
    config.ssid_hidden = 0;
    config.max_connection = 4;
    config.beacon_interval = 100;

    if (password_p == NULL) {
        config.authmode = AUTH_OPEN;
    } else {
        strcpy((char *)config.password, password_p);
        config.authmode = AUTH_WPA2_PSK;
    }

    return (!wifi_softap_set_config_current(&config));
}

int esp_wifi_softap_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    return (-1);
}

int esp_wifi_softap_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, -EINVAL);

    struct ip_info info;

    if (wifi_get_ip_info(SOFTAP_IF, &info) != 1) {
        return (-1);
    }

    info_p->address.number = info.ip.addr;
    info_p->netmask.number = info.netmask.addr;
    info_p->gateway.number = info.gw.addr;

    return (0);
}

int esp_wifi_softap_get_number_of_connected_stations()
{
    return (wifi_softap_get_station_num());
}

int esp_wifi_softap_get_station_info(struct esp_wifi_softap_station_info_t *info_p,
                                     int length)
{
    int i;
    struct station_info *elem_p;

    elem_p = wifi_softap_get_station_info();
    i = 0;

    while (elem_p != NULL) {
        memcpy(&info_p[i].bssid[0], &elem_p->bssid[0], sizeof(info_p->bssid));
        info_p[i].ip_address.number = elem_p->ip.addr;
        i++;

        if (i == length) {
            break;
        }

        elem_p = STAILQ_NEXT(elem_p, next);
    }

    wifi_softap_free_station_info();

    return (i);
}

int esp_wifi_softap_dhcp_server_start()
{
    return (!wifi_softap_dhcps_start());
}

int esp_wifi_softap_dhcp_server_stop()
{
    return (!wifi_softap_dhcps_stop());
}

enum esp_wifi_dhcp_status_t esp_wifi_softap_dhcp_server_status()
{
    return (wifi_softap_dhcps_status());
}
