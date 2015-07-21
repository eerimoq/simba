/**
 * @file main.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

int main()
{
    struct sock_t udp;
    struct sock_t tcp;
    struct sock_t client;
    chan_t *chan_p;
    size_t size;
    char buf[32];
    struct chan_list_t list;

    sys_start();

    /* Open a UDP socket. */
    BTASSERT(sock_open(&udp, AF_INET, SOCK_TYPE_DGRAM, 0) == 0);
    BTASSERT(sock_bind(&udp, NULL, 0) == 0);
    BTASSERT(sock_connect(&udp, NULL, 0) == 0);

    /* Open a TCP socket. */
    BTASSERT(sock_open(&tcp, AF_INET, SOCK_TYPE_STREAM, 0) == 0);
    BTASSERT(sock_bind(&tcp, NULL, 0) == 0);
    BTASSERT(sock_listen(&tcp, 1) == 0);

    /* Add socket input channels to a channel list. */
    BTASSERT(chan_list_init(&list, NULL, 0) == 0);
    BTASSERT(chan_list_add(&list, &udp) == 1);
    BTASSERT(chan_list_add(&list, &tcp) == 2);

    while (1) {
        /* Wait for events on sockets. */
        chan_p = chan_list_poll(&list);

        if (chan_p == &udp) {
            /* Receive UDP packet. */
            BTASSERT((size = sock_recvfrom(&udp,
                                           buf,
                                           sizeof(buf),
                                           0,
                                           NULL,
                                           0)) == sizeof("UDP says hello!"));
            BTASSERT(strcmp(buf, "UDP says hello!") == 0);
        } else if (chan_p == &tcp) {
            /* Accept TCP client. */
            BTASSERT(sock_accept(&tcp, &client, NULL, 0) == 0);
            BTASSERT(chan_list_add(&list, &client) == 3);
        } else if (chan_p == &client) {
            /* Receive TCP data. */
            BTASSERT(chan_read(&client,
                               buf,
                               sizeof("TCP says hello!")) == sizeof("TCP says hello!"));
            BTASSERT(strcmp(buf, "TCP says hello!") == 0);
            break;
        }
    }

    /* Close all sockets. */
    BTASSERT(sock_close(&client) == 0);
    BTASSERT(sock_close(&tcp) == 0);
    BTASSERT(sock_close(&udp) == 0);

    return (0);
}
