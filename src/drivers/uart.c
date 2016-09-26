/**
 * @file uart.c
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

#include "uart_port.i"

static int initialized = 0;

int uart_module_init(void)
{
    if (initialized == 1) {
        return (0);
    }

    initialized = 1;

    return (uart_port_module_init());
}

int uart_init(struct uart_driver_t *self_p,
              struct uart_device_t *dev_p,
              long baudrate,
              void *rxbuf_p,
              size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    self_p->dev_p = dev_p;
    self_p->baudrate = baudrate;

    sem_init(&self_p->sem, 0, 1);

    chan_init(&self_p->chout,
              chan_read_null,
              (ssize_t (*)(void *, const void *, size_t))uart_port_write_cb,
              chan_size_null);
    
    if (size > 0) {
        queue_init(&self_p->chin, rxbuf_p, size);
    }

    return (0);
}

int uart_start(struct uart_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (uart_port_start(self_p));
}

int uart_stop(struct uart_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (uart_port_stop(self_p));
}
