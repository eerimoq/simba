/**
 * @file inet/network_interface.h
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

#ifndef __INET_NETWORK_INTERFACE_H__
#define __INET_NETWORK_INTERFACE_H__

#include "simba.h"

#include "lwip/sys.h"
#include "lwip/ip.h"

struct network_interface_t;

typedef int (*network_interface_start_t)(struct network_interface_t *netif_p);
typedef int (*network_interface_stop_t)(struct network_interface_t *netif_p);
typedef int (*network_interface_is_up_t)(struct network_interface_t *netif_p);
typedef int (*network_interface_set_ip_info_t)(struct network_interface_t *netif_p,
                                               const struct inet_if_ip_info_t *info_p);
typedef int (*network_interface_get_ip_info_t)(struct network_interface_t *netif_p,
                                               struct inet_if_ip_info_t *info_p);

struct network_interface_t {
    struct netif netif;
    const char *name_p;
    struct inet_if_ip_info_t info;
    netif_init_fn init;
    network_interface_start_t start;
    network_interface_stop_t stop;
    network_interface_is_up_t is_up;
    network_interface_set_ip_info_t set_ip_info;
    network_interface_get_ip_info_t get_ip_info;
    struct network_interface_t *next_p;
};

/**
 * Initialize the network interface module. This function must be
 * called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_module_init(void);

/**
 * Add given network interface to the IP stack.
 *
 * @param[in] netif_p Network interface to add.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_add(struct network_interface_t *netif_p);

/**
 * Enable given network interface. Use `network_interface_is_up()` to
 * check if the interface is connected.
 *
 * @param[in] netif_p Network interface to enable.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_start(struct network_interface_t *netif_p);

/**
 * Get the connection status of given network interface.
 *
 * @param[in] netif_p Network interface to get the connection status
 *                    of.
 *
 * @return true(1) if the network interface is up, false(0) is it is
 *         down, and otherwise negative error code.
 */
int network_interface_is_up(struct network_interface_t *netif_p);

/**
 * Search the list of network interfaces for an interface with given
 * name and return it.
 *
 * @param[in] name_p Name of the network interface to find.
 *
 * @return Found network interface or NULL if it was not found.
 */
struct network_interface_t *network_interface_get_by_name(const char *name_p);

/**
 * Get the ip address of given network interface.
 *
 * @param[in] netif_p Network interface to get the ip address of.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_set_ip_info(struct network_interface_t *netif_p,
                                  const struct inet_if_ip_info_t *info_p);

/**
 * Get the ip address of given network interface.
 *
 * @param[in] netif_p Network interface to get the ip address of.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_get_ip_info(struct network_interface_t *netif_p,
                                  struct inet_if_ip_info_t *info_p);

#endif
