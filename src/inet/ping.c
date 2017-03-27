/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

/* Packet types. */
#define ECHO_REPLY   0
#define ECHO_REQUEST 8

struct echo_header_t {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seqno;
};

struct module_t {
    int initialized;
    short ping_id;
#if CONFIG_FS_CMD_PING_PING == 1
    struct fs_command_t cmd_ping;
#endif
};

static struct module_t module;

#if CONFIG_FS_CMD_PING_PING == 1

static int cmd_ping_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    int res;
    int round_trip_time_ms;
    struct inet_ip_addr_t address;
    struct time_t round_trip_time, timeout;
    const char *remote_host_p;
    
    if (argc != 2) {
        std_fprintf(out_p, OSTR("Usage: ping <remote host>\r\n"));
        return (-1);
    }

    remote_host_p = argv[1];
    
    if (inet_aton(remote_host_p, &address) != 0) {
        std_fprintf(out_p, OSTR("Bad ip address '%s'.\r\n"), remote_host_p);
        return (-1);
    }

    timeout.seconds = 3;
    timeout.nanoseconds = 0;

    res = ping_host_by_ip_address(&address, &timeout, &round_trip_time);

    if (res == 0) {
        round_trip_time_ms = (round_trip_time.seconds * 1000
                              + round_trip_time.nanoseconds / 1000000);
        std_fprintf(out_p,
                    OSTR("Successfully pinged '%s' in %d ms.\r\n"),
                    remote_host_p,
                    round_trip_time_ms);
    } else {
        std_fprintf(out_p, OSTR("Failed to ping '%s'.\r\n"), remote_host_p);
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
                    CSTR("/inet/ping/ping"),
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
    ASSERTN(address_p != NULL, EINVAL);
    ASSERTN(timeout_p != NULL, EINVAL);
    ASSERTN(round_trip_time_p != NULL, EINVAL);
    
    int res = -1;
    ssize_t reply_size;
    struct socket_t socket;
    struct time_t start, stop, timeout;
    struct echo_header_t request;
    struct echo_header_t *reply_p;
    uint8_t reply[28]; /* IP header and icmp packet. */
    struct inet_addr_t address;

    reply_size = -1;
    address.ip = *address_p;
    timeout = *timeout_p;

    /* Initiate the ping echo packet. */
    request.type = ECHO_REQUEST;
    request.code = 0;
    request.checksum = 0;
    request.id = htons(module.ping_id++);
    request.seqno = htons(1);
    request.checksum = htons(inet_checksum(&request, sizeof(request)));
    
    /* Use a raw socket to send and receive ICMP packets. */
    if (socket_open_raw(&socket) != 0) {
        return (-1);
    }
    
    time_get(&start);
     
    if (socket_sendto(&socket,
                      &request,
                      sizeof(request),
                      0,
                      &address) == sizeof(request)) {
        /* Packets for _all_ raw sockets with IMCP and received. */
        while (1) {
            if (chan_poll(&socket, &timeout) == NULL) {
                res = -ETIMEDOUT;
                break;
            }

            time_get(&stop);
            
            reply_size = socket_recvfrom(&socket,
                                         &reply,
                                         sizeof(reply),
                                         0,
                                         &address); 
           
            /* Calculate the round trip time (RTT). */
            time_subtract(round_trip_time_p, &stop, &start);
            
            /* Decrement the timeout by the elapsed time waiting for a
               packet. */
            if ((round_trip_time_p->seconds < 0)
                || (round_trip_time_p->nanoseconds < 0)) {
                timeout.seconds = 0;
                timeout.nanoseconds = -1;
            } else {
                time_subtract(&timeout, timeout_p, round_trip_time_p);
            }
            
            if (reply_size != sizeof(reply)) {
                continue;
            }
                
            reply_p = (struct echo_header_t *)&reply[20];
            
            /* Check the reply. */
            if ((reply_p->type != ECHO_REPLY)
                || (reply_p->code != request.code)
                || (reply_p->id != request.id)
                || (reply_p->seqno != request.seqno)) {
                continue;
            }

            if (inet_checksum(reply_p, sizeof(*reply_p)) == 0) {
                /* A valid reply was received. */
                res = 0;
            }

            break;
        }
    }

    socket_close(&socket);

    return (res);
}
