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

int socket_module_init()
{
    return (0);
}

int socket_open_tcp(struct socket_t *self_p)
{
    chan_init(&self_p->base,
              chan_read_null,
              chan_write_null,
              (chan_size_fn_t)socket_size);

    return (0);
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
    chan_init(&accepted_p->base,
              chan_read_null,
              chan_write_null,
              (chan_size_fn_t)socket_size);

    return (0);
}

ssize_t socket_sendto(struct socket_t *self_p,
                      const void *buf_p,
                      size_t size,
                      int flags,
                      const struct inet_addr_t *remote_addr_p)
{
    return (0);
}

ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct inet_addr_t *remote_addr)
{
    return (0);
}

ssize_t socket_write(struct socket_t *self_p,
                     const void *buf_p,
                     size_t size)
{
    if (size == 6) {
        return (6);
    }

    if (size == 8) {
        return (8);
    }

    return (-1);
}

ssize_t socket_read(struct socket_t *self_p,
                    void *buf_p,
                    size_t size)
{
    if (size == 8) {
        strcpy((char *)buf_p, "goodbye");
        return (8);
    } else if (size == 6) {
        strcpy((char *)buf_p, "hello");
        return (6);
    }

    return (-1);
}

ssize_t socket_size(struct socket_t *self_p)
{
    return (0);
}
