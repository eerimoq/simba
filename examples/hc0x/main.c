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

/**
 * Setup a shell on the UART towards the PC
 */

#include "simba.h"

#define EOT '\x04'

FS_COMMAND_DEFINE("/at", cmd_at);
FS_COMMAND_DEFINE("/pipe", cmd_pipe);

static struct uart_driver_t uart;
static struct uart_driver_t uart_hc0x;
static struct hc0x_driver_t hc0x;

static int communicate(void *out_p, void *in_p)
{
    struct chan_list_t list;
    chan_t *chan_p;
    char c;
    char buf[32];

    /* Create channel list channel list. */
    chan_list_init(&list, buf, sizeof(buf));
    chan_list_add(&list, in_p);

    /* Pass data between PC and bluetooth device. */
    while (1) {
        chan_p = chan_list_poll(&list);

        if (chan_p == in_p) {
            chan_read(in_p, &c, sizeof(c));
            
            if (c == EOT) {
                break;
            }
        }
    }

    return (0);
}

int cmd_at(int argc,
           const char *argv[],
           void *out_p,
           void *in_p)
{
    hc0x_at_command_mode_begin(&hc0x);
    communicate(out_p, in_p);
    hc0x_at_command_mode_end(&hc0x);

    return (0);
}

int cmd_pipe(int argc,
             const char *argv[],
             void *out_p,
             void *in_p)
{
    communicate(out_p, in_p);

    return (0);
}

static char qinbuf[32];
static struct shell_args_t shell_args;

int main()
{
    sys_start();

    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);
    sys_set_stdout(&uart.chout);

    /* Start the bloetooth device. */
    hc0x_init(&hc0x, &uart_hc0x, &pin_d7_dev);

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_entry(&shell_args);

    return (0);
}
