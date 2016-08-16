/**
 * @file inet/ping.h
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
 * MERBITSTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __INET_PING_H__
#define __INET_PING_H__

#include "simba.h"

/**
 * Initialize the ping module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int ping_module_init(void);

/**
 * Ping host by given ip address. Send an echo request packet to given
 * host and wait for the echo reply packet.
 *
 * @param[in] address_p IP address of the host to ping.
 * @param[in] timeout_p Number of seconds to wait for the echo reply
 *                      packet.
 * @param[out] rtt_p Ping round trip time. The time it took from
 *                   sending the echo request packet to receiving the
 *                   echo reply packet.
 *
 * @return zero(0) or negative error code.
 */
int ping_host_by_ip_address(struct inet_ip_addr_t *address_p,
                            struct time_t *timeout_p,
                            struct time_t *rtt_p);

#endif
