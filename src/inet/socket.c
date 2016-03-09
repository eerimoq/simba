/**
 * @file socket.c
 * @version 0.2.0
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

/* /\** */
/*  * Callback function called by lwip when there is data available. */
/*  *\/ */
/* static err_t recv_cb(void *arg_p, */
/*                      struct tcp_pcb *pcb_p, */
/*                      struct pbuf *pbuf_p, */
/*                      err_t err) */
/* { */
/*     struct socket_t *socket_p = arg_p; */

/*     UNUSED(socket_p); */

/*     return (0); */
/* } */

/* /\** */
/*  * Callback function called by lwip when data has been sent. */
/*  *\/ */
/* static err_t sent_cb(void *arg_p, */
/*                      struct tcp_pcb *pcb_p, */
/*                      uint16_t len) */
/* { */
/*     struct socket_t *socket_p = arg_p; */

/*     UNUSED(socket_p); */

/*     return (0); */
/* } */

/* /\** */
/*  * Callback function called by lwip when an error occurs. */
/*  *\/ */
/* static void err_cb(void *arg_p, err_t err) */
/* { */
/*     struct socket_t *socket_p = arg_p; */

/*     UNUSED(socket_p); */
/* } */

int socket_open(struct socket_t *self_p,
                int domain,
                int type,
                int protocol)
{
    void *pcb_p = NULL;
    
    switch (type) {

    /* case SOCKET_TYPE_STREAM: */
    /*     /\* /\\* Create and initiate the TCP pcb. *\\/ *\/ */
    /*     /\* pcb_p = tcp_new(); *\/ */
    /*     /\* tcp_arg(pcb_p, self_p); *\/ */
    /*     /\* //tcp_accept(pcb_p, accept_cb); *\/ */
    /*     /\* tcp_recv(pcb_p, recv_cb); *\/ */
    /*     /\* tcp_sent(pcb_p, sent_cb); *\/ */
    /*     /\* tcp_err(pcb_p, err_cb); *\/ */
    /*     break; */

    case SOCKET_TYPE_DGRAM:
        /* Create and initiate the UDP pcb. */
        pcb_p = udp_new();
        break;

    default:
        return (-1);
    }

    self_p->pcb_p = pcb_p;

    return (0);
}

int socket_close(struct socket_t *self_p)
{
    udp_remove(self_p->pcb_p);

    return (0);
}

int socket_bind(struct socket_t *self_p,
                const struct socket_addr_t *local_addr_p,
                size_t addrlen)
{
    ip_addr_t ip;
    
    ip.addr = local_addr_p->ip;
    
    return (udp_bind(self_p->pcb_p,
                     &ip,
                     local_addr_p->port));
}

int socket_listen(struct socket_t *self_p, int backlog)
{
    return (-1);
}

int socket_connect(struct socket_t *self_p,
                   const struct socket_addr_t *addr_p,
                   size_t addrlen)
{
    return (-1);
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
    struct pbuf *pbuf_p;
    ip_addr_t ip;
    
    ip.addr = remote_addr_p->ip;
    pbuf_p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);

    memcpy(pbuf_p->payload, buf_p, size);

    if (udp_sendto(self_p->pcb_p,
                   pbuf_p,
                   &ip,
                   remote_addr_p->port) != 0) {
        return (-1);
    }

    return (0);
}

ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct socket_addr_t *remote_addr,
                        size_t addrlen)
{
    return (-1);
}
