/**
 * @file network_interface_wifi_espressif.c
 * @version 6.0.0
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

int network_interface_wifi_station_espressif_module_init(void)
{
    return (0);
}

int network_interface_wifi_station_espressif_init(struct network_interface_wifi_station_espressif_t *self_p,
                                                  const uint8_t *ssid_p,
                                                  const uint8_t *password_p)
{
    self_p->network_interface.name_p = "esp-wlan";
    self_p->network_interface.init = init;
    self_p->network_interface.start = start;
    self_p->network_interface.stop = stop;
    self_p->network_interface.is_up = is_up;

    wifi_set_opmode_current(STATION_MODE);

    memset(&self_p->station_config, 0, sizeof(self_p->station_config));
    std_sprintf((char *)self_p->station_config.ssid, FSTR("%s"), ssid_p);
    std_sprintf((char *)self_p->station_config.password,
                FSTR("%s"), password_p);
    
    return (0);
}

int network_interface_wifi_station_espressif_start(struct network_interface_wifi_station_espressif_t *self_p)
{
    struct inet_ip_addr_t addr;

    wifi_station_set_config(&self_p->station_config);

    /* Wait for a connection. */
    do {
        wifi_get_ip_info(STATION_IF, &self_p->ip_config);
    } while (self_p->ip_config.ip.addr == 0);

    addr.number = self_p->ip_config.ip.addr;
    network_interface_set_ip_address(&self_p->network_interface,
                                     &addr);

    return (0);
}

int network_interface_wifi_station_espressif_stop(struct network_interface_wifi_station_espressif_t *self_p)
{
    return (-1);
}

#endif
