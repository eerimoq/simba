/**
 * @file main.c
 * @version 0.1.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

/* UDP ports to use. */
#define UDP_LOCAL_PORT  30303
#define UDP_REMOTE_PORT 30303

static char packet[512];
static struct uart_driver_t uart;

FS_COMMAND_DEFINE("/udp_send", cmd_udp_send);

static struct shell_args_t shell_args;

int cmd_udp_send()
{
    ssize_t size;
    struct socket_t udp;
    struct socket_addr_t remote_addr;
    chan_t *chan_p;
    struct chan_list_t list;
    struct time_t timeout;

    /* Setup a UDP socket with given ports. */
    socket_open(&udp, AF_INET, SOCKET_TYPE_DGRAM, 0);
    
    /* Send a packet to the server. */
    socket_sendto(&udp,
                  packet,
                  sizeof(packet),
                  0,
                  &remote_addr,
                  sizeof(remote_addr));
        
    std_printf(FSTR("Packet sent. Waiting for reply.\r\n"));

    /* Wait for the server to send the packet back to us. Timeout is
     * it doesn't. */
    chan_p = chan_list_poll(&list, &timeout);

    if (chan_p != NULL) {
        size = socket_recvfrom(&udp,
                               packet,
                               sizeof(packet),
                               0,
                               &remote_addr,
                               sizeof(remote_addr));
        
        if (size == 0) {
            std_printf(FSTR("The socket was closed.\r\n"));
            return (1);
        }

        std_printf(FSTR("Received packet of size %ld.\r\n"),
                   (long)size);
    } else {
        std_printf(FSTR("A timeout occured when waiting for the "
                        "server to send the packet back to us.\r\n"));
    }

    /* Close the socket. */
    socket_close(&udp);

    return (0);    
}

int main()
{
    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&uart);

    shell_main(&shell_args);

    return (0);
}
