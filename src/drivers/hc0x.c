/**
 * @file drivers/hc0x.c
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

#define BAUDRATE 38400

int hc0x_init(struct hc0x_driver_t *drv_p,
              struct uart_device_t *uart_dev_p,
              struct pin_device_t *key_pin_dev_p,
              void *rxbuf_p,
              size_t size)
{
    uart_init(&drv_p->uart, uart_dev_p, BAUDRATE, rxbuf_p, size);

    pin_init(&drv_p->key, key_pin_dev_p, PIN_OUTPUT);

    return (0);
}

int hc0x_start(struct hc0x_driver_t *drv_p)
{
    return (uart_start(&drv_p->uart));
}

int hc0x_at_command_mode_begin(struct hc0x_driver_t *drv_p)
{
    return (pin_write(&drv_p->key, 1));
}

int hc0x_at_command_mode_end(struct hc0x_driver_t *drv_p)
{
    return (pin_write(&drv_p->key, 0));
}
