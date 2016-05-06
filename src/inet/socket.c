/**
 * @file socket.c
 * @version 0.3.0
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

#include "inet.h"

#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_common.h"

extern xSemaphoreHandle thrd_idle_sem;

/**
 * The LwIP-stack calls this function when a UDP packet has been
 * received. It is called from the LwIP-thread. For ESP, this is the
 * FreeRTOS LwIP-thread.
 */
static void on_udp_recv(void *arg_p,
                        struct udp_pcb *pcb_p,
                        struct pbuf *pbuf_p,
                        ip_addr_t *addr_p,
                        uint16_t port)
{
    struct socket_t *socket_p = arg_p;

    if (socket_p->recv.thrd_p != NULL) {
        if (pbuf_p->tot_len < socket_p->recv.size) {
            socket_p->recv.size = pbuf_p->tot_len;
        }

        /* Write the packet to the receive buffer. */
        pbuf_copy_partial(pbuf_p,
                          socket_p->recv.buf_p,
                          socket_p->recv.size,
                          0);

        /* Copy the remote address and port to the receiver. */
        socket_p->recv.remote_addr_p->ip = addr_p->addr;
        socket_p->recv.remote_addr_p->port = port;

        /* Resume the reading thread. */
        thrd_resume_isr(socket_p->recv.thrd_p, 0);
        socket_p->recv.thrd_p = NULL;
        xSemaphoreGive(thrd_idle_sem);
    }

    pbuf_free(pbuf_p);
}

/**
 * Called by lwip when there is TCP data available.
 */
static err_t on_tcp_recv(void *arg_p,
                         struct tcp_pcb *pcb_p,
                         struct pbuf *pbuf_p,
                         err_t err)
{
    return (0);
}

/**
 * Called by lwip when TCP data has been sent.
 */
static err_t on_tcp_sent(void *arg_p,
                         struct tcp_pcb *pcb_p,
                         uint16_t len)
{
    return (0);
}

/**
 * Called by lwip when an TCP error occurs.
 */
static void on_tcp_err(void *arg_p, err_t err)
{
}

int socket_open(struct socket_t *self_p,
                int domain,
                int type,
                int protocol)
{
    void *pcb_p = NULL;

    switch (type) {

    case SOCKET_TYPE_STREAM:
        /* Create and initiate the TCP pcb. */
        pcb_p = tcp_new();
        tcp_arg(pcb_p, self_p);
        //tcp_accept(pcb_p, on_tcp_accept);
        tcp_recv(pcb_p, on_tcp_recv);
        tcp_sent(pcb_p, on_tcp_sent);
        tcp_err(pcb_p, on_tcp_err);
        break;

    case SOCKET_TYPE_DGRAM:
        /* Create and initiate the UDP pcb. */
        pcb_p = udp_new();
        udp_recv(pcb_p, on_udp_recv, self_p);
        break;

    default:
        return (-1);
    }

    self_p->type = type;
    self_p->pcb_p = pcb_p;

    return (0);
}

int socket_close(struct socket_t *self_p)
{
    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        tcp_close(self_p->pcb_p);
        break;

    case SOCKET_TYPE_DGRAM:
        udp_recv(self_p->pcb_p, NULL, NULL);
        udp_remove(self_p->pcb_p);
        break;

    default:
        return (-1);
    }

    return (0);
}

int socket_bind(struct socket_t *self_p,
                const struct socket_addr_t *local_addr_p,
                size_t addrlen)
{
    ip_addr_t ip;

    ip.addr = local_addr_p->ip;

    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        return (tcp_bind(self_p->pcb_p, &ip, local_addr_p->port));
        
    case SOCKET_TYPE_DGRAM:
        return (udp_bind(self_p->pcb_p, &ip, local_addr_p->port));
        
    default:
        return (-1);
    }
}

int socket_listen(struct socket_t *self_p, int backlog)
{
    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        tcp_listen_with_backlog(self_p->pcb_p, backlog);
        break;
        
    default:
        return (-1);
    }

    return (0);
}

int socket_connect(struct socket_t *self_p,
                   const struct socket_addr_t *addr_p,
                   size_t addrlen)
{
    ip_addr_t ip;

    ip.addr = addr_p->ip;

    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        return (tcp_connect(self_p->pcb_p, &ip, addr_p->port, NULL));
        
    case SOCKET_TYPE_DGRAM:
        return (udp_connect(self_p->pcb_p, &ip, addr_p->port));
        
    default:
        return (-1);
    }

}

int socket_accept(struct socket_t *self_p,
                  struct socket_t *accepted_p,
                  struct socket_addr_t *addr_p,
                  size_t *addrlen_p)
{
    return (-1);
}

ssize_t socket_sendto(struct socket_t *self_p,
                      const void *buf_p,
                      size_t size,
                      int flags,
                      const struct socket_addr_t *remote_addr_p,
                      size_t addrlen)
{
    ssize_t res = size;
    struct pbuf *pbuf_p;
    ip_addr_t ip;

    ip.addr = remote_addr_p->ip;
    pbuf_p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
    memcpy(pbuf_p->payload, buf_p, size);

    if (udp_sendto(self_p->pcb_p,
                   pbuf_p,
                   &ip,
                   remote_addr_p->port) != 0) {
        res = -1;
    }

    pbuf_free(pbuf_p);

    return (res);
}

ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct socket_addr_t *remote_addr_p,
                        size_t addrlen)
{
    /* The receive callback copies the data to the receive buffer. */
    self_p->recv.buf_p = buf_p;
    self_p->recv.size = size;
    self_p->recv.remote_addr_p = remote_addr_p;
    self_p->recv.thrd_p = thrd_self();

    /* Wait for data. Resumed from on_udp_recv(). */
    thrd_suspend(NULL);
    
    return (self_p->recv.size);
}

ssize_t socket_write(struct socket_t *self_p,
                     const void *buf_p,
                     size_t size)
{
    return (-1);
}

ssize_t socket_read(struct socket_t *self_p,
                    void *buf_p,
                    size_t size)
{
    return (-1);
}
