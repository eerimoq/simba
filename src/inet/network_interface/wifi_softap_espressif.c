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

#include "esp_misc.h"
#include "esp_wifi.h"
#include "esp_softap.h"

static struct softap_config softap_config;
static struct ip_info ip_config;

static err_t init()
{
    return (ERR_OK);
}

static int start(struct network_interface_t *netif_p)
{
    struct network_interface_wifi_softap_espressif_t *self_p;

    self_p = container_of(netif_p,
                          struct network_interface_wifi_softap_espressif_t,
                          network_interface);

    return (network_interface_wifi_softap_espressif_start(self_p));
}

static int stop(struct network_interface_t *netif_p)
{
    struct network_interface_wifi_softap_espressif_t *self_p;

    self_p = container_of(netif_p,
                          struct network_interface_wifi_softap_espressif_t,
                          network_interface);

    return (network_interface_wifi_softap_espressif_stop(self_p));
}

static int is_up(struct network_interface_t *netif_p)
{
    return (1);
}

static int get_ip_address(struct network_interface_t *netif_p,
                          struct inet_ip_addr_t *addr_p)
{
    struct network_interface_wifi_softap_espressif_t *self_p;

    self_p = container_of(netif_p,
                          struct network_interface_wifi_softap_espressif_t,
                          network_interface);

    return (network_interface_wifi_softap_espressif_get_ip_address(self_p, addr_p));
}

int network_interface_wifi_softap_espressif_module_init(void)
{
    return (0);
}

int network_interface_wifi_softap_espressif_init(struct network_interface_wifi_softap_espressif_t *self_p,
                                                  const uint8_t *ssid_p,
                                                  const uint8_t *password_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(ssid_p != NULL, -EINVAL);
    ASSERTN(password_p != NULL, -EINVAL);

    self_p->network_interface.name_p = "esp-softap";
    self_p->network_interface.init = init;
    self_p->network_interface.start = start;
    self_p->network_interface.stop = stop;
    self_p->network_interface.is_up = is_up;
    self_p->network_interface.get_ip_address = get_ip_address;

    memset(&softap_config, 0, sizeof(softap_config));
    wifi_softap_get_config(&softap_config);
    softap_config.authmode = AUTH_WPA2_PSK;
    softap_config.ssid_len = 0;
    softap_config.max_connection = 4;
    std_sprintf((char *)softap_config.ssid, FSTR("%s"), ssid_p);
    std_sprintf((char *)softap_config.password,
                FSTR("%s"), password_p);

    return (0);
}

int network_interface_wifi_softap_espressif_start(struct network_interface_wifi_softap_espressif_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    wifi_set_opmode_current(SOFTAP_MODE | wifi_get_opmode());
    wifi_softap_set_config(&softap_config);
    wifi_softap_dhcps_start();

    return (0);
}

int network_interface_wifi_softap_espressif_stop(struct network_interface_wifi_softap_espressif_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    wifi_softap_dhcps_stop();
    wifi_set_opmode_current(~SOFTAP_MODE & wifi_get_opmode());

    return (0);
}

int network_interface_wifi_softap_espressif_is_up(struct network_interface_wifi_softap_espressif_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    return (is_up(&self_p->network_interface));
}

int network_interface_wifi_softap_espressif_get_ip_address(struct network_interface_wifi_softap_espressif_t *self_p,
                                                            struct inet_ip_addr_t *addr_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(addr_p != NULL, -EINVAL);

    wifi_get_ip_info(SOFTAP_IF, &ip_config);

    addr_p->number = ip_config.ip.addr;

    return (ip_config.ip.addr != 0 ? 0 : -1);
}
