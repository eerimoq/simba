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
#include "nrf24l01_mock.h"

int mock_write_nrf24l01_module_init(int res)
{
    harness_mock_write("nrf24l01_module_init()",
                       NULL,
                       0);

    harness_mock_write("nrf24l01_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nrf24l01_module_init)()
{
    int res;

    harness_mock_assert("nrf24l01_module_init()",
                        NULL,
                        0);

    harness_mock_read("nrf24l01_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nrf24l01_init(struct spi_device_t *spi_p,
                             struct pin_device_t *cs_p,
                             struct pin_device_t *ce_p,
                             struct exti_device_t *exti_p,
                             uint32_t address,
                             int res)
{
    harness_mock_write("nrf24l01_init(spi_p)",
                       spi_p,
                       sizeof(*spi_p));

    harness_mock_write("nrf24l01_init(cs_p)",
                       cs_p,
                       sizeof(*cs_p));

    harness_mock_write("nrf24l01_init(ce_p)",
                       ce_p,
                       sizeof(*ce_p));

    harness_mock_write("nrf24l01_init(exti_p)",
                       exti_p,
                       sizeof(*exti_p));

    harness_mock_write("nrf24l01_init(address)",
                       &address,
                       sizeof(address));

    harness_mock_write("nrf24l01_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nrf24l01_init)(struct nrf24l01_driver_t *self_p,
                                               struct spi_device_t *spi_p,
                                               struct pin_device_t *cs_p,
                                               struct pin_device_t *ce_p,
                                               struct exti_device_t *exti_p,
                                               uint32_t address)
{
    int res;

    harness_mock_assert("nrf24l01_init(spi_p)",
                        spi_p,
                        sizeof(*spi_p));

    harness_mock_assert("nrf24l01_init(cs_p)",
                        cs_p,
                        sizeof(*cs_p));

    harness_mock_assert("nrf24l01_init(ce_p)",
                        ce_p,
                        sizeof(*ce_p));

    harness_mock_assert("nrf24l01_init(exti_p)",
                        exti_p,
                        sizeof(*exti_p));

    harness_mock_assert("nrf24l01_init(address)",
                        &address,
                        sizeof(address));

    harness_mock_read("nrf24l01_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nrf24l01_start(int res)
{
    harness_mock_write("nrf24l01_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nrf24l01_start)(struct nrf24l01_driver_t *self_p)
{
    int res;

    harness_mock_read("nrf24l01_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nrf24l01_stop(int res)
{
    harness_mock_write("nrf24l01_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nrf24l01_stop)(struct nrf24l01_driver_t *self_p)
{
    int res;

    harness_mock_read("nrf24l01_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nrf24l01_read(void *buf_p,
                             size_t size,
                             ssize_t res)
{
    harness_mock_write("nrf24l01_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("nrf24l01_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("nrf24l01_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(nrf24l01_read)(struct nrf24l01_driver_t *self_p,
                                                   void *buf_p,
                                                   size_t size)
{
    ssize_t res;

    harness_mock_assert("nrf24l01_read(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("nrf24l01_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("nrf24l01_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nrf24l01_write(uint32_t address,
                              uint8_t pipe,
                              const void *buf_p,
                              size_t size,
                              ssize_t res)
{
    harness_mock_write("nrf24l01_write(address)",
                       &address,
                       sizeof(address));

    harness_mock_write("nrf24l01_write(pipe)",
                       &pipe,
                       sizeof(pipe));

    harness_mock_write("nrf24l01_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("nrf24l01_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("nrf24l01_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(nrf24l01_write)(struct nrf24l01_driver_t *self_p,
                                                    uint32_t address,
                                                    uint8_t pipe,
                                                    const void *buf_p,
                                                    size_t size)
{
    ssize_t res;

    harness_mock_assert("nrf24l01_write(address)",
                        &address,
                        sizeof(address));

    harness_mock_assert("nrf24l01_write(pipe)",
                        &pipe,
                        sizeof(pipe));

    harness_mock_assert("nrf24l01_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("nrf24l01_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("nrf24l01_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
