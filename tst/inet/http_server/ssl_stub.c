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

int ssl_open_counter = 0;
int ssl_close_counter = 0;
int ssl_write_counter = 0;
int ssl_read_counter = 0;
int ssl_size_counter = 0;

int ssl_module_init()
{
    return (0);
}

int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    int flags,
                    const char *server_hostname_p)
{
    BTASSERT(self_p != NULL);
    BTASSERT(context_p != NULL);
    BTASSERT(socket_p != NULL);
    BTASSERT(flags & SSL_SOCKET_SERVER_SIDE);

    ssl_open_counter++;

    return (chan_init(&self_p->base,
                      (chan_read_fn_t)ssl_socket_read,
                      (chan_write_fn_t)ssl_socket_write,
                      (chan_size_fn_t)ssl_socket_size));
}

int ssl_socket_close(struct ssl_socket_t *self_p)
{
    ssl_close_counter++;

    return (0);
}

ssize_t ssl_socket_write(struct ssl_socket_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    BTASSERT(self_p != NULL);

    ssl_write_counter++;

    return (socket_write(NULL, buf_p, size));
}

ssize_t ssl_socket_read(struct ssl_socket_t *self_p,
                        void *buf_p,
                        size_t size)
{
    BTASSERT(self_p != NULL);

    ssl_read_counter++;

    return (socket_read(NULL, buf_p, size));
}

ssize_t ssl_socket_size(struct ssl_socket_t *self_p)
{
    PRINT_FILE_LINE();
    BTASSERT(self_p != NULL);

    ssl_size_counter++;

    return (0);
}
