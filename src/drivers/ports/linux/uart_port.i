/**
 * @file drivers/sh/uart_port.c
 * @version 0.1.0
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

static int uart_port_start(struct uart_driver_t *drv)
{
    return (0);
}

static int uart_port_stop(struct uart_driver_t *drv)
{
    return (0);
}

static ssize_t uart_port_write_cb(void *arg,
                                  const void *txbuf,
                                  size_t size)
{
    const char *c = txbuf;

    while (size > 0) {
        putchar(*c++);
        size--;
    }

    return (size);
}
