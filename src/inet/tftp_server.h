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

#ifndef __INET_TFTP_SERVER_H__
#define __INET_TFTP_SERVER_H__

#include "simba.h"

struct tftp_server_t {
    struct inet_addr_t addr;
    struct socket_t listener;
    int timeout_ms;
    const char *name_p;
    const char *root_p;
    void *stack_p;
    size_t stack_size;
    struct thrd_t *thrd_p;
};

/**
 * Initialize given TFTP server.
 *
 * @param[in, out] self_p TFTP server to initialize.
 * @param[in] addr_p Ip address and port of the server.
 * @param[in] timeout_ms Packet reception timeout.
 * @param[in] name_p Name of the server thread.
 * @param[in] root_p File system root path.
 * @param[in] stack_p Server thread stack.
 * @param[in] stack_size Server thread stack size.
 *
 * @return zero(0) or negative error code.
 */
int tftp_server_init(struct tftp_server_t *self_p,
                     struct inet_addr_t *addr_p,
                     int timeout_ms,
                     const char *name_p,
                     const char *root_p,
                     void *stack_p,
                     size_t stack_size);

/**
 * Start given TFTP server.
 *
 * @param[in] self_p TFTP server to start.
 *
 * @return zero(0) or negative error code.
 */
int tftp_server_start(struct tftp_server_t *self_p);

#endif
