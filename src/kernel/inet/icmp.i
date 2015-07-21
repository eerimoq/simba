/**
 * @file icmp.i
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

#define ICMP_ECHO_REPLY   0
#define ICMP_ECHO_REQUEST 8

struct icmp_header_t {
    uint8_t type;        /* ICMP type (request/reply). */
    uint8_t code;        /* Subtype to the given type. */
    uint16_t checksum;   /* Internet checksum. */
    uint32_t quench;     /* Rest of Header – Four byte field. Will vary
                            based on the ICMP type and code. */
};

static int inet_icmp_input(struct ip_header_t *ipheader, size_t size)
{
    struct icmp_header_t header;
    struct iovec_t iovec[3] = {
        { &header, sizeof(header) }
    };
    uint8_t ioveclen = 1;

    if (size < sizeof(header))
        return (-1);

    inet_netif_read(&header, sizeof(header));

    if (header.type == ICMP_ECHO_REQUEST) {
        header.type = ICMP_ECHO_REPLY;
        /* update checksum. Only type field changed.*/
        header.checksum = ntohs(header.checksum);

        if (header.checksum >= (0xffffU - (ICMP_ECHO_REQUEST << 8))) {
            header.checksum += (ICMP_ECHO_REQUEST << 8) + 1;
        } else {
            header.checksum += (ICMP_ECHO_REQUEST << 8);
        }

        header.checksum = htons(header.checksum);

        /* Send truncated reply.*/
        return (inet_ip_output(iovec,
                               ioveclen,
                               sizeof(header),
                               ipheader->daddr,
                               ipheader->saddr,
                               PROTO_ICMP));
    }

    return (-1);
}
