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

/*
 * ESP32 supports 5/6/7/8 data bits, N/E/O parity, 1/1.5 stop bits
 */
#define ESP32_UART_DATA_BITS_5  (0x00 << ESP32_UART_CONF0_BIT_NUM_POS)
#define ESP32_UART_DATA_BITS_6  (0x01 << ESP32_UART_CONF0_BIT_NUM_POS)
#define ESP32_UART_DATA_BITS_7  (0x02 << ESP32_UART_CONF0_BIT_NUM_POS)
#define ESP32_UART_DATA_BITS_8  (0x03 << ESP32_UART_CONF0_BIT_NUM_POS)
#define ESP32_UART_PARITY_N     (0x00 << ESP32_UART_CONF0_PARITY_POS)
#define ESP32_UART_PARITY_E     (0x02 << ESP32_UART_CONF0_PARITY_POS)
#define ESP32_UART_PARITY_O     (0x03 << ESP32_UART_CONF0_PARITY_POS)
#define ESP32_UART_STOP_BITS_1  (0x01 << ESP32_UART_CONF0_STOP_BIT_NUM_POS)
#define ESP32_UART_STOP_BITS_15 (0x02 << ESP32_UART_CONF0_STOP_BIT_NUM_POS)
#define UART_PORT_FRAME_FORMAT_5N1   (ESP32_UART_DATA_BITS_5 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_5N15  (ESP32_UART_DATA_BITS_5 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_5O1   (ESP32_UART_DATA_BITS_5 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_5O15  (ESP32_UART_DATA_BITS_5 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_5E1   (ESP32_UART_DATA_BITS_5 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_5E15  (ESP32_UART_DATA_BITS_5 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_6N1   (ESP32_UART_DATA_BITS_6 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_6N15  (ESP32_UART_DATA_BITS_6 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_6O1   (ESP32_UART_DATA_BITS_6 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_6O15  (ESP32_UART_DATA_BITS_6 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_6E1   (ESP32_UART_DATA_BITS_6 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_6E15  (ESP32_UART_DATA_BITS_6 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_7N1   (ESP32_UART_DATA_BITS_7 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_7N15  (ESP32_UART_DATA_BITS_7 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_7O1   (ESP32_UART_DATA_BITS_7 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_7O15  (ESP32_UART_DATA_BITS_7 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_7E1   (ESP32_UART_DATA_BITS_7 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_7E15  (ESP32_UART_DATA_BITS_7 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_8N1   (ESP32_UART_DATA_BITS_8 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_8N15  (ESP32_UART_DATA_BITS_8 \
        | ESP32_UART_PARITY_N | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_8O1   (ESP32_UART_DATA_BITS_8 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_8O15  (ESP32_UART_DATA_BITS_8 \
        | ESP32_UART_PARITY_O | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_8E1   (ESP32_UART_DATA_BITS_8 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_8E15  (ESP32_UART_DATA_BITS_8 \
        | ESP32_UART_PARITY_E | ESP32_UART_STOP_BITS_15)
#define UART_PORT_FRAME_FORMAT_DEFAULT UART_PORT_FRAME_FORMAT_8N1

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
