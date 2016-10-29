/**
 * @file drivers/sh/uart_port.c
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

static int uart_port_module_init()
{
    return (0);
}

static int uart_port_start(struct uart_driver_t *drv)
{
    return (0);
}

static int uart_port_stop(struct uart_driver_t *drv)
{
    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    const char *c_p = txbuf_p;

    while (size > 0) {
        putc(*c_p++, stdout);
        fflush(stdout);
        size--;
    }

    return (size);
}

static ssize_t uart_port_write_cb_isr(void *arg_p,
                                      const void *txbuf_p,
                                      size_t size)
{
    return (uart_port_write_cb(arg_p, txbuf_p, size));
}
