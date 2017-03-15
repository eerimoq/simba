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

#ifndef __DRIVERS_UART_H__
#define __DRIVERS_UART_H__

#include "simba.h"

typedef int (*uart_rx_filter_cb_t)(char c);

#include "uart_port.h"

extern struct uart_device_t uart_device[UART_DEVICE_MAX];

/**
 * Initialize UART module. This function must be called before calling
 * any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int uart_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] baudrate Baudrate.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 *
 * @return zero(0) or negative error code.
 */
int uart_init(struct uart_driver_t *self_p,
              struct uart_device_t *dev_p,
              long baudrate,
              void *rxbuf_p,
              size_t size);

/**
 * Set the reception filter callback function.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] rx_filter_cb Callback to set.
 *
 * @return zero(0) or negative error code.
 */
int uart_set_rx_filter_cb(struct uart_driver_t *self_p,
                          uart_rx_filter_cb_t rx_filter_cb);

/**
 * Starts the UART device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int uart_start(struct uart_driver_t *self_p);

/**
 * Stops the UART device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int uart_stop(struct uart_driver_t *self_p);

/**
 * Read data from the UART.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of received bytes or negative error code.
 */
#define uart_read(self_p, buf_p, size)           \
    queue_read(&(self_p)->chin, buf_p, size)

/**
 * Write data to the UART.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
#define uart_write(self_p, buf_p, size)                  \
    (self_p)->chout.write(&(self_p)->chout, buf_p, size)

/**
 * Starts the UART device using given configration. The UART device
 * group of functions does not use interrupts, but instead polls the
 * hardware for events. The driver and device functions may not be
 * used for the same UART device.
 *
 * @param[in] dev_p UART device to start.
 * @param[in] baudrate Baudrate.
 *
 * @return zero(0) or negative error code.
 */
int uart_device_start(struct uart_device_t *dev_p,
                      long baudrate);

/**
 * Stops given UART device.
 *
 * @param[in] dev_p UART device to stop.
 *
 * @return zero(0) or negative error code.
 */
int uart_device_stop(struct uart_device_t *dev_p);

/**
 * Read data from the UART. This function does not wait for
 * interrupts, but instead busy-waits for data by polling UART
 * registers.
 *
 * @param[in] dev_p UART device to read from.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of received bytes or negative error code.
 */
ssize_t uart_device_read(struct uart_device_t *dev_p,
                         void *buf_p,
                         size_t size);

/**
 * Write data to the UART. This function does not wait for interrupts,
 * but instead busy-waits for data by polling UART registers.
 *
 * @param[in] dev_p UART device to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t uart_device_write(struct uart_device_t *dev_p,
                          const void *buf_p,
                          size_t size);

#endif
