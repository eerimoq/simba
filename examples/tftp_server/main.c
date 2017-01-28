/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#define IP 192.168.0.7

static struct tftp_server_t server;
static THRD_STACK(tftp_server_stack, 2048);

int main()
{
    struct inet_addr_t addr;

    sys_start();

    inet_aton(STRINGIFY(IP), &addr.ip);
    addr.port = 69;

    /* Create the TFTP server. */
    if (tftp_server_init(&server,
                         &addr,
                         5000,
                         "tftp_server",
                         tftp_server_stack,
                         sizeof(tftp_server_stack)) != 0) {
        std_printf(FSTR("tftp_server_init() failed\r\n"));
        return (-1);
    }

    if (tftp_server_start(&server) != 0) {
        std_printf(FSTR("tftp_server_start() failed\r\n"));
        return (-1);
    }

    std_printf(FSTR("TFTP server running.\r\n"));

    thrd_suspend(NULL);

    return (0);
}
