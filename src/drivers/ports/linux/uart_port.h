/**
 * @file drivers/uart_port.h
 * @version 1.1.0
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

#ifndef __DRIVERS_UART_PORT_H__
#define __DRIVERS_UART_PORT_H__

#include <io.h>

struct uart_device_t {
    volatile struct st_scif4567 *port_p;
};

struct uart_driver_t {
    const struct uart_device_t *dev_p;
    struct sem_t sem;
    long baudrate;
    size_t rxsize;
    struct chan_t chout;
    struct queue_t chin;
};

#endif
