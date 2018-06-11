/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "uart_port.h"

/* UART frame formats */
#define UART_FRAME_FORMAT_5N1  UART_PORT_FRAME_FORMAT_5N1
#define UART_FRAME_FORMAT_5N15 UART_PORT_FRAME_FORMAT_5N15
#define UART_FRAME_FORMAT_5N2  UART_PORT_FRAME_FORMAT_5N2
#define UART_FRAME_FORMAT_5O1  UART_PORT_FRAME_FORMAT_5O1
#define UART_FRAME_FORMAT_5O15 UART_PORT_FRAME_FORMAT_5O15
#define UART_FRAME_FORMAT_5O2  UART_PORT_FRAME_FORMAT_5O2
#define UART_FRAME_FORMAT_5E1  UART_PORT_FRAME_FORMAT_5E1
#define UART_FRAME_FORMAT_5E15 UART_PORT_FRAME_FORMAT_5E15
#define UART_FRAME_FORMAT_5E2  UART_PORT_FRAME_FORMAT_5E2
#define UART_FRAME_FORMAT_6N1  UART_PORT_FRAME_FORMAT_6N1
#define UART_FRAME_FORMAT_6N15 UART_PORT_FRAME_FORMAT_6N15
#define UART_FRAME_FORMAT_6N2  UART_PORT_FRAME_FORMAT_6N2
#define UART_FRAME_FORMAT_6O1  UART_PORT_FRAME_FORMAT_6O1
#define UART_FRAME_FORMAT_6O15 UART_PORT_FRAME_FORMAT_6O15
#define UART_FRAME_FORMAT_6O2  UART_PORT_FRAME_FORMAT_6O2
#define UART_FRAME_FORMAT_6E1  UART_PORT_FRAME_FORMAT_6E1
#define UART_FRAME_FORMAT_6E15 UART_PORT_FRAME_FORMAT_6E15
#define UART_FRAME_FORMAT_6E2  UART_PORT_FRAME_FORMAT_6E2
#define UART_FRAME_FORMAT_7N1  UART_PORT_FRAME_FORMAT_7N1
#define UART_FRAME_FORMAT_7N15 UART_PORT_FRAME_FORMAT_7N15
#define UART_FRAME_FORMAT_7N2  UART_PORT_FRAME_FORMAT_7N2
#define UART_FRAME_FORMAT_7O1  UART_PORT_FRAME_FORMAT_7O1
#define UART_FRAME_FORMAT_7O15 UART_PORT_FRAME_FORMAT_7O15
#define UART_FRAME_FORMAT_7O2  UART_PORT_FRAME_FORMAT_7O2
#define UART_FRAME_FORMAT_7E1  UART_PORT_FRAME_FORMAT_7E1
#define UART_FRAME_FORMAT_7E15 UART_PORT_FRAME_FORMAT_7E15
#define UART_FRAME_FORMAT_7E2  UART_PORT_FRAME_FORMAT_7E2
#define UART_FRAME_FORMAT_8N1  UART_PORT_FRAME_FORMAT_8N1
#define UART_FRAME_FORMAT_8N15 UART_PORT_FRAME_FORMAT_8N15
#define UART_FRAME_FORMAT_8N2  UART_PORT_FRAME_FORMAT_8N2
#define UART_FRAME_FORMAT_8O1  UART_PORT_FRAME_FORMAT_8O1
#define UART_FRAME_FORMAT_8O15 UART_PORT_FRAME_FORMAT_8O15
#define UART_FRAME_FORMAT_8O2  UART_PORT_FRAME_FORMAT_8O2
#define UART_FRAME_FORMAT_8E1  UART_PORT_FRAME_FORMAT_8E1
#define UART_FRAME_FORMAT_8E15 UART_PORT_FRAME_FORMAT_8E15
#define UART_FRAME_FORMAT_8E2  UART_PORT_FRAME_FORMAT_8E2
#define UART_FRAME_FORMAT_9N1  UART_PORT_FRAME_FORMAT_9N1
#define UART_FRAME_FORMAT_9N15 UART_PORT_FRAME_FORMAT_9N15
#define UART_FRAME_FORMAT_9N2  UART_PORT_FRAME_FORMAT_9N2
#define UART_FRAME_FORMAT_9O1  UART_PORT_FRAME_FORMAT_9O1
#define UART_FRAME_FORMAT_9O15 UART_PORT_FRAME_FORMAT_9O15
#define UART_FRAME_FORMAT_9O2  UART_PORT_FRAME_FORMAT_9O2
#define UART_FRAME_FORMAT_9E1  UART_PORT_FRAME_FORMAT_9E1
#define UART_FRAME_FORMAT_9E15 UART_PORT_FRAME_FORMAT_9E15
#define UART_FRAME_FORMAT_9E2  UART_PORT_FRAME_FORMAT_9E2
#define UART_FRAME_FORMAT_DEFAULT UART_PORT_FRAME_FORMAT_DEFAULT

typedef int (*uart_rx_filter_cb_t)(char c);

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
 * Set the UART frame format (number of data bits, parity and number of stop
 * bits) in the driver object. This function must be called after uart_init(),
 * and before uart_start().
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] format An UART_FRAME_FORMAT_* constant.
 *
 * @return zero(0) or negative error code.
 */
int uart_set_frame_format(struct uart_driver_t *self_p, int format);

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
    queue_read(&(self_p)->base, buf_p, size)

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
    (self_p)->base.base.write(&(self_p)->base.base, buf_p, size)

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
