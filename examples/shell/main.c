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

FS_COMMAND_DEFINE("/tmp/foo", tmp_foo);

FS_COUNTER_DEFINE("/foo", foo);
FS_COUNTER_DEFINE("/bar", bar);
FS_COUNTER_DEFINE("/fie", fie);

int tmp_foo(int argc,
            const char *argv[],
            void *out_p,
            void *in_p)
{
    UNUSED(in_p);

    if (argc != 4) {
        std_fprintf(out_p, FSTR("3 arguments required.\r\n"));
        return (1);
    }

    std_fprintf(out_p,
                FSTR("argc = %d, argv[0] = %s, argv[1] = %s, argv[2] = %s\r\n"),
                argc,
                argv[0],
                argv[1],
                argv[2]);

    return (0);
}

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_args_t shell_args;

int main()
{
    struct owi_driver_t owi;
    struct ds18b20_driver_t ds;
    struct owi_device_t devices[4];
    
    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    /* Initialize temperature sensor. */
    owi_init(&owi, &pin_d10_dev, devices, membersof(devices));
    ds18b20_init(&ds, &owi);

    FS_COUNTER_INC(foo, 0xfffd);
    FS_COUNTER_INC(foo, 2);
    FS_COUNTER_INC(bar, 339283982393);
    FS_COUNTER_INC(fie, 1);

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_entry(&shell_args);

    return (0);
}
