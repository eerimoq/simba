/**
 * @file ping.c
 * @version 6.0.0
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

/* Packet types. */
#define ECHO_REPLY   0
#define ECHO_REQUEST 8

#define PING_ID      0

struct echo_header_t {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seqno;
};

int ping_module_init(void)
{
    return (socket_module_init());
}

int ping_host_by_ip_address(struct inet_ip_addr_t *address_p,
                            struct time_t *timeout_p,
                            struct time_t *rtt_p)
{
    ssize_t reply_size;
    struct socket_t socket;
    struct time_t start, stop;
    struct echo_header_t request;
    struct echo_header_t reply;
    struct inet_addr_t address;

    reply_size = -1;
    address.ip = *address_p;
    
    /* Initiate the ping echo packet. */
    request.type = ECHO_REQUEST;
    request.code = 0;
    request.checksum = 0;
    request.id = PING_ID;
    request.seqno = 0;
    request.checksum = -1;
    
    /* Use a raw socket to send and receive ICMP packets. */
    socket_open_raw(&socket);
    time_get(&start);

    socket_sendto(&socket, &request, sizeof(request), 0, &address);
    
    if (chan_poll(&socket, timeout_p) != NULL) {
        time_get(&stop);
        reply_size = socket_recvfrom(&socket,
                                     &reply,
                                     sizeof(reply),
                                     0,
                                     &address);
    }

    socket_close(&socket);

    if (reply_size != sizeof(reply)) {
        return (-1);
    }

    /* Check the reply. */
    if ((reply.type != ECHO_REPLY)
        || (reply.code != request.code)
        || (reply.id != request.id)
        || (reply.seqno != request.seqno)) {
        return (-1);
    }

    /* Calculate the round trip time (RTT). */
    time_diff(rtt_p, &stop, &start);
    
    return (0);
}
