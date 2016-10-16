/**
 * @file network_interface.c
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

struct module_t {
    int initialized;
    /* A linked list of all network interfaces. */
    struct network_interface_t *network_interfaces_p;
#if CONFIG_FS_CMD_NETWORK_INTERFACE_LIST == 1
    struct fs_command_t cmd_list;
#endif
};

static struct module_t module;

#if CONFIG_FS_CMD_NETWORK_INTERFACE_LIST == 1

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct network_interface_t *network_interface_p;
    struct inet_if_ip_info_t info;
    char buf[16];

    /* Print the title. */
    std_fprintf(out_p, FSTR("NAME            "
                            "STATE  "
                            "ADDRESS          "
                            "  TX BYTES  "
                            "  RX BYTES\r\n"));

    /* Print a list of all network interfaces. */
    network_interface_p = module.network_interfaces_p;

    while (network_interface_p != NULL) {
        network_interface_get_ip_info(network_interface_p, &info);
        
        std_fprintf(out_p,
                    FSTR("%-14s  %-5s  %-15s  %10s  %10s\r\n"),
                    network_interface_p->name_p,
                    (network_interface_p->is_up(network_interface_p)
                     ? "up"
                     : "down"),
                    inet_ntoa(&info.address, buf),
                    "-",
                    "-");

        network_interface_p = network_interface_p->next_p;
    }

    return (0);
}

#endif

int network_interface_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_NETWORK_INTERFACE_LIST == 1

    fs_command_init(&module.cmd_list,
                    FSTR("/inet/network_interface/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

    return (0);
}

#if defined(ARCH_ESP)

int network_interface_add(struct network_interface_t *netif_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);

    netif_p->next_p = module.network_interfaces_p;
    module.network_interfaces_p = netif_p;

    return (0);
}

int network_interface_start(struct network_interface_t *netif_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);

    return (netif_p->start(netif_p));
}

#elif !defined(ARCH_LINUX)

int network_interface_add(struct network_interface_t *netif_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);

    ip_addr_t ipaddr, netmask, gw;

    ipaddr.addr = netif_p->info.address.number;
    netmask.addr = netif_p->info.netmask.number;
    gw.addr = netif_p->info.gateway.number;

    netif_add(&netif_p->netif,
              &ipaddr,
              &netmask,
              &gw,
              NULL,
              netif_p->init,
              NULL);

    netif_p->next_p = module.network_interfaces_p;
    module.network_interfaces_p = netif_p;

    return (0);
}

int network_interface_start(struct network_interface_t *netif_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);

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

int network_interface_add(struct network_interface_t *netif_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);

    netif_p->next_p = module.network_interfaces_p;
    module.network_interfaces_p = netif_p;

    return (0);
}

int network_interface_start(struct network_interface_t *netif_p)
{
    return (0);
}

#endif

int network_interface_is_up(struct network_interface_t *netif_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);
    
    return (netif_p->is_up(netif_p));
}

struct network_interface_t *network_interface_get_by_name(const char *name_p)
{
    ASSERTN(name_p != NULL, -EINVAL);
    
    struct network_interface_t *netif_p;

    netif_p = module.network_interfaces_p;

    while (netif_p != NULL) {
        if (strcmp(name_p, netif_p->name_p) == 0) {
            return (netif_p);
        }

        netif_p = netif_p->next_p;
    }

    return (NULL);
}

int network_interface_set_ip_info(struct network_interface_t *netif_p,
                                  const struct inet_if_ip_info_t *info_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);
    ASSERTN(info_p != NULL, -EINVAL);

    if (netif_p->set_ip_info(netif_p, info_p) != 0) {
        return (-1);
    }

    netif_p->info = *info_p;

    return (0);
}

int network_interface_get_ip_info(struct network_interface_t *netif_p,
                                  struct inet_if_ip_info_t *info_p)
{
    ASSERTN(netif_p != NULL, -EINVAL);
    ASSERTN(info_p != NULL, -EINVAL);
    
    if (netif_p->get_ip_info(netif_p, &netif_p->info) != 0) {
        return (-1);
    }

    *info_p = netif_p->info;

    return (0);
}
