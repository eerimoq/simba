/**
 * @file inet/network_interface/slip.h
 * @version 5.0.0
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

#ifndef __INET_NETWORK_INTERFACE_SLIP_H__
#define __INET_NETWORK_INTERFACE_SLIP_H__

#include "simba.h"

#define NETWORK_INTERFACE_SLIP_FRAME_SIZE_MAX 1500

enum network_interface_slip_state_t {
    NETWORK_INTERFACE_SLIP_STATE_NORMAL = 0,
    NETWORK_INTERFACE_SLIP_STATE_ESCAPE
};

struct network_interface_slip_t {
    enum network_interface_slip_state_t state;
    struct {
        struct pbuf *pbuf_p;
        uint8_t *buf_p;
        size_t size;
    } frame;
    chan_t *chout_p;
    struct network_interface_t network_interface;
};

/**
 * Initialize the slip module.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_slip_module_init(void);

/**
 * Initialize given slip network interface with given configuration
 * and output channel.
 *
 * @param[in] self_p Slip to initialize.
 * @param[in] ipaddr_p Network interface IP address.
 * @param[in] netmask_p Network interface netmask.
 * @param[in] gateway_p Network interface gateway.
 * @param[in] chout_p Output channel.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_slip_init(struct network_interface_slip_t *self_p,
                                struct inet_ip_addr_t *ipaddr_p,
                                struct inet_ip_addr_t *netmask_p,
                                struct inet_ip_addr_t *gateway_p,
                                chan_t *chout_p);

/**
 * Input a byte into the SLIP IP stack. Normally a user thread reads
 * one byte at a time from the UART and calls this functions with the
 * read byte as argument.
 *
 * @param[in] self_p Slip to initialize.
 * @param[in] data Byte to input into the stack.
 *
 * @return Number of bytes written to the input frame or negative
 *         error code.
 */
int network_interface_slip_input(struct network_interface_slip_t *self_p,
                                 uint8_t data);

#endif
