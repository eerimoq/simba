/**
 * @file ip.i
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

static int inet_icmp_input(struct ip_header_t *ipheader_p, size_t size);

/* IP header defines.*/
#define VERSION_IPV4  0x40
#define IHL_MINIMUM   0x05
#define MORE_FRAGMENT 0x2000

/**
 * Add IP layer header and send frame.
 */
static int inet_ip_output(struct iovec_t *iovec_p,
                          uint8_t ioveclen,
                          size_t size,
                          uint32_t laddr,
                          uint32_t raddr,
                          uint8_t protocol)
{
    struct ip_header_t header;

    /* Build IPv4 header.*/
    header.version_ihl = (VERSION_IPV4 | IHL_MINIMUM);
    header.dscp_ecn = 0;
    header.length = htons(sizeof(header) + size);
    header.identification = 0;
    header.flags_fragment_offset = 0;
    header.ttl = 64;
    header.protocol = protocol;
    header.header_checksum = 0;
    header.saddr = laddr;
    header.daddr = raddr;
    header.header_checksum = ntohs(inet_checksum((uint16_t*)&header, sizeof(header)));

    /* Add header to io vector.*/
    iovec_p[ioveclen].buf_p = &header;
    iovec_p[ioveclen].size = sizeof(header);
    ioveclen++;

    return (inet_netif_output(iovec_p, ioveclen, raddr));
}

/**
 * Insert a packet into the IP protocol layer.
 */
int inet_ip_input(size_t size)
{
    struct ip_header_t header;
    uint16_t checksum;

    if (size < sizeof(header)) {
        return (-1);
    }

    /* Read fixed IP header from network interface.*/
    inet_netif_read(&header, sizeof(header));

    checksum = inet_checksum((uint16_t *)&header, sizeof(header));
    header.length = htons(header.length);

    /* Header validation.*/
    if ((header.version_ihl != (VERSION_IPV4 | IHL_MINIMUM))
        || (checksum != 0)
        || (header.flags_fragment_offset == htons(MORE_FRAGMENT))
        || (size < header.length)) {
        return (-1);
    }

    /* Size of IP payload.*/
    size = header.length - sizeof(header);

    /* Input packet to next protocol layer.*/
    switch (header.protocol) {
#if (USE_INET_UDP == 1)
    case PROTO_UDP:
        return (inet_udp_input(&header, size));
        break;
#endif
#if (USE_INET_TCP == 1)
    case PROTO_TCP:
        return (inet_tcp_input(&header, size));
        break;
#endif
#if (USE_INET_ICMP == 1)
    case PROTO_ICMP:
        return (inet_icmp_input(&header, size));
        break;
#endif
    default:
        break;
    }

    return (-1);
}
