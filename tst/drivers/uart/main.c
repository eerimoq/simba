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

static struct uart_driver_t uarts[UART_DEVICE_MAX - 1];
static char rxbufs[UART_DEVICE_MAX - 1][8];

static int test_echo_server(struct harness_t *harness_p)
{
    struct uart_driver_t *uart_p;
    char byte;
    int i;
    struct chan_list_t list;
    void *workspace[UART_DEVICE_MAX];
    
    chan_list_init(&list, workspace, sizeof(workspace));

    /* Initialize and start all UART drivers except the UART used for
       the console. */
    for (i = 0; i < membersof(uarts); i++) {
        uart_init(&uarts[i],
                  &uart_device[i + 1],
                  38400,
                  &rxbufs[i][0],
                  sizeof(rxbufs[0]));
        uart_start(&uarts[i]);
        chan_list_add(&list, &uarts[i]);
    }

    std_printf(FSTR("echo server testing UART 1-%d press 'q' to quit\r\n"),
               UART_DEVICE_MAX - 1);

    while (1) {
        uart_p = chan_list_poll(&list, NULL);
        uart_read(uart_p, &byte, sizeof(byte));

        std_printf(FSTR("Read 0x%x on UART %d.\r\n"),
                   (int)byte,
                   (uart_p - &uarts[0]) / sizeof(uarts[0]) + 1);

        if (byte == 'q') {
            break;
        }

        uart_write(uart_p, &byte, sizeof(byte));
    }

    for (i = 0; i < membersof(uarts); i++) {
        uart_stop(&uarts[i]);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_echo_server, "test_echo_server" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
