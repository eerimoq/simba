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

struct module_t {
    int initialized;
#if CONFIG_FS_CMD_PING_PING == 1
    struct fs_command_t cmd_ping;
#endif
};

static struct module_t module;

#if CONFIG_FS_CMD_PING_PING == 1

static int cmd_ping_cb(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    int res;
    int round_trip_time_ms;
    struct inet_ip_addr_t address;
    struct time_t round_trip_time, timeout;
    const char *remote_host_p;
    
    if (argc != 2) {
        std_fprintf(out_p, FSTR("Usage: %s <remote host>\r\n"), argv[0]);
        return (-1);
    }

    remote_host_p = argv[1];
    
    if (inet_aton(remote_host_p, &address) != 0) {
        std_printf(FSTR("Bad ip address '%s'.\r\n"), remote_host_p);
        return (-1);
    }

    timeout.seconds = 3;
    timeout.nanoseconds = 0;

    res = ping_host_by_ip_address(&address, &timeout, &round_trip_time);

    if (res == 0) {
        round_trip_time_ms = (round_trip_time.seconds * 1000
                              + round_trip_time.nanoseconds / 1000000);
        std_printf(FSTR("Successfully pinged '%s' in %d ms.\r\n"),
                   remote_host_p,
                   round_trip_time_ms);
    } else {
        std_printf(FSTR("Failed to ping '%s'.\r\n"), remote_host_p);
    }
    
    return (0);
}

#endif

int ping_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_PING_PING == 1

    fs_command_init(&module.cmd_ping,
                    FSTR("/inet/ping/ping"),
                    cmd_ping_cb,
                    NULL);
    fs_command_register(&module.cmd_ping);

#endif

    return (socket_module_init());
}

int ping_host_by_ip_address(struct inet_ip_addr_t *address_p,
                            struct time_t *timeout_p,
                            struct time_t *round_trip_time_p)
{
    ssize_t reply_size;
    struct socket_t socket;
    struct time_t start, stop;
    struct echo_header_t request;
    struct echo_header_t *reply_p;
    uint8_t reply[28]; /* IP header and icmp packet. */
    struct inet_addr_t address;

    reply_size = -1;
    address.ip = *address_p;

    /* Initiate the ping echo packet. */
    request.type = ECHO_REQUEST;
    request.code = 0;
    request.checksum = 0;
    request.id = htons(PING_ID);
    request.seqno = htons(1);
    request.checksum = htons(inet_checksum(&request, sizeof(request)));
    
    /* Use a raw socket to send and receive ICMP packets. */
    if (socket_open_raw(&socket) != 0) {
        return (-1);
    }
    
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

    reply_p = (struct echo_header_t *)&reply[20];
    
    /* Check the reply. */
    if ((reply_p->type != ECHO_REPLY)
        || (reply_p->code != request.code)
        || (reply_p->id != request.id)
        || (reply_p->seqno != request.seqno)
        || (inet_checksum(reply_p, sizeof(*reply_p)) != 0)) {
        return (-1);
    }

    /* Calculate the round trip time (RTT). */
    time_diff(round_trip_time_p, &stop, &start);

    return (0);
}
