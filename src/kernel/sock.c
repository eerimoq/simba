/**
 * @file sock.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

int sock_open(struct sock_t *sock_p,
              int domain,
              int type,
              int protocol)
{
    UNUSED(domain);
    UNUSED(protocol);

    switch (type) {
#if (USE_YIP_TCP == 1)
    case SOCK_STREAM:
        inet_tcp_open(sock_p);
        break;
#endif
#if (USE_YIP_UDP == 1)
    case SOCK_DGRAM:
        inet_udp_open(sock_p);
        break;
#endif
    default:
        break;
    }

    return (0);
}

int sock_close(struct sock_t *sock_p)
{
    return (-1);//sock_p->proto_p->close(sock_p->pcb));
}

int sock_bind(struct sock_t *sock_p,
              const struct sockaddr_t *addr_p,
              size_t addrlen)
{
    /* sock_p->pcb.udp.laddr = netif_get_ip(); */
    /* sock_p->pcb.udp.lport = addr_p->port; */

    return (-1);
}

int sock_listen(struct sock_t *sock_p, int backlog)
{
    return (-1);//sock_p->proto_p->listen(sock_p, backlog));
}

int sock_connect(struct sock_t *sock_p,
                 const struct sockaddr_t *addr,
                 size_t addrlen)
{
    return (-1);//sock_p->proto_p->connect(sock_p, addr, addrlen));
}

int sock_accept(struct sock_t *sock_p,
                struct sock_t *accepted,
                struct sockaddr_t *addr,
                size_t *addrlen)
{
    return (-1);//sock_p->proto_p->accept(sock_p, addr, addrlen));
}

ssize_t sock_sendto(struct sock_t *sock_p,
                    const void *buf_p,
                    size_t size,
                    int flags,
                    const struct sockaddr_t *remote_addr,
                    size_t addrlen)
{
    return (-1);/*sock_p->proto_p->sendto(sock_p,
                                buf,
                                size,
                                flags,
                                remote_addr,
                                addrlen));*/
}

ssize_t sock_recvfrom(struct sock_t *sock_p,
                      void *buf_p,
                      size_t size,
                      int flags,
                      struct sockaddr_t *remote_addr,
                      size_t addrlen)
{
    return (-1);/* sock_p->proto_p->recvfrom(sock_p, */
               /*                    buf, */
               /*                    size, */
               /*                    flags, */
               /*                    remote_addr, */
               /*                    addrlen)); */
}
