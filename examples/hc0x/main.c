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

static char buf[64];

int main()
{
    char c;
    struct hc0x_driver_t hc0x;

    sys_start();

    uart_module_init();

    /* Start the bloetooth device. */
    hc0x_init(&hc0x, &uart_device[0], &pin_d7_dev, buf, sizeof(buf));
    hc0x_start(&hc0x);

    sys_set_stdout(&hc0x.uart.chout);

    /* Read data and write it back to the sender two times. */
    while (1) {
        if (hc0x_read(&hc0x, &c, sizeof(c)) != sizeof(c)) {
            continue;
        }
        
        hc0x_write(&hc0x, &c, sizeof(c));
        hc0x_write(&hc0x, &c, sizeof(c));
    }
    
    return (0);
}
