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

#ifndef __DRIVERS_UART_PORT_H__
#define __DRIVERS_UART_PORT_H__

#include <io.h>

struct uart_device_t {
    struct uart_driver_t *drv_p;         /* Current started driver. */
    volatile struct esp32_uart_t *regs_p;
    int tx_signal;
    int rx_signal;
    struct pin_device_t *tx_pin_p;
    struct pin_device_t *rx_pin_p;
    struct {
        int source;
        int cpu;
    } interrupt;
};

struct uart_driver_t {
    struct queue_t chin;
    struct uart_device_t *dev_p;
    struct sem_t sem;
    const char *txbuf_p;
    size_t txsize;
    size_t rxsize;
    struct thrd_t *thrd_p;
    long baudrate;
    struct chan_t chout;
};

#endif
