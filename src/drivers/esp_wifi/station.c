/**
 * @file esp_wifi/station.c
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
#include "espressif/esp_sta.h"

static struct station_config config;

int esp_wifi_station_init(const char *ssid_p,
                          const char *password_p,
                          struct inet_if_ip_info_t *info_p)
{
    ASSERTN(ssid_p != NULL, -EINVAL);

    if ((strlen(ssid_p) == 0)
        || (strlen(ssid_p) >= sizeof(config.ssid))) {
        return (-1);
    }

    strcpy((char *)config.ssid, ssid_p);

    if (password_p != NULL) {
        strcpy((char *)config.password, password_p);
    } else {
        config.password[0] = 0;
    }

    /* Static or dynamic ip. */
    if (info_p != NULL) {
        esp_wifi_station_set_ip_info(info_p);
    } else {
        esp_wifi_station_dhcp_client_start();
    }

    return (!wifi_station_set_config_current(&config));
}

int esp_wifi_station_connect(void)
{
    return (!wifi_station_connect());
}

int esp_wifi_station_disconnect(void)
{
    return (!wifi_station_disconnect());
}

int esp_wifi_station_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, -EINVAL);

    struct ip_info info;

    info.ip.addr = info_p->address.number;
    info.netmask.addr = info_p->netmask.number;
    info.gw.addr = info_p->gateway.number;

    esp_wifi_station_dhcp_client_stop();

    if (wifi_set_ip_info(STATION_IF, &info) != 1) {
        return (-1);
    }

    return (0);
}

int esp_wifi_station_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, -EINVAL);

    struct ip_info info;

    if (wifi_get_ip_info(STATION_IF, &info) != 1) {
        return (-1);
    }

    info_p->address.number = info.ip.addr;
    info_p->netmask.number = info.netmask.addr;
    info_p->gateway.number = info.gw.addr;

    return (0);
}

int esp_wifi_station_set_reconnect_policy(int policy)
{
    return (!wifi_station_set_reconnect_policy(policy));
}

int esp_wifi_station_get_reconnect_policy(void)
{
    return (wifi_station_get_reconnect_policy());
}

enum esp_wifi_station_status_t esp_wifi_station_get_connect_status(void)
{
    return (wifi_station_get_connect_status());
}

int esp_wifi_station_dhcp_client_start(void)
{
    return (!wifi_station_dhcpc_start());
}

int esp_wifi_station_dhcp_client_stop(void)
{
    return (!wifi_station_dhcpc_stop());
}

enum esp_wifi_dhcp_status_t esp_wifi_station_dhcp_client_status(void)
{
    return (wifi_station_dhcpc_status());
}
