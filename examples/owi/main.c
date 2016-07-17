/**
 * @file main.c
 * @version 1.0.0
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

#include "simba.h"

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_t shell;
static struct owi_driver_t owi;
static struct ds18b20_driver_t ds;
static struct owi_device_t devices[4];

int main()
{
    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    owi_init(&owi, &pin_d7_dev, devices, membersof(devices));
    ds18b20_init(&ds, &owi);

    shell_init(&shell, &uart.chin, &uart.chout, NULL, NULL, NULL, NULL);
    shell_main(&shell);

    return (0);
}
