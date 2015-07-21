/**
 * @file udp.i
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

struct udp_header_t {
    uint16_t sport;    /* Source port.*/
    uint16_t dport;    /* Destination port.*/
    uint16_t length;   /* Length header + payload.*/
    uint16_t checksum; /* Checksum (mandatory for IPv6).*/
};

static int udp_close(void *self)
{
    struct udp_pcb_t *pcb = (struct udp_pcb_t *)self;

    pcb->lport = 0;
    pcb->sock = (struct socket_t *)freelist;

    return (0);
}

/**
 * Set remote address and port. Also, packets
 * will only be received from this address.
 */
static int udp_connect(void *self, const struct sockaddr *addr, socklen_t addrlen)
{
    (void)addrlen;
    struct udp_pcb_t *pcb = (struct udp_pcb_t *)self;
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;

    pcb->raddr = addr_in->sin_addr.s_addr;
    pcb->rport = addr_in->sin_port;

    return (0);
}

/**
 * Add UDP header and send packet to IP layer.
 */
static size_t udp_sendto(void *self,
                              const void *buf,
                              size_t size,
                              int flags,
                              const struct sockaddr *dest_addr,
                              socklen_t addrlen)
{
    (void)flags;
    (void)addrlen;
    struct sockaddr_in *addr_in = (struct sockaddr_in *)dest_addr;
    struct udp_header_t header;
    uint32_t raddr;
    uint8_t ioveclen = 2;
    struct iovec_t iovec[YIP_IOVEC_MAX] = {
        { (uint8_t*)buf, size },
        { &header, sizeof(header) }
    };
    struct udp_pcb_t *pcb = (struct udp_pcb_t *)self;

    /* Build UDP header.*/
    header.sport = pcb->lport;

    if (dest_addr == NULL) {
        header.dport = pcb->rport;
        raddr = pcb->raddr;
    } else {
        header.dport = addr_in->sin_port;
        raddr = addr_in->sin_addr.s_addr;
    }

    header.length = htons(sizeof(header) + size);
    header.checksum = 0;

    if (inet_ip_output(iovec,
                       ioveclen,
                       sizeof(header) + size,
                       pcb->laddr,
                       raddr,
                       PROTO_UDP) != 0) {
        return (0);
    }

    return (size);
}

static struct sock_protocol_t udp_protocol = {
    .close = udp_close,
    .connect = udp_connect,
    .recvfrom = udp_recvfrom,
    .sendto = udp_sendto,
};

/**
 * Open UDP PCB.
 * @param[in] sock socket for this PCB.
 * @return socket descriptor or negative error code
 */
void *inet_udp_open(struct socket_t *sock)
{
    return (NULL);
}

/**
 * Insert packet into UDP layer.
 * @param[in] ipheader packet ip header
 * @return zero(0) or negative error code
 */
static int udp_input(struct ip_header_t *ipheader, size_t size)
{
    struct udp_header_t header;
    struct udp_pcb_t *pcb, *uncon_pcb;
    struct socket_t *sock;

    if (size < sizeof(header)) {
        return (-1);
    }

    /* Read UDP header.*/
    inet_netif_read(&header, sizeof(header));
    size -= sizeof(header);

    /* Find receiving pcb.*/
    uncon_pcb = NULL;

    for (pcb = udp_pcb; pcb < &udp_pcb[YIP_UDP_MAX]; pcb++) {
        /* Local match.*/
        if ((pcb->lport != header.dport)
            || (pcb->laddr != ipheader->daddr)) {
            continue;
        }

        uncon_pcb = pcb;

        /* Remote match for connected PCBs.*/
        if ((pcb->rport == header.sport)
            && (pcb->raddr == ipheader->saddr)) {
            goto read;
        }
    }

    pcb = uncon_pcb;

    /* Discard packets with no pcb associated with them.*/
    if (pcb == NULL) {
        return (-1);
    }

    /* Read packet if socket is expecting it, otherwise discard.*/
 read:
    sock = pcb->sock;
    yipSelectSync(sock);

    if (IS_STATE(sock->state, YIP_RECV)) {
        /* Copy source address if requested.*/
        if (sock->recv.src_addr != NULL) {
            sock->recv.src_addr->sin_addr.s_addr = ipheader->saddr;
            sock->recv.src_addr->sin_port = header.sport;
        }

        /* Copy packet payload.*/
        if (size < sock->recv.size) {
            sock->recv.size = size;
        }

        inet_netif_read(sock->recv.buf, sock->recv.size);
        SET_STATE(sock->state, YIP_IDLE);
        chThdResume(sock->tp);
    }

    return (0);
}
