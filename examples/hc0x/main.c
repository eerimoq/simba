/**
 * @file main.c
 * @version 0.3.0
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

/**
 * Setup a shell on the UART towards the PC
 */

#include "simba.h"

#define EOT '\x04'

static struct fs_command_t cmd_at;

static struct uart_driver_t uart;
static struct uart_soft_driver_t uart_hc0x;

static char qinbuf[32];
static char qinbuf_hc0x[32];

static struct shell_args_t shell_args;

static int cmd_at_cb(int argc,
                     const char *argv[],
                     chan_t *out_p,
                     chan_t *in_p,
                     void *arg_p,
                     void *call_arg_p)
{
    struct chan_list_t list;
    chan_t *chan_p;
    char c;
    char buf[32];

    std_fprintf(out_p, FSTR("type ctrl-d to exit\r\n"));

    /* Wait for data from PC and HC-0X. */
    chan_list_init(&list, buf, sizeof(buf));
    chan_list_add(&list, &uart_hc0x.chin);
    chan_list_add(&list, in_p);

    /* Pass data between PC and bluetooth device. */
    while (1) {
        chan_p = chan_list_poll(&list, NULL);

        if (chan_p == in_p) {
            chan_read(chan_p, &c, sizeof(c));

            /* break if EOT is found, otherwise write to HC-0X. */
            if (c == EOT) {
                break;
            }

            chan_write(&uart_hc0x.chout, &c, sizeof(c));
        } else if (chan_p == &uart_hc0x.chin) {
            /* Write all output from HC-0X to the PC. */
            chan_read(chan_p, &c, sizeof(c));
            chan_write(out_p, &c, sizeof(c));
        } else {
            std_printf(FSTR("bad input channel 0x%02x\r\n"),  (int)chan_p);
        }
    }

    chan_list_destroy(&list);

    return (0);
}

int main()
{
    sys_start();

    uart_module_init();

    uart_init(&uart,
              &uart_device[0],
              38400,
              qinbuf,
              sizeof(qinbuf));

    uart_soft_init(&uart_hc0x,
                   &pin_d4_dev,
                   &pin_d3_dev,
                   &exti_d3_dev,
                   9600,
                   qinbuf_hc0x,
                   sizeof(qinbuf_hc0x));

    fs_command_init(&cmd_at,
                    FSTR("/at"),
                    cmd_at_cb,
                    NULL);
    fs_command_register(&cmd_at);

    sys_set_stdout(&uart.chout);

    uart_start(&uart);

    std_printf(FSTR("Welcome to HC-0X configuration tool!\r\n"
                    "\r\n"
                    "SETUP: Connect pin34 to VCC so the device enters AT mode.\r\n"
                    "\r\n"
                    "Type 'at' to start communicating with the device.\r\n"));

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_main(&shell_args);

    return (0);
}
