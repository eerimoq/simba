/**
 * @file network_interface_wifi.c
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

static err_t init(struct netif *netif)
{
    return (ERR_OK);
}

int network_interface_wifi_module_init(void)
{
    return (0);
}

int network_interface_wifi_init(struct network_interface_wifi_t *self_p,
                                const char *name_p,
                                struct network_interface_wifi_driver_t *driver_p,
                                void *arg_p,
                                const char *ssid_p,
                                const char *password_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(name_p != NULL, -EINVAL);
    ASSERTN(driver_p != NULL, -EINVAL);
    ASSERTN(ssid_p != NULL, -EINVAL);

    self_p->network_interface.name_p = name_p;
    self_p->network_interface.init = init;
    self_p->network_interface.start =
        (network_interface_start_t)network_interface_wifi_start;
    self_p->network_interface.stop =
        (network_interface_stop_t)network_interface_wifi_stop;
    self_p->network_interface.is_up =
        (network_interface_is_up_t)network_interface_wifi_is_up;
    self_p->network_interface.set_ip_info =
        (network_interface_set_ip_info_t)network_interface_wifi_set_ip_info;
    self_p->network_interface.get_ip_info =
        (network_interface_get_ip_info_t)network_interface_wifi_get_ip_info;

    self_p->driver_p = driver_p;

    return (0);
}

int network_interface_wifi_start(struct network_interface_wifi_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    return (self_p->driver_p->start(self_p->arg_p));
}

int network_interface_wifi_stop(struct network_interface_wifi_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    return (self_p->driver_p->stop(self_p->arg_p));
}

int network_interface_wifi_is_up(struct network_interface_wifi_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    return (self_p->driver_p->is_up(self_p->arg_p));
}

int network_interface_wifi_set_ip_info(struct network_interface_wifi_t *self_p,
                                       const struct inet_if_ip_info_t *info_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(info_p != NULL, -EINVAL);

    return (self_p->driver_p->set_ip_info(self_p->arg_p, info_p));
}

int network_interface_wifi_get_ip_info(struct network_interface_wifi_t *self_p,
                                       struct inet_if_ip_info_t *info_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(info_p != NULL, -EINVAL);

    return (self_p->driver_p->get_ip_info(self_p->arg_p, info_p));
}
