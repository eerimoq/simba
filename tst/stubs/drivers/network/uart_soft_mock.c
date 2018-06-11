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

#include "simba.h"
#include "uart_soft_mock.h"

int mock_write_uart_soft_init(struct pin_device_t *tx_dev_p,
                              struct pin_device_t *rx_dev_p,
                              struct exti_device_t *rx_exti_dev_p,
                              int baudrate,
                              void *rxbuf_p,
                              size_t size,
                              int res)
{
    harness_mock_write("uart_soft_init(tx_dev_p)",
                       tx_dev_p,
                       sizeof(*tx_dev_p));

    harness_mock_write("uart_soft_init(rx_dev_p)",
                       rx_dev_p,
                       sizeof(*rx_dev_p));

    harness_mock_write("uart_soft_init(rx_exti_dev_p)",
                       rx_exti_dev_p,
                       sizeof(*rx_exti_dev_p));

    harness_mock_write("uart_soft_init(baudrate)",
                       &baudrate,
                       sizeof(baudrate));

    harness_mock_write("uart_soft_init(rxbuf_p)",
                       rxbuf_p,
                       size);

    harness_mock_write("uart_soft_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("uart_soft_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(uart_soft_init)(struct uart_soft_driver_t *self_p,
                                                struct pin_device_t *tx_dev_p,
                                                struct pin_device_t *rx_dev_p,
                                                struct exti_device_t *rx_exti_dev_p,
                                                int baudrate,
                                                void *rxbuf_p,
                                                size_t size)
{
    int res;

    harness_mock_assert("uart_soft_init(tx_dev_p)",
                        tx_dev_p,
                        sizeof(*tx_dev_p));

    harness_mock_assert("uart_soft_init(rx_dev_p)",
                        rx_dev_p,
                        sizeof(*rx_dev_p));

    harness_mock_assert("uart_soft_init(rx_exti_dev_p)",
                        rx_exti_dev_p,
                        sizeof(*rx_exti_dev_p));

    harness_mock_assert("uart_soft_init(baudrate)",
                        &baudrate,
                        sizeof(baudrate));

    harness_mock_assert("uart_soft_init(rxbuf_p)",
                        rxbuf_p,
                        size);

    harness_mock_assert("uart_soft_init(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("uart_soft_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
