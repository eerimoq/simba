/**
 * @file drivers/ports/esp/uart_port.h
 * @version 5.0.0
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
    struct uart_driver_t *drv_p;         /* Current started driver. */
    volatile struct esp8266_uart_t *regs_p;
};

struct uart_driver_t {
    struct uart_device_t *dev_p;
    struct sem_t sem;
    const char *txbuf_p;
    size_t txsize;
    size_t rxsize;
    struct thrd_t *thrd_p;
    long baudrate;
    struct chan_t chout;
    struct queue_t chin;
};

#endif
