/**
 * @file socket_stub.c
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
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

static struct queue_t qinput;
static struct queue_t qoutput;
static char qinputbuf[256];
static char qoutputbuf[256];
static struct event_t accept_events;

static ssize_t read(void *self_p,
                    void *buf_p,
                    size_t size)
{
    return (queue_read(&qinput, buf_p, size));
}

static ssize_t write(void *self_p,
                     const void *buf_p,
                     size_t size)
{
    return (chan_write(&qoutput, buf_p, size));
}

static size_t size(void *self_p)
{
    return (0);
}

int socket_module_init()
{
    return (0);
}

int socket_open_tcp(struct socket_t *self_p)
{
    return (chan_init(&self_p->base, read, write, size));
}

int socket_open_udp(struct socket_t *self_p)
{
    return (-1);
}

int socket_open_raw(struct socket_t *self_p)
{
    return (0);
}

int socket_close(struct socket_t *self_p)
{
    return (0);
}

int socket_bind(struct socket_t *self_p,
                const struct inet_addr_t *local_addr_p)
{
    char buf[16];

    BTASSERT(strcmp(inet_ntoa(&local_addr_p->ip, &buf[0]), "127.0.0.1") == 0);

    return (0);
}

int socket_listen(struct socket_t *self_p, int backlog)
{
    return (0);
}

int socket_connect(struct socket_t *self_p,
                   const struct inet_addr_t *addr_p)
{
    return (0);
}

int socket_accept(struct socket_t *self_p,
                  struct socket_t *accepted_p,
                  struct inet_addr_t *addr_p)
{
    uint32_t mask;

    mask = 0x1;
    event_read(&accept_events, &mask, sizeof(mask));

    return (0);
}

ssize_t socket_sendto(struct socket_t *self_p,
                      const void *buf_p,
                      size_t size,
                      int flags,
                      const struct inet_addr_t *remote_addr_p)
{
    return (write(NULL, buf_p, size));
}

ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct inet_addr_t *remote_addr)
{
    return (read(NULL, buf_p, size));
}

ssize_t socket_write(struct socket_t *self_p,
                     const void *buf_p,
                     size_t size)
{
    return (write(NULL, buf_p, size));
}

ssize_t socket_read(struct socket_t *self_p,
                    void *buf_p,
                    size_t size)
{
    return (read(NULL, buf_p, size));
}

void socket_stub_init()
{
    queue_init(&qinput, qinputbuf, sizeof(qinputbuf));
    queue_init(&qoutput, qoutputbuf, sizeof(qoutputbuf));
    event_init(&accept_events);
}

void socket_stub_accept()
{
    uint32_t mask;

    mask = 0x1;
    event_write(&accept_events, &mask, sizeof(mask));
}

void socket_stub_input(void *buf_p, size_t size)
{
    chan_write(&qinput, buf_p, size);
}

void socket_stub_output(void *buf_p, size_t size)
{
    chan_read(&qoutput, buf_p, size);
}
