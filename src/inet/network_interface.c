/**
 * @file network_interface.c
 * @version 3.1.1
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

int network_interface_module_init(void)
{
    return (0);
}

int network_interface_add(struct network_interface_t *netif_p)
{
    return (0);
}

int network_interface_start(struct network_interface_t *netif_p)
{
    return (netif_p->start(netif_p));
}

#elif !defined(ARCH_LINUX)

int network_interface_module_init(void)
{
    return (0);
}

int network_interface_add(struct network_interface_t *netif_p)
{
    ip_addr_t ipaddr, netmask, gw;

    ipaddr.addr = netif_p->ipaddr.number;
    netmask.addr = netif_p->netmask.number;
    gw.addr = netif_p->gw.number;

    netif_add(&netif_p->netif,
              &ipaddr,
              &netmask,
              &gw,
              NULL,
              netif_p->init,
              NULL);

    return (0);
}

int network_interface_start(struct network_interface_t *netif_p)
{
    int res = 0;

    if (netif_p->start != NULL) {
        res = netif_p->start(netif_p);
    }

    if (res == 0) {
        netif_set_up(&netif_p->netif);
    }

    return (res);
}

#else

int network_interface_module_init(void)
{
    return (0);
}

int network_interface_add(struct network_interface_t *netif_p)
{
    return (0);
}

int network_interface_start(struct network_interface_t *netif_p)
{
    return (0);
}

#endif

int network_interface_set_ip_address(struct network_interface_t *netif_p,
                                     struct inet_ip_addr_t *addr_p)
{
    netif_p->ipaddr = *addr_p;

    return (0);
}

int network_interface_get_ip_address(struct network_interface_t *netif_p,
                                     struct inet_ip_addr_t *addr_p)
{
    *addr_p = netif_p->ipaddr;

    return (0);
}
