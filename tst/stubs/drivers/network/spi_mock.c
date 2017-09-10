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

#include "simba.h"
#include "spi_mock.h"

int mock_write_spi_module_init(int res)
{
    harness_mock_write("spi_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_module_init)()
{
    int res;

    harness_mock_read("spi_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_init(struct spi_device_t *dev_p,
                        struct pin_device_t *ss_pin_p,
                        int mode,
                        int speed,
                        int polarity,
                        int phase,
                        int res)
{
    harness_mock_write("spi_init(dev_p)",
                       dev_p,
                       sizeof(*dev_p));

    harness_mock_write("spi_init(ss_pin_p)",
                       ss_pin_p,
                       sizeof(*ss_pin_p));

    harness_mock_write("spi_init(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("spi_init(speed)",
                       &speed,
                       sizeof(speed));

    harness_mock_write("spi_init(polarity)",
                       &polarity,
                       sizeof(polarity));

    harness_mock_write("spi_init(phase)",
                       &phase,
                       sizeof(phase));

    harness_mock_write("spi_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_init)(struct spi_driver_t *self_p,
                                          struct spi_device_t *dev_p,
                                          struct pin_device_t *ss_pin_p,
                                          int mode,
                                          int speed,
                                          int polarity,
                                          int phase)
{
    int res;

    harness_mock_assert("spi_init(dev_p)",
                        dev_p);

    harness_mock_assert("spi_init(ss_pin_p)",
                        ss_pin_p);

    harness_mock_assert("spi_init(mode)",
                        &mode);

    harness_mock_assert("spi_init(speed)",
                        &speed);

    harness_mock_assert("spi_init(polarity)",
                        &polarity);

    harness_mock_assert("spi_init(phase)",
                        &phase);

    harness_mock_read("spi_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_start(int res)
{
    harness_mock_write("spi_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_start)(struct spi_driver_t *self_p)
{
    int res;

    harness_mock_read("spi_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_stop(int res)
{
    harness_mock_write("spi_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_stop)(struct spi_driver_t *self_p)
{
    int res;

    harness_mock_read("spi_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_take_bus(int res)
{
    harness_mock_write("spi_take_bus(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_take_bus)(struct spi_driver_t *self_p)
{
    int res;

    harness_mock_read("spi_take_bus(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_give_bus(int res)
{
    harness_mock_write("spi_give_bus(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_give_bus)(struct spi_driver_t *self_p)
{
    int res;

    harness_mock_read("spi_give_bus(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_select(int res)
{
    harness_mock_write("spi_select(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_select)(struct spi_driver_t *self_p)
{
    int res;

    harness_mock_read("spi_select(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_deselect(int res)
{
    harness_mock_write("spi_deselect(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(spi_deselect)(struct spi_driver_t *self_p)
{
    int res;

    harness_mock_read("spi_deselect(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_transfer(void *rxbuf_p,
                            const void *txbuf_p,
                            size_t size,
                            ssize_t res)
{
    harness_mock_write("spi_transfer(): return (rxbuf_p)",
                       rxbuf_p,
                       size);

    harness_mock_write("spi_transfer(txbuf_p)",
                       txbuf_p,
                       size);

    harness_mock_write("spi_transfer(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("spi_transfer(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(spi_transfer)(struct spi_driver_t *self_p,
                                                  void *rxbuf_p,
                                                  const void *txbuf_p,
                                                  size_t size)
{
    ssize_t res;

    harness_mock_read("spi_transfer(): return (rxbuf_p)",
                      rxbuf_p,
                      -1);

    harness_mock_assert("spi_transfer(txbuf_p)",
                        txbuf_p);

    harness_mock_assert("spi_transfer(size)",
                        &size);

    harness_mock_read("spi_transfer(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_read(void *buf_p,
                        size_t size,
                        ssize_t res)
{
    harness_mock_write("spi_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("spi_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("spi_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(spi_read)(struct spi_driver_t *self_p,
                                              void *buf_p,
                                              size_t size)
{
    ssize_t res;

    harness_mock_read("spi_read(): return (buf_p)",
                      buf_p,
                      -1);

    harness_mock_assert("spi_read(size)",
                        &size);

    harness_mock_read("spi_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_write(const void *buf_p,
                         size_t size,
                         ssize_t res)
{
    harness_mock_write("spi_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("spi_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("spi_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(spi_write)(struct spi_driver_t *self_p,
                                               const void *buf_p,
                                               size_t size)
{
    ssize_t res;

    harness_mock_assert("spi_write(buf_p)",
                        buf_p);

    harness_mock_assert("spi_write(size)",
                        &size);

    harness_mock_read("spi_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_get(uint8_t *data_p,
                       ssize_t res)
{
    harness_mock_write("spi_get(): return (data_p)",
                       data_p,
                       sizeof(*data_p));

    harness_mock_write("spi_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(spi_get)(struct spi_driver_t *self_p,
                                             uint8_t *data_p)
{
    ssize_t res;

    harness_mock_read("spi_get(): return (data_p)",
                      data_p,
                      -1);

    harness_mock_read("spi_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spi_put(uint8_t data,
                       ssize_t res)
{
    harness_mock_write("spi_put(data)",
                       &data,
                       sizeof(data));

    harness_mock_write("spi_put(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(spi_put)(struct spi_driver_t *self_p,
                                             uint8_t data)
{
    ssize_t res;

    harness_mock_assert("spi_put(data)",
                        &data);

    harness_mock_read("spi_put(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
