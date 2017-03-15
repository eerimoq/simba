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

#include "simba.h"

#undef BIT
#undef O_RDONLY
#undef O_WRONLY
#undef O_RDWR
#undef O_APPEND
#undef O_CREAT
#undef O_TRUNC
#undef O_EXCL
#undef O_SYNC

#include "lwip/init.h"
#include "lwip/stats.h"
#include "lwip/tcpip.h"

#if !defined(ARCH_LINUX)

/** Protocol definitions. */
#define SLIP_END          0xc0
#define SLIP_ESC          0xdb
#define SLIP_ESC_END      0xdc
#define SLIP_ESC_ESC      0xdd

static int netif_allocated = 0;
static struct netif netif;

/**
 * Encapsulate given packet and write it to the output channel.
 */
static err_t output(struct netif *netif_p,
                    struct pbuf *pbuf_p,
#if LWIP_VERSION_MINOR <= 4
                    ip_addr_t *ip_addr_p
#else
                    const ip4_addr_t *ip_addr_p
#endif
                    )
{
    int i;
    uint8_t data;
    struct network_interface_slip_t *self_p;

    self_p = container_of((void *)netif_p,
                          struct network_interface_slip_t,
                          network_interface.netif_p);

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
                        self_p->network_interface.netif_p);

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
        /* Protocol error. Discard current frame. */
        self_p->frame.size = 0;
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
                                struct inet_ip_addr_t *ipaddr_p,
                                struct inet_ip_addr_t *netmask_p,
                                struct inet_ip_addr_t *gateway_p,
                                void *chout_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(ipaddr_p != NULL, EINVAL);
    ASSERTN(netmask_p != NULL, EINVAL);
    ASSERTN(gateway_p != NULL, EINVAL);
    ASSERTN(chout_p != NULL, EINVAL);

    /* Only one netif available for now. */
    if (netif_allocated != 0) {
        return (-1);
    }
    
    netif_allocated = 1;

    self_p->state = NETWORK_INTERFACE_SLIP_STATE_NORMAL;
    self_p->chout_p = chout_p;
    self_p->frame.pbuf_p = pbuf_alloc(PBUF_LINK,
                                      (PBUF_POOL_BUFSIZE
                                       - PBUF_LINK_HLEN),
                                      PBUF_POOL);
    self_p->frame.buf_p = self_p->frame.pbuf_p->payload;
    self_p->frame.size = 0;

    self_p->network_interface.info.address = *ipaddr_p;
    self_p->network_interface.info.netmask = *netmask_p;
    self_p->network_interface.info.gateway = *gateway_p;

    netif.name[0] = 's';
    netif.name[1] = 'l';
    netif.output = output;
    netif.mtu = NETWORK_INTERFACE_SLIP_FRAME_SIZE_MAX;
#if LWIP_VERSION_MINOR <= 4
    netif.flags |= NETIF_FLAG_POINTTOPOINT;
#endif
    self_p->network_interface.netif_p = &netif;
    
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
                                struct inet_ip_addr_t *ipaddr_p,
                                struct inet_ip_addr_t *netmask_p,
                                struct inet_ip_addr_t *gateway_p,
                                void *chout_p)
{
    self_p->network_interface.start = NULL;
    self_p->network_interface.stop = NULL;

    return (0);
}

int network_interface_slip_input(struct network_interface_slip_t *self_p,
                                 uint8_t data)
{
    return (0);
}

#endif
