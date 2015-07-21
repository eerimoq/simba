/**
 * @file kernel/inet.h
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

#ifndef __KERNEL_INET_H__
#define __KERNEL_INET_H__

/**
 * Configuration.
 */

/* Enable all protocols by default.*/
#ifndef USE_INET_ETHARP
#    define USE_INET_ETHARP 0
#endif

#ifndef USE_INET_UDP
#    define USE_INET_UDP 0
#endif

#ifndef USE_INET_TCP
#    define USE_INET_TCP 0
#endif

#ifndef USE_INET_ICMP
#    define USE_INET_ICMP 1
#endif

/* Protocol specific configuration.*/
#ifndef INET_ARP_CACHE_MAX
#    define INET_ARP_CACHE_MAX 4
#endif

#ifndef INET_IP_STACK_THREAD_PRIO
#    define INET_IP_STACK_THREAD_PRIO 64
#endif

/**
 * Socket states and helper macros.
 */
#define INET_IDLE       0x00
#define INET_SEND       0x01
#define INET_RECV       0x02
#define INET_SELECT     0x03
#define INET_ACCEPT     0x04
#define INET_STATE_MASK 0x07
#define INET_EVENT      0x08
#define SET_STATE(sock_state, state) sock_state = ((sock_state & ~INET_STATE_MASK) | state)
#define IS_STATE(sock_state, state) ((sock_state & INET_STATE_MASK) == state)

#define INET_IOVEC_MAX 4

/* IP protocol numbers.*/
#define PROTO_ICMP  1
#define PROTO_TCP   6
#define PROTO_UDP  17

#define ETH_MAX 6

struct iovec_t {
    void *buf_p;
    size_t size;
};

struct inet_tcp_t {
    uint32_t laddr;      /* Local address. */
    uint32_t raddr;      /* Remote address. */
    uint16_t lport;      /* Local port. */
    uint16_t rport;      /* Remote port. */
    uint8_t state;       /* TCP socket state. */
    uint32_t seqno;      /* Next seqno to send. */
    uint32_t ackno;      /* Last recieved seqno. */
};

struct inet_udp_t {
    uint32_t laddr;      /* Local address. */
    uint32_t raddr;      /* Remote address. */
    uint16_t lport;      /* Local port. */
    uint16_t rport;      /* Remote port. */
};

struct ip_header_t {
    uint8_t version_ihl;            /* Version & internet header length.*/
    uint8_t dscp_ecn;               /* Differentiated services code point
                                       & explicit congestion notification.*/
    uint16_t length;                /* Total length (header + data).*/
    uint16_t identification;        /* Identification.*/
    uint16_t flags_fragment_offset; /* Flags & fragment offset.*/
    uint8_t ttl;                    /* Time to live.*/
    uint8_t protocol;               /* IP protocol number.*/
    uint16_t header_checksum;       /* Header checksum.*/
    uint32_t saddr;                 /* Source IP address.*/
    uint32_t daddr;                 /* Destination IP address.*/
} PACK_STRUCT_STRUCT;

struct etharp_header_t {
    uint16_t hwtype;
    uint16_t proto;
    uint8_t  hwlen;
    uint8_t  protolen;
    uint16_t opcode;
    uint8_t smacaddr[ETH_MAX];
    uint32_t sipaddr;
    uint8_t dmacaddr[ETH_MAX];
    uint32_t dipaddr;
};

#endif
