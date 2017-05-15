/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_SOCKET_DEVICE_H__
#define __DRIVERS_SOCKET_DEVICE_H__

#include "simba.h"

/**
 * Initialize the socket device module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code
 */
int socket_device_module_init(void);

/**
 * Check if a client is connected for given UART device.
 *
 * @param[in] dev_p UART device.
 *
 * @return true(1) if a client is connected, otherwise false(0).
 */
int socket_device_is_uart_device_connected_isr(
    const struct uart_device_t *dev_p);

/**
 * Write data to given UART device.
 *
 * @param[in] dev_p UART device.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written, or negative error code.
 */
ssize_t socket_device_uart_device_write_isr(
    const struct uart_device_t *dev_p,
    const void *buf_p,
    size_t size);

/**
 * Check if a client is connected for given pin device.
 *
 * @param[in] dev_p Pin device.
 *
 * @return true(1) if a client is connected, otherwise false(0).
 */
int socket_device_is_pin_device_connected_isr(
    const struct pin_device_t *dev_p);

/**
 * Write data to given pin device.
 *
 * @param[in] dev_p Pin device.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written, or negative error code.
 */
ssize_t socket_device_pin_device_write_isr(const struct pin_device_t *dev_p,
                                           const void *buf_p,
                                           size_t size);

/**
 * Check if a client is connected for given pwm device.
 *
 * @param[in] dev_p Pwm device.
 *
 * @return true(1) if a client is connected, otherwise false(0).
 */
int socket_device_is_pwm_device_connected_isr(
    const struct pwm_device_t *dev_p);

/**
 * Write data to given pwm device.
 *
 * @param[in] dev_p Pwm device.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written, or negative error code.
 */
ssize_t socket_device_pwm_device_write_isr(const struct pwm_device_t *dev_p,
                                           const void *buf_p,
                                           size_t size);

/**
 * Check if a client is connected for given can device.
 *
 * @param[in] dev_p Can device.
 *
 * @return true(1) if a client is connected, otherwise false(0).
 */
int socket_device_is_can_device_connected_isr(
    const struct can_device_t *dev_p);

/**
 * Write data to given can device.
 *
 * @param[in] dev_p Can device.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written, or negative error code.
 */
ssize_t socket_device_can_device_write_isr(const struct can_device_t *dev_p,
                                           const void *buf_p,
                                           size_t size);

/**
 * Check if a client is connected for given i2c device.
 *
 * @param[in] dev_p I2c device.
 *
 * @return true(1) if a client is connected, otherwise false(0).
 */
int socket_device_is_i2c_device_connected_isr(
    const struct i2c_device_t *dev_p);

/**
 * Write data to given i2c device.
 *
 * @param[in] dev_p I2c device.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written, or negative error code.
 */
ssize_t socket_device_i2c_device_write_isr(const struct i2c_device_t *dev_p,
                                           int address,
                                           const void *buf_p,
                                           size_t size);

#endif
