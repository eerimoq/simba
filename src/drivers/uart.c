/**
 * @file uart.c
 * @version 0.2.0
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

int uart_module_init(void)
{
    return (0);
}

int uart_init(struct uart_driver_t *self_p,
              struct uart_device_t *dev_p,
              long baudrate,
              void *rxbuf_p,
              size_t size)
{
    self_p->dev_p = dev_p;
    self_p->baudrate = baudrate;

    sem_init(&self_p->sem, 1);

    chan_init(&self_p->chout,
              NULL,
              (ssize_t (*)(chan_t *, const void *, size_t))uart_port_write_cb,
              NULL);

    return (queue_init(&self_p->chin, rxbuf_p, size));
}

int uart_start(struct uart_driver_t *self_p)
{
    return (uart_port_start(self_p));
}

int uart_stop(struct uart_driver_t *self_p)
{
    return (uart_port_stop(self_p));
}
