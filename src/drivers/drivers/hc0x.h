/**
 * @file drivers/hc0x.h
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

#ifndef __DRIVERS_HC0X_H__
#define __DRIVERS_HC0X_H__

#include "simba.h"

struct hc0x_driver_t {
    struct uart_driver_t uart;
    struct pin_driver_t key;
};

/**
 * Initialize driver object from given config.
 * @param[out] drv_p Driver object to be initialized.
 * @param[in] uart_dev_p UART device.
 * @param[in] key_pin_dev_p Pin device for enabling AT mode.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 * @return zero(0) or negative error code.
 */
int hc0x_init(struct hc0x_driver_t *drv_p,
              struct uart_device_t *uart_dev_p,
              struct pin_device_t *key_pin_dev_p,
              void *rxbuf_p,
              size_t size);

/**
 * Start the device.
 * @param[in] drv_p Initialized driver object.
 * @return zero(0) or negative error code.
 */
int hc0x_start(struct hc0x_driver_t *drv_p);

/**
 * Enter AT command mode.
 * @param[in] drv_p Initialized driver object.
 * @return zero(0) or negative error code.
 */
int hc0x_at_command_mode_begin(struct hc0x_driver_t *drv_p);

/**
 * Exit AT command mode.
 * @param[in] drv_p Initialized driver object.
 * @return zero(0) or negative error code.
 */
int hc0x_at_command_mode_end(struct hc0x_driver_t *drv_p);

/**
 * Read data from the HC-0X device.
 * @param[in] drv_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 * @return Number of received bytes or negative error code.
 */
#define hc0x_read(drv_p, buf_p, size)           \
    uart_read(&(drv_p)->uart, buf_p, size)

/**
 * Write data to the HC-0X device.
 * @param[in] drv_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 * @return number of sent bytes or negative error code.
 */
#define hc0x_write(drv_p, buf_p, size)          \
    uart_write(&(drv_p)->uart, buf_p, size)

#endif
