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

#ifndef __UART_MOCK_H__
#define __UART_MOCK_H__

#include "simba.h"

int mock_write_uart_module_init(int res);

int mock_write_uart_init(struct uart_device_t *dev_p,
                         long baudrate,
                         void *rxbuf_p,
                         size_t size,
                         int res);

int mock_write_uart_set_rx_filter_cb(uart_rx_filter_cb_t rx_filter_cb,
                                     int res);

int mock_write_uart_start(int res);

int mock_write_uart_stop(int res);

int mock_write_uart_set_frame_format(int format,
                                     int res);

int mock_write_uart_device_start(struct uart_device_t *dev_p,
                                 long baudrate,
                                 int res);

int mock_write_uart_device_stop(struct uart_device_t *dev_p,
                                int res);

int mock_write_uart_device_read(struct uart_device_t *dev_p,
                                void *buf_p,
                                size_t size,
                                ssize_t res);

int mock_write_uart_device_write(struct uart_device_t *dev_p,
                                 const void *buf_p,
                                 size_t size,
                                 ssize_t res);

#endif
