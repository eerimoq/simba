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
#include "mcp2515_mock.h"

int mock_write_mcp2515_init(struct spi_device_t *spi_p,
                            struct pin_device_t *cs_p,
                            struct exti_device_t *exti_p,
                            void *chin_p,
                            int mode,
                            int speed,
                            int res)
{
    harness_mock_write("mcp2515_init(spi_p)",
                       spi_p,
                       sizeof(*spi_p));

    harness_mock_write("mcp2515_init(cs_p)",
                       cs_p,
                       sizeof(*cs_p));

    harness_mock_write("mcp2515_init(exti_p)",
                       exti_p,
                       sizeof(*exti_p));

    harness_mock_write("mcp2515_init(chin_p)",
                       chin_p,
                       sizeof(chin_p));

    harness_mock_write("mcp2515_init(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("mcp2515_init(speed)",
                       &speed,
                       sizeof(speed));

    harness_mock_write("mcp2515_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mcp2515_init)(struct mcp2515_driver_t *self_p,
                                              struct spi_device_t *spi_p,
                                              struct pin_device_t *cs_p,
                                              struct exti_device_t *exti_p,
                                              void *chin_p,
                                              int mode,
                                              int speed)
{
    int res;

    harness_mock_assert("mcp2515_init(spi_p)",
                        spi_p,
                        sizeof(*spi_p));

    harness_mock_assert("mcp2515_init(cs_p)",
                        cs_p,
                        sizeof(*cs_p));

    harness_mock_assert("mcp2515_init(exti_p)",
                        exti_p,
                        sizeof(*exti_p));

    harness_mock_assert("mcp2515_init(chin_p)",
                        chin_p,
                        sizeof(*chin_p));

    harness_mock_assert("mcp2515_init(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_assert("mcp2515_init(speed)",
                        &speed,
                        sizeof(speed));

    harness_mock_read("mcp2515_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mcp2515_start(int res)
{
    harness_mock_write("mcp2515_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mcp2515_start)(struct mcp2515_driver_t *self_p)
{
    int res;

    harness_mock_read("mcp2515_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mcp2515_stop(int res)
{
    harness_mock_write("mcp2515_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mcp2515_stop)(struct mcp2515_driver_t *self_p)
{
    int res;

    harness_mock_read("mcp2515_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mcp2515_read(struct mcp2515_frame_t *frame_p,
                            ssize_t res)
{
    harness_mock_write("mcp2515_read(): return (frame_p)",
                       frame_p,
                       sizeof(*frame_p));

    harness_mock_write("mcp2515_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(mcp2515_read)(struct mcp2515_driver_t *self_p,
                                                  struct mcp2515_frame_t *frame_p)
{
    ssize_t res;

    harness_mock_read("mcp2515_read(): return (frame_p)",
                      frame_p,
                      sizeof(*frame_p));

    harness_mock_read("mcp2515_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mcp2515_write(const struct mcp2515_frame_t *frame_p,
                             ssize_t res)
{
    harness_mock_write("mcp2515_write(): return (frame_p)",
                       frame_p,
                       sizeof(*frame_p));

    harness_mock_write("mcp2515_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(mcp2515_write)(struct mcp2515_driver_t *self_p,
                                                   const struct mcp2515_frame_t *frame_p)
{
    ssize_t res;

    harness_mock_read("mcp2515_write(): return (frame_p)",
                      frame_p,
                      sizeof(*frame_p));

    harness_mock_read("mcp2515_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
