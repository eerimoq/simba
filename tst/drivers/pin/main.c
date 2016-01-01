/**
 * @file main.c
 * @version 0.1.0
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

static struct pin_driver_t red;
static struct pin_driver_t green;
static struct pin_driver_t blue;
static struct pin_driver_t white;
struct uart_driver_t uart;

int main()
{
    int i;

    sys_start();
    pin_module_init();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&uart);

    pin_init(&red, &pin_led_dev, PIN_OUTPUT);
    pin_init(&green, &pin_d9_dev, PIN_OUTPUT);
    pin_init(&blue, &pin_d6_dev, PIN_OUTPUT);
    pin_init(&white, &pin_d3_dev, PIN_OUTPUT);

    uart_write(&uart, "Hello world!", sizeof("Hello world!") - 1);

    i = 0;

    while (1) {
        thrd_usleep(500000);
        pin_write(&red, (i & 0x1) != 0);
        pin_write(&green, (i & 0x2) != 0);
        pin_write(&blue, (i & 0x4) != 0);
        pin_write(&white, (i & 0x8) != 0);
        i++;
    }

    return (0);
}
