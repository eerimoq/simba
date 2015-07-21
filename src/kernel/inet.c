/**
 * @file inet.c
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

#define htons(v) v
#define htonl(v) v
#define ntohs(v) v
#define ntohl(v) v

static uint32_t inet_checksum_begin(void)
{
  return (0);
}

static uint32_t inet_checksum_acc(uint32_t acc, uint16_t *buf_p, size_t size)
{
    while (size > 1) {
        acc += htons(*buf_p++);
        size -= 2;
    }

    if (size > 0) {
        acc += (htons(*buf_p) & 0xff00);
    }

    return (acc);
}

static uint16_t inet_checksum_end(uint32_t acc)
{
    acc = (acc >> 16) + (acc & 0xffffUL);

    if ((acc & 0xffff0000UL) != 0) {
        acc = (acc >> 16) + (acc & 0xffffUL);
    }

    return (~acc);
}

static uint16_t inet_checksum(uint16_t *buf_p, size_t size)
{
    uint32_t acc = inet_checksum_begin();

    acc = inet_checksum_acc(acc, buf_p, size);

    return (inet_checksum_end(acc));
}

#include "inet/ip.i"
#if (USE_INET_UDP == 1)
#    include "inet/udp.i"
#endif
#if (USE_INET_TCP == 1)
#    include "inet/tcp.i"
#endif
#if (USE_INET_ICMP == 1)
#    include "inet/icmp.i"
#endif

/* static int timeout = 0; */
/* static struct thrd_t *inet_thrd; */

/* static void inet_timeout() */
/* { */
/*     timeout = 1; */
/*     thrd_resume_i(inet_thrd, 0); */
/* } */

void *inet_entry(void *arg_p)
{
    /* Start timer. */
    //timer_set();

    while (1) {
        thrd_suspend(NULL);

        /* Handle incoming packets from network interface. */
        inet_netif_input();
#if 0
        /* Check if data should be sent. */
        while ((job = job_pop()) != NULL) {
            job_p->sock_p->cb(sock);
        }

        /* Timer events. */
        if (timeout == 1) {
            timeout = 0;
            timer_set();
        }
#endif
    }
}
