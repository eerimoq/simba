/**
 * @file network_interface_wifi_espressif.c
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#if defined(ARCH_ESP)

#include "esp_wifi.h"
#include "esp_sta.h"

static struct station_config station_config;
static struct ip_info ip_config;

static err_t init()
{
    return (ERR_OK);
}

static int start(struct network_interface_t *netif_p)
{
    struct network_interface_wifi_station_espressif_t *self_p;

    self_p = container_of(netif_p,
                          struct network_interface_wifi_station_espressif_t,
                          network_interface);

    return (network_interface_wifi_station_espressif_start(self_p));
}

static int stop(struct network_interface_t *netif_p)
{
    return (0);
}

static int is_up(struct network_interface_t *netif_p)
{
    return (wifi_station_get_connect_status() == STATION_GOT_IP);
}

static int get_ip_address(struct network_interface_t *netif_p,
                          struct inet_ip_addr_t *addr_p)
{
    struct network_interface_wifi_station_espressif_t *self_p;

    self_p = container_of(netif_p,
                          struct network_interface_wifi_station_espressif_t,
                          network_interface);

    return (network_interface_wifi_station_espressif_get_ip_address(self_p, addr_p));
}

int network_interface_wifi_station_espressif_module_init(void)
{
    return (0);
}

int network_interface_wifi_station_espressif_init(struct network_interface_wifi_station_espressif_t *self_p,
                                                  const uint8_t *ssid_p,
                                                  const uint8_t *password_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(ssid_p != NULL, -EINVAL);
    ASSERTN(password_p != NULL, -EINVAL);

    self_p->network_interface.name_p = "esp-wlan";
    self_p->network_interface.init = init;
    self_p->network_interface.start = start;
    self_p->network_interface.stop = stop;
    self_p->network_interface.is_up = is_up;
    self_p->network_interface.get_ip_address = get_ip_address;

    wifi_set_opmode_current(STATION_MODE);

    memset(&station_config, 0, sizeof(station_config));
    std_sprintf((char *)station_config.ssid, FSTR("%s"), ssid_p);
    std_sprintf((char *)station_config.password,
                FSTR("%s"), password_p);

    return (0);
}

int network_interface_wifi_station_espressif_start(struct network_interface_wifi_station_espressif_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    wifi_station_set_config(&station_config);

    return (0);
}

int network_interface_wifi_station_espressif_stop(struct network_interface_wifi_station_espressif_t *self_p)
{
    return (-1);
}

int network_interface_wifi_station_espressif_is_up(struct network_interface_wifi_station_espressif_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    return (is_up(&self_p->network_interface));
}

int network_interface_wifi_station_espressif_get_ip_address(struct network_interface_wifi_station_espressif_t *self_p,
                                                            struct inet_ip_addr_t *addr_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(addr_p != NULL, -EINVAL);

    wifi_get_ip_info(STATION_IF, &ip_config);

    addr_p->number = ip_config.ip.addr;

    return (ip_config.ip.addr != 0 ? 0 : -1);
}

#endif
