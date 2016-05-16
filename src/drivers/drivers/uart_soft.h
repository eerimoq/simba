/**
 * @file drivers/uart_soft.h
 * @version 0.4.0
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

#ifndef __DRIVERS_UART_SOFT_H__
#define __DRIVERS_UART_SOFT_H__

#include "simba.h"

struct uart_soft_driver_t {
    struct pin_driver_t tx_pin;
    struct pin_driver_t rx_pin;
    struct exti_driver_t rx_exti;
    struct chan_t chout;
    struct queue_t chin;
    int sample_time;
    int baudrate;
};

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] tx_dev_p TX pin device.
 * @param[in] rx_dev_p RX pin device.
 * @param[in] rx_exti_dev_p RX pin external interrupt device.
 * @param[in] baudrate Baudrate.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 *
 * @return zero(0) or negative error code.
 */
int uart_soft_init(struct uart_soft_driver_t *self_p,
                   struct pin_device_t *tx_dev_p,
                   struct pin_device_t *rx_dev_p,
                   struct exti_device_t *rx_exti_dev_p,
                   int baudrate,
                   void *rxbuf_p,
                   size_t size);

/**
 * Read data from the UART.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of received bytes or negative error code.
 */
#define uart_soft_read(self_p, buf_p, size)      \
    queue_read(&(self_p)->chin, buf_p, size)

/**
 * Write data to the UART.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return number of sent bytes or negative error code.
 */
#define uart_soft_write(self_p, buf_p, size)             \
    (self_p)->chout.write(&(self_p)->chout, buf_p, size)

#endif
