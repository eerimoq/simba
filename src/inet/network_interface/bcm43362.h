/**
 * @file inet/network_interface/bcm43362.h
 * @version 7.0.0
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

#ifndef __INET_NETWORK_INTERFACE_BCM43362_H__
#define __INET_NETWORK_INTERFACE_BCM43362_H__

#include "simba.h"

#define NETWORK_INTERFACE_BCM43362_FRAME_SIZE_MAX 1500

enum network_interface_bcm43362_state_t {
    NETWORK_INTERFACE_BCM43362_STATE_NORMAL = 0,
    NETWORK_INTERFACE_BCM43362_STATE_ESCAPE
};

struct network_interface_bcm43362_t {
    enum network_interface_bcm43362_state_t state;
    struct {
        struct pbuf *pbuf_p;
        uint8_t *buf_p;
        size_t size;
    } frame;
    chan_t *chout_p;
    struct network_interface_t network_interface;
};

/**
 * Initialize the bcm43362 module.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_bcm43362_module_init(void);

/**
 * Initialize given bcm43362 network interface with given configuration
 * and output channel.
 *
 * @param[in] self_p Bcm43362 to initialize.
 * @param[in] ipaddr_p Network interface IP address.
 * @param[in] netmask_p Network interface netmask.
 * @param[in] gw_p Network interface gateway.
 * @param[in] chout_p Output channel.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_bcm43362_init(struct network_interface_bcm43362_t *self_p,
                                struct inet_ip_addr_t *ipaddr_p,
                                struct inet_ip_addr_t *netmask_p,
                                struct inet_ip_addr_t *gw_p,
                                chan_t *chout_p);

#endif
