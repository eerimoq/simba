/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
 * host and wait for the echo reply packet. No extra payload data is
 * transmitted, only the ICMP header.
 *
 * @param[in] address_p IP address of the host to ping.
 * @param[in] timeout_p Number of seconds to wait for the echo reply
 *                      packet.
 * @param[out] round_trip_time_p The time it took from sending the
 *                               echo request packet to receiving the
 *                               echo reply packet. Only valid if this
 *                               functions returns zero(0).
 *
 * @return zero(0) or negative error code.
 */
int ping_host_by_ip_address(struct inet_ip_addr_t *address_p,
                            struct time_t *timeout_p,
                            struct time_t *round_trip_time_p);

#endif
