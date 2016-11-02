/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
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
    void *chout_p;
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
                                void *chout_p);

#endif
