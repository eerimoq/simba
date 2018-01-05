/**
 * @section License
 *
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

#include "simba.h"
#include "uart_mock.h"

int mock_write_uart_module_init(int res)
{
    harness_mock_write("uart_module_init()",
                       NULL,
                       0);

    harness_mock_write("uart_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_module_init)()
{
    int res;

    harness_mock_assert("uart_module_init()",
                        NULL,
                        0);

    harness_mock_read("uart_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_init(struct uart_device_t *dev_p,
                         long baudrate,
                         void *rxbuf_p,
                         size_t size,
                         int res)
{
    harness_mock_write("uart_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("uart_init(baudrate)",
                       &baudrate,
                       sizeof(baudrate));

    harness_mock_write("uart_init(rxbuf_p)",
                       rxbuf_p,
                       size);

    harness_mock_write("uart_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("uart_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_init)(struct uart_driver_t *self_p,
                                           struct uart_device_t *dev_p,
                                           long baudrate,
                                           void *rxbuf_p,
                                           size_t size)
{
    int res;

    harness_mock_assert("uart_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("uart_init(baudrate)",
                        &baudrate,
                        sizeof(baudrate));

    harness_mock_assert("uart_init(rxbuf_p)",
                        rxbuf_p,
                        size);

    harness_mock_assert("uart_init(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("uart_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_set_rx_filter_cb(uart_rx_filter_cb_t rx_filter_cb,
                                     int res)
{
    harness_mock_write("uart_set_rx_filter_cb(rx_filter_cb)",
                       &rx_filter_cb,
                       sizeof(rx_filter_cb));

    harness_mock_write("uart_set_rx_filter_cb(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_set_rx_filter_cb)(struct uart_driver_t *self_p,
                                                       uart_rx_filter_cb_t rx_filter_cb)
{
    int res;

    harness_mock_assert("uart_set_rx_filter_cb(rx_filter_cb)",
                        &rx_filter_cb,
                        sizeof(rx_filter_cb));

    harness_mock_read("uart_set_rx_filter_cb(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_start(int res)
{
    harness_mock_write("uart_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_start)(struct uart_driver_t *self_p)
{
    int res;

    harness_mock_read("uart_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_stop(int res)
{
    harness_mock_write("uart_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_stop)(struct uart_driver_t *self_p)
{
    int res;

    harness_mock_read("uart_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_set_frame_format(int format,
                                     int res)
{
    harness_mock_write("uart_set_frame_format(format)",
                       &format,
                       sizeof(format));

    harness_mock_write("uart_set_frame_format(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_set_frame_format)(struct uart_driver_t *self_p,
                                                       int format)
{
    int res;

    harness_mock_assert("uart_set_frame_format(format)",
                        &format,
                        sizeof(format));

    harness_mock_read("uart_set_frame_format(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_device_start(struct uart_device_t *dev_p,
                                 long baudrate,
                                 int res)
{
    harness_mock_write("uart_device_start(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("uart_device_start(baudrate)",
                       &baudrate,
                       sizeof(baudrate));

    harness_mock_write("uart_device_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_device_start)(struct uart_device_t *dev_p,
                                                   long baudrate)
{
    int res;

    harness_mock_assert("uart_device_start(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("uart_device_start(baudrate)",
                        &baudrate,
                        sizeof(baudrate));

    harness_mock_read("uart_device_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_device_stop(struct uart_device_t *dev_p,
                                int res)
{
    harness_mock_write("uart_device_stop(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("uart_device_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_device_stop)(struct uart_device_t *dev_p)
{
    int res;

    harness_mock_assert("uart_device_stop(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_read("uart_device_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_device_read(struct uart_device_t *dev_p,
                                void *buf_p,
                                size_t size,
                                ssize_t res)
{
    harness_mock_write("uart_device_read(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("uart_device_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("uart_device_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("uart_device_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(uart_device_read)(struct uart_device_t *dev_p,
                                                      void *buf_p,
                                                      size_t size)
{
    ssize_t res;

    harness_mock_assert("uart_device_read(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("uart_device_read(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("uart_device_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("uart_device_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_uart_device_write(struct uart_device_t *dev_p,
                                 const void *buf_p,
                                 size_t size,
                                 ssize_t res)
{
    harness_mock_write("uart_device_write(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("uart_device_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("uart_device_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("uart_device_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(uart_device_write)(struct uart_device_t *dev_p,
                                                       const void *buf_p,
                                                       size_t size)
{
    ssize_t res;

    harness_mock_assert("uart_device_write(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("uart_device_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("uart_device_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("uart_device_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
