/**
 * @file drivers/uart.h
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

#ifndef __DRIVERS_UART_H__
#define __DRIVERS_UART_H__

#include "simba.h"

#include "uart_port.h"

extern struct uart_device_t uart_device[UART_DEVICE_MAX];

/**
 * Initialize UART module.
 */
int uart_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] drv_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] baudrate Baudrate.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 *
 * @return zero(0) or negative error code.
 */
int uart_init(struct uart_driver_t *drv_p,
              struct uart_device_t *dev_p,
              long baudrate,
              void *rxbuf_p,
              size_t size);

/**
 * Starts the UART device using given driver object.
 *
 * @param[in] drv_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int uart_start(struct uart_driver_t *drv_p);

/**
 * Stops the UART device referenced by driver object.
 *
 * @param[in] drv_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int uart_stop(struct uart_driver_t *drv_p);

/**
 * Read data from the UART.
 *
 * @param[in] drv_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of received bytes or negative error code.
 */
#define uart_read(drv_p, buf_p, size)           \
    queue_read(&(drv_p)->chin, buf_p, size)

/**
 * Write data to the UART.
 *
 * @param[in] drv_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return number of sent bytes or negative error code.
 */
#define uart_write(drv_p, buf_p, size)                  \
    (drv_p)->chout.write(&(drv_p)->chout, buf_p, size)

#endif
