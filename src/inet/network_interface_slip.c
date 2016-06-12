/**
 * @file slip.c
 * @version 0.6.0
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

#include "lwip/tcpip.h"

#if !defined(ARCH_LINUX)

/** Protocol definitions. */
#define SLIP_END          0xc0
#define SLIP_ESC          0xdb
#define SLIP_ESC_END      0xdc
#define SLIP_ESC_ESC      0xdd

static err_t init()
{
    return (ERR_OK);
}

/**
 * Encapsulate given packet and write it to the output channel.
 */
static err_t output(struct netif *netif_p,
                    struct pbuf *pbuf_p,
                    ip_addr_t *ip_addr_p)
{
    int i;
    uint8_t data;
    struct network_interface_slip_t *self_p;

    self_p = container_of(netif_p,
                          struct network_interface_slip_t,
                          network_interface.netif);

    data = SLIP_END;
    chan_write(self_p->chout_p, &data, 1);

    while (pbuf_p != NULL) {
        for (i = 0; i < pbuf_p->len; i++) {
            data = ((uint8_t *)pbuf_p->payload)[i];

            switch (data) {

            case SLIP_END:
                data = SLIP_ESC;
                chan_write(self_p->chout_p, &data, 1);
                data = SLIP_ESC_END;
                break;

            case SLIP_ESC:
                data = SLIP_ESC;
                chan_write(self_p->chout_p, &data, 1);
                data = SLIP_ESC_ESC;
                break;
            }

            chan_write(self_p->chout_p, &data, 1);
        }

        pbuf_p = pbuf_p->next;
    }

    data = SLIP_END;
    chan_write(self_p->chout_p, &data, 1);

    return (ERR_OK);
}

/**
 * Handle an input data byte in normal state.
 */
static int state_normal_input(struct network_interface_slip_t *self_p,
                              uint8_t data)
{
    int res = 1;

    switch (data) {

    case SLIP_END:

        /* Input complete frames to the ip layer. */
        if (self_p->frame.size > 0) {
            tcpip_input(self_p->frame.pbuf_p,
                        &self_p->network_interface.netif);

            /* Allocate a new buffer. */
            self_p->frame.pbuf_p = pbuf_alloc(PBUF_LINK,
                                              (PBUF_POOL_BUFSIZE
                                               - PBUF_LINK_HLEN),
                                              PBUF_POOL);
            self_p->frame.buf_p = self_p->frame.pbuf_p->payload;
            self_p->frame.size = 0;
        }

        res = 0;

        break;

    case SLIP_ESC:
        self_p->state = NETWORK_INTERFACE_SLIP_STATE_ESCAPE;
        res = 0;
        break;

    default:
        break;
    }

    return (res);
}

/**
 * Handle an input data byte in escaped state.
 */
static int state_escaped_input(struct network_interface_slip_t *self_p,
                               uint8_t data)
{
    int res = 1;

    switch (data) {

    case SLIP_ESC_END:
        data = SLIP_END;
        break;

    case SLIP_ESC_ESC:
        data = SLIP_ESC;
        break;

    default:
        /* Protocol error. */
        res = -1;
    }

    self_p->state = NETWORK_INTERFACE_SLIP_STATE_NORMAL;

    return (res);
}

int network_interface_slip_module_init(void)
{
    return (0);
}

int network_interface_slip_init(struct network_interface_slip_t *self_p,
                                ip_addr_t *ipaddr_p,
                                ip_addr_t *netmask_p,
                                ip_addr_t *gw_p,
                                chan_t *chout_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(ipaddr_p != NULL, EINVAL);
    ASSERTN(netmask_p != NULL, EINVAL);
    ASSERTN(gw_p != NULL, EINVAL);
    ASSERTN(chout_p != NULL, EINVAL);

    self_p->state = NETWORK_INTERFACE_SLIP_STATE_NORMAL;
    self_p->chout_p = chout_p;
    self_p->frame.pbuf_p = pbuf_alloc(PBUF_LINK,
                                      (PBUF_POOL_BUFSIZE
                                       - PBUF_LINK_HLEN),
                                      PBUF_POOL);
    self_p->frame.buf_p = self_p->frame.pbuf_p->payload;
    self_p->frame.size = 0;

    self_p->network_interface.ipaddr = *ipaddr_p;
    self_p->network_interface.netmask = *netmask_p;
    self_p->network_interface.gw = *gw_p;
    self_p->network_interface.init = init;

    self_p->network_interface.netif.name[0] = 's';
    self_p->network_interface.netif.name[1] = 'l';
    self_p->network_interface.netif.output = output;
    self_p->network_interface.netif.mtu =
        NETWORK_INTERFACE_SLIP_FRAME_SIZE_MAX;
    self_p->network_interface.netif.flags |= NETIF_FLAG_POINTTOPOINT;

    return (0);
}

int network_interface_slip_input(struct network_interface_slip_t *self_p,
                                 uint8_t data)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res = -1;

    switch (self_p->state) {

    case NETWORK_INTERFACE_SLIP_STATE_NORMAL:
        res = state_normal_input(self_p, data);
        break;

    case NETWORK_INTERFACE_SLIP_STATE_ESCAPE:
        res = state_escaped_input(self_p, data);
        break;
    }

    if (res <= 0) {
        return (res);
    }

    /* Truncate long packets. */
    if (self_p->frame.size == NETWORK_INTERFACE_SLIP_FRAME_SIZE_MAX) {
        return (-1);
    }

    /* Write the data to the frame buffer. */
    self_p->frame.buf_p[self_p->frame.size] = data;
    self_p->frame.size++;

    return (0);
}

#else

int network_interface_slip_module_init(void)
{
    return (0);
}

int network_interface_slip_init(struct network_interface_slip_t *self_p,
                                ip_addr_t *ipaddr_p,
                                ip_addr_t *netmask_p,
                                ip_addr_t *gw_p,
                                chan_t *chout_p)
{
    return (0);
}

int network_interface_slip_input(struct network_interface_slip_t *self_p,
                                 uint8_t data)
{
    return (0);
}

#endif
