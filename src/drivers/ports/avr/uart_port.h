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

#define UART_PORT_FRAME_FORMAT_5N1 0x00
#define UART_PORT_FRAME_FORMAT_6N1 0x02
#define UART_PORT_FRAME_FORMAT_7N1 0x04
#define UART_PORT_FRAME_FORMAT_8N1 0x06
#define UART_PORT_FRAME_FORMAT_5N2 0x08
#define UART_PORT_FRAME_FORMAT_6N2 0x0A
#define UART_PORT_FRAME_FORMAT_7N2 0x0C
#define UART_PORT_FRAME_FORMAT_8N2 0x0E
#define UART_PORT_FRAME_FORMAT_5E1 0x20
#define UART_PORT_FRAME_FORMAT_6E1 0x22
#define UART_PORT_FRAME_FORMAT_7E1 0x24
#define UART_PORT_FRAME_FORMAT_8E1 0x26
#define UART_PORT_FRAME_FORMAT_5E2 0x28
#define UART_PORT_FRAME_FORMAT_6E2 0x2A
#define UART_PORT_FRAME_FORMAT_7E2 0x2C
#define UART_PORT_FRAME_FORMAT_8E2 0x2E
#define UART_PORT_FRAME_FORMAT_5O1 0x30
#define UART_PORT_FRAME_FORMAT_6O1 0x32
#define UART_PORT_FRAME_FORMAT_7O1 0x34
#define UART_PORT_FRAME_FORMAT_8O1 0x36
#define UART_PORT_FRAME_FORMAT_5O2 0x38
#define UART_PORT_FRAME_FORMAT_6O2 0x3A
#define UART_PORT_FRAME_FORMAT_7O2 0x3C
#define UART_PORT_FRAME_FORMAT_8O2 0x3E

#define UART_PORT_FRAME_FORMAT_DEFAULT UART_PORT_FRAME_FORMAT_8N1

struct uart_device_t {
    struct uart_driver_t *drv_p;         /* Current started driver. */
    volatile uint8_t *sfr_p;
};

struct uart_driver_t {
    struct queue_t base;
    struct uart_device_t *dev_p;
    struct mutex_t mutex;
    const char *txbuf_p;
    size_t txsize;
    struct thrd_t *thrd_p;
    long baudrate;
    int format;
};

#endif
