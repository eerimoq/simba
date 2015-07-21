/**
 * @file tcp.i
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

/* TCP header defines.*/
#define FIN 0x01
#define SYN 0x02
#define RST 0x04
#define PSH 0x08
#define ACK 0x10
#define URG 0x20
#define ECE 0x40
#define CWR 0x80
#define NS  0x01

#define DATA_OFFSET           0xf0
#define DATA_OFFSET_MINIMUM   0x50
#define WINDOW_SIZE         0x0020

/* Sequence number comparsions.*/
#define SEQ_LT(s,h) ((int32_t)((s) - (h)) < 0L)
#define SEQ_LE(s,h) ((int32_t)((s) - (h)) <= 0L)
#define SEQ_GT(s,l) ((int32_t)((s) - (l)) > 0L)
#define SEQ_GE(s,l) ((int32_t)((s) - (l)) >= 0L)
#define SEQ_GE_LE(s,l,h) (SEQ_GE(s,l) && SEQ_LE(s,h))
#define SEQ_GT_LE(s,l,h) (SEQ_GT(s,l) && SEQ_LE(s,h))

/* PCB states.*/
#define CLOSED        0
#define SYN_SENT      1
#define ESTABLISHED   2
#define LISTEN        3
#define SYN_RECEIVED  4
/* Active close.*/
#define FIN_WAIT1     5
#define FIN_WAIT2     6
#define CLOSING       7
#define TIME_WAIT     8
/* Passive close.*/
#define CLOSE_WAIT    9
#define LAST_ACK     10

/* Number of retransmission attempts.*/
#define ATTEMPTS_MAX  8

struct tcp_header_t {
    uint16_t lport;          /* Source port.*/
    uint16_t rport;          /* Destination port.*/
    uint32_t seqno;          /* Sequence number.*/
    uint32_t ackno;          /* Acknowledge number.*/
    uint8_t data_offset_ns;  /* TCP header size in 32 bit words and NS flag.*/
    uint8_t flags;           /* Various flags.*/
    uint16_t window_size;    /* Window size.*/
    uint16_t checksum;       /* Checksum.*/
    uint16_t urgent_pointer; /* Urgent pointer (not used).*/
} PACK_STRUCT_STRUCT;

struct pseudo_ip_header_t {
    uint32_t laddr;          /* Source IP address.*/
    uint32_t raddr;          /* Destination IP address.*/
    uint8_t zeros;
    uint8_t protocol;        /* IP protocol number.*/
    uint16_t tcp_length;     /* TCP length.*/
} PACK_STRUCT_STRUCT;

/**
 * Send given payload and flags to remote endpoint.
 * @param[in] pcb TCP protocol control block
 * @param[in] buf payload or NULL
 * @param[in] size payload size
 * @param[in] flags TCP flags
 * @param[in] attempts number of retransmission attempts.
 *                      1: fire and forget. Don't wait for ACK.
 *                     >1: acknowleded delivery. Wait for ACK.
 * @return zero(0) or negative error code
 */
static int inet_tcp_output(struct tcp_pcb_t *pcb,
                           const void *buf,
                           size_t size,
                           uint8_t flags,
                           int attempts)
{
    struct pseudo_ip_header_t pseudoipheader;
    struct tcp_header_t header;
    uint8_t ioveclen = 0;
    struct iovec_t iovec[YIP_IOVEC_MAX];
    int res;
    uint32_t acc;

    /* Fill IO vector.*/
    if (buf != NULL) {
        iovec[ioveclen].buf = (void *)buf;
        iovec[ioveclen].size = size;
        ioveclen++;
    }

    iovec[ioveclen].buf = &header;
    iovec[ioveclen].size = sizeof(header);
    ioveclen++;

    /* Build pseudo IP header.*/
    pseudoipheader.saddr = pcb->laddr;
    pseudoipheader.daddr = pcb->raddr;
    pseudoipheader.zeros = 0;
    pseudoipheader.protocol = PROTO_TCP;
    pseudoipheader.tcp_length = htons(size + sizeof(header));

    /* Build TCP header.*/
    header.sport = pcb->lport;
    header.dport = pcb->rport;
    header.seqno = htonl(pcb->seqno);
    header.ackno = htonl(pcb->ackno);
    header.data_offset_ns = DATA_OFFSET_MINIMUM;
    header.flags = flags;
    header.window_size = WINDOW_SIZE;
    header.checksum = 0;
    header.urgent_pointer = 0;

    /* Calculate TCP packet checksum and add it to the header.*/
    acc = inet_checksum_begin();
    acc = inet_checksum_acc(acc, (uint16_t *)&pseudoipheader, sizeof(pseudoipheader));
    acc = inet_checksum_acc(acc, (uint16_t *)&header, sizeof(header));
    acc = inet_checksum_acc(acc, (uint16_t *)buf, size);
    header.checksum = htons(inet_checksum_end(acc));

    /* Update next sequence number to send.*/
    pcb->seqno += (size + ((flags & (SYN | FIN)) != 0));

    /* Send frame.*/
    return (inet_ip_output(iovec,
                           ioveclen,
                           size + sizeof(header),
                           pcb->laddr,
                           pcb->raddr,
                           PROTO_TCP));
}

/**
 * There are two kinds of close; active and passive. Active
 * close is initiated by local endpoint while passive is initiated
 * by the remote endpoint.
 * This implementation is simplified to send FIN+ACK and wait
 * for ACK. Then the PCB will not receive more packets. The active
 * close will _not_ ACK the FIN from the remote endpoint.
 */
static int inet_tcp_close(void *self)
{
    struct tcp_pcb_t *pcb = (struct tcp_pcb_t *)self;

    /* Already closed if RST was received. Delete PCB.*/
    if (pcb->state != CLOSED) {
        pcb->state = CLOSING;
        inet_tcp_output(pcb, NULL, 0, (FIN | ACK), ATTEMPTS_MAX);
    }

    /* No more packets will be received.*/
    pcb->state = CLOSED;
    pcb->lport = 0;

    return (0);
}

static int inet_tcp_listen(void *self, int backlog)
{
    UNUSED(backlog);

    struct tcp_pcb_t *pcb = (struct tcp_pcb_t *)self;

    pcb->state = LISTEN;

    return (0);
}

static int inet_tcp_connect(void *self,
                            const struct sockaddr *addr,
                            socklen_t addrlen)
{
    sock->cb = inet_tcp_connect_send_syn;
    sock->args = &args;
    thrd_resume(inet_thrd);

    return (thrd_suspend(NULL));
}

/**
 * Initiate 3-way-handshake.
 * 1. Send SYN.
 * 2. Receive SYN+ACK or timeout. Goto 1. on timeout.
 * 3. Send ACK.
 */
static int inet_tcp_connect_send_syn(sock)
{
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    struct tcp_pcb_t *pcb = (struct tcp_pcb_t *)self;

    /* 3-way-handshake step 1/3: Send SYN. */
    pcb->raddr = addr_in->sin_addr.s_addr;
    pcb->rport = addr_in->sin_port;
    pcb->state = SYN_SENT;
    inet_tcp_output(&sock->pcb, NULL, 0, SYN);
}

static int inet_tcp_accept(void *self,
                           struct sockaddr *addr,
                           socklen_t *addrlen)
{
    struct tcp_pcb_t *pcb = (struct tcp_pcb_t *)self, *npcb;
    struct socket_t *sock = pcb->sock;

    /* Set accept variables.*/
    sock->recv.buf = &npcb;
    sock->recv.src_addr = (struct sockaddr_in *)addr;
    sock->recv.addrlen = addrlen;

    /* Send SYN+ACK packet and wait for ACK.*/
    inet_tcp_output(npcb,
                    NULL,
                    0,
                    (SYN | ACK));

    return (npcb->sock->sd);
}

static size_t inet_tcp_sendto(void *self,
                              const void *buf,
                              size_t size,
                              int flags,
                              const struct sockaddr *dest_addr,
                              socklen_t addrlen)
{
    UNUSED(flags);
    UNUSED(dest_addr);
    UNUSED(addrlen);

    struct tcp_pcb_t *pcb = (struct tcp_pcb_t *)self;

    return ((inet_tcp_output(pcb,
                             buf,
                             size,
                             ACK | PSH) == 0) ? size : 0);
}

static int inet_tcp_input_listen(struct tcp_pcb_t *pcb,
                                 struct socket_t *sock,
                                 struct ip_header_t *ipheader,
                                 struct tcp_header_t *header)
{
    if (header->flags & RST) {
        return (0);
    }

    if (header->flags & ACK) {
        return (tcpOutput(pcb, NULL, 0, RST, 1));
    }

    if (header->flags & SYN) {
        if (IS_STATE(sock->state, YIP_ACCEPT)) {
            /* Copy source address if requested.*/
            if (sock->recv.src_addr != NULL) {
                sock->recv.src_addr->sin_addr.s_addr = ipheader->saddr;
                sock->recv.src_addr->sin_port = header->sport;
            }

            struct socket_t *nsock = socketAlloc();
            *(void **)sock->recv.buf = nsock;

            if (nsock != NULL) {
                SET_STATE(nsock->state, YIP_IDLE);
                nsock->tp = sock->tp;
                struct tcp_pcb_t *npcb = yipTcpOpen(nsock);
                *(void **)sock->recv.buf = nsock->pcb = npcb;

                if (npcb != NULL) {
                    npcb->laddr = pcb->laddr;
                    npcb->lport = pcb->lport;
                    npcb->raddr = ipheader->saddr;
                    npcb->rport = header->sport;
                    npcb->seqno = 0;
                    npcb->ackno = htonl(header->seqno) + 1;
                    npcb->state = SYN_RECEIVED;
                } else {
                    socketFree(nsock);
                }
            }

            SET_STATE(sock->state, YIP_IDLE);
            chThdResume(sock->tp);
        }
    }

    return (0);
}

static int inet_tcp_input_syn_sent(struct tcp_pcb_t *pcb,
                                   struct socket_t *sock,
                                   struct tcp_header_t *header)
{
    if ((header->flags & (SYN | ACK)) == (SYN | ACK)) {
        pcb->state = ESTABLISHED;
        chThdResume(sock->tp);
    }

    return (0);
}

/**
 * Segment arrives.
 */
static int inet_tcp_input_otherwise(struct tcp_pcb_t *pcb,
                                    struct socket_t *sock,
                                    size_t size,
                                    struct ip_header_t *ipheader,
                                    struct tcp_header_t *header)
{
    UNUSED(ipheader);

    /* Acceptable segment checks. Only accept if next
       expected sequence number is received.*/
    if (ntohl(header->seqno) != pcb->ackno) {
        return (0);
    }

    /* Create idealized packet.*/


    /* SYN is never expected within the window. Reset.*/
    if (header->flags & SYN) {
        return (-1);
    }

    /* ACK handling.*/
    if (header->flags & ACK) {
        if (pcb->state == ESTABLISHED) {
            /* Resume waiting thread when all sent data
               is acknowledged.*/
            if ((IS_STATE(sock->state, YIP_SEND)
                 && (ntohl(header->ackno) == pcb->seqno))) {
                chThdResume(sock->tp);
            }
        } else if (pcb->state == CLOSING) {
            /* Passive or active close "completed".*/
            chThdResume(sock->tp);
        } else if (pcb->state == SYN_RECEIVED) {
            pcb->state = ESTABLISHED;
            chThdResume(sock->tp);
            return (0);
        }
    }

    /* Process segment text or FIN. Resume waiting thread or discard.*/
    yipSelectSync(sock);
    if (IS_STATE(sock->state, YIP_RECV)) {
        int resume = 0;
        pcb->ackno = htonl(header->seqno);
        /* Immediatly enter closed state on RST.*/
        if (header->flags & RST) {
            sock->recv.offset = 0;
            pcb->state = CLOSED;
            resume = 1;
        } else if (pcb->state == ESTABLISHED) {
            /* Only accept data in ESTABLISHED.*/
            if (size > 0) {
                /* Receive payload.*/
                if ((sock->recv.size - sock->recv.offset) < size) {
                    size = (sock->recv.size - sock->recv.offset);
                }
                size = netifRead(sock->recv.buf + sock->recv.offset, size);
                sock->recv.offset += size;
                resume = (sock->recv.offset == sock->recv.size);
                pcb->ackno += size;
            } else if (header->flags & FIN) {
                /* FIN received. Passive close.*/
                sock->recv.offset = 0;
                pcb->state = CLOSE_WAIT;
                resume = 1;
            }
        }
        if (resume == 1) {
            SET_STATE(sock->state, YIP_IDLE);
            chThdResume(sock->tp);
        }
        /* Form the ACK packet (text or FIN).*/
        tcpOutput(pcb, NULL, 0, ACK, 1);
    }

    return (0);
}

static struct yip_protocol_t protocol = {
    .close = inet_tcp_close,
    .listen = inet_tcp_listen,
    .connect = inet_tcp_connect,
    .accept = inet_tcp_accept,
    .sendto = inet_tcp_sendto,
};

/**
 * Open TCP PCB.
 * @param[in] sock socket for this PCB.
 * @return socket descriptor or negative error code
 */
void *inet_tcp_open(struct socket_t *sock)
{
    struct tcp_pcb_t *pcb = tcpAlloc();

    if (pcb == NULL) return (NULL);

    memset(pcb, 0, sizeof(*pcb));
    sock->protocol = &protocol;
    pcb->sock = sock;

    return (pcb);
}

/**
 * Insert packet into TCP layer.
 * @param[in] ipheader packet ip header
 * @return zero(0) or negative error code
 */
int inet_tcp_input(struct ip_header_t *ipheader, size_t size)
{
    uint8_t i;
    uint32_t ignore;
    struct tcp_header_t header;
    struct tcp_pcb_t *pcb, *listen_pcb = NULL;
    struct socket_t *sock;
    uint8_t data_offset;

    if (size < sizeof(header)) {
        return (-1);
    }

    /* Read TCP header.*/
    inet_netif_read(&header, sizeof(header));

    data_offset = (4 * ((header.data_offset_ns & DATA_OFFSET) >> 4));

    if (size < data_offset)
        return (-1);

    /* Ignore any optional header fields.*/
    for (i = 0; i < (data_offset - sizeof(header)) / 4; i++) {
        netifRead(&ignore, sizeof(ignore));
    }

    size -= data_offset;

    /* Find receiving pcb.*/
    for (pcb = tcp_pcb; pcb < &tcp_pcb[YIP_TCP_MAX]; pcb++) {
        /* Local match.*/
        if ((pcb->lport == header.dport)
            && (pcb->laddr == ipheader->daddr)) {
            /* Connected has higher prio than listen.*/
            if ((pcb->rport == header.sport)
                && (pcb->raddr == ipheader->saddr)) {
                goto input;
            }
            if (pcb->state == LISTEN) {
                listen_pcb = pcb;
            }
        }
    }

    /* Discard unexpected packets.*/
    pcb = listen_pcb;

    if (pcb == NULL) {
        return (-1);
    }

 input:
    sock = pcb->sock;

    /* State handling.*/
    switch (pcb->state) {
    case LISTEN:
        return (inet_tcp_input_listen(pcb, sock, ipheader, &header));
    case SYN_SENT:
        return (inet_tcp_input_syn_sent(pcb, sock, &header));
    default:
        return (inet_tcp_input_otherwise(pcb, sock, size, ipheader, &header));
    }

    return (0);
}
