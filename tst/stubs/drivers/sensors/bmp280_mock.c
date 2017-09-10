/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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
#include "bmp280_mock.h"

int mock_write_bmp280_module_init(int res)
{
    harness_mock_write("bmp280_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_module_init)()
{
    int res;

    harness_mock_read("bmp280_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bmp280_init(struct bmp280_transport_t *transport_p,
                           enum bmp280_mode_t mode,
                           enum bmp280_standby_time_t standby_time,
                           enum bmp280_filter_t filter,
                           enum bmp280_temperature_oversampling_t temperature_oversampling,
                           enum bmp280_pressure_oversampling_t pressure_oversampling,
                           int res)
{
    harness_mock_write("bmp280_init(transport_p)",
                       transport_p,
                       sizeof(*transport_p));

    harness_mock_write("bmp280_init(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("bmp280_init(standby_time)",
                       &standby_time,
                       sizeof(standby_time));

    harness_mock_write("bmp280_init(filter)",
                       &filter,
                       sizeof(filter));

    harness_mock_write("bmp280_init(temperature_oversampling)",
                       &temperature_oversampling,
                       sizeof(temperature_oversampling));

    harness_mock_write("bmp280_init(pressure_oversampling)",
                       &pressure_oversampling,
                       sizeof(pressure_oversampling));

    harness_mock_write("bmp280_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_init)(struct bmp280_driver_t *self_p,
                                             struct bmp280_transport_t *transport_p,
                                             enum bmp280_mode_t mode,
                                             enum bmp280_standby_time_t standby_time,
                                             enum bmp280_filter_t filter,
                                             enum bmp280_temperature_oversampling_t temperature_oversampling,
                                             enum bmp280_pressure_oversampling_t pressure_oversampling)
{
    int res;

    harness_mock_assert("bmp280_init(transport_p)",
                        transport_p);

    harness_mock_assert("bmp280_init(mode)",
                        &mode);

    harness_mock_assert("bmp280_init(standby_time)",
                        &standby_time);

    harness_mock_assert("bmp280_init(filter)",
                        &filter);

    harness_mock_assert("bmp280_init(temperature_oversampling)",
                        &temperature_oversampling);

    harness_mock_assert("bmp280_init(pressure_oversampling)",
                        &pressure_oversampling);

    harness_mock_read("bmp280_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bmp280_start(int res)
{
    harness_mock_write("bmp280_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_start)(struct bmp280_driver_t *self_p)
{
    int res;

    harness_mock_read("bmp280_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bmp280_stop(int res)
{
    harness_mock_write("bmp280_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_stop)(struct bmp280_driver_t *self_p)
{
    int res;

    harness_mock_read("bmp280_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bmp280_read(float *temperature_p,
                           float *pressure_p,
                           int res)
{
    harness_mock_write("bmp280_read(): return (temperature_p)",
                       temperature_p,
                       sizeof(*temperature_p));

    harness_mock_write("bmp280_read(): return (pressure_p)",
                       pressure_p,
                       sizeof(*pressure_p));

    harness_mock_write("bmp280_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_read)(struct bmp280_driver_t *self_p,
                                             float *temperature_p,
                                             float *pressure_p)
{
    int res;

    harness_mock_read("bmp280_read(): return (temperature_p)",
                      temperature_p,
                      -1);

    harness_mock_read("bmp280_read(): return (pressure_p)",
                      pressure_p,
                      -1);

    harness_mock_read("bmp280_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bmp280_read_fixed_point(long *temperature_p,
                                       long *pressure_p,
                                       int res)
{
    harness_mock_write("bmp280_read_fixed_point(): return (temperature_p)",
                       temperature_p,
                       sizeof(*temperature_p));

    harness_mock_write("bmp280_read_fixed_point(): return (pressure_p)",
                       pressure_p,
                       sizeof(*pressure_p));

    harness_mock_write("bmp280_read_fixed_point(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_read_fixed_point)(struct bmp280_driver_t *self_p,
                                                         long *temperature_p,
                                                         long *pressure_p)
{
    int res;

    harness_mock_read("bmp280_read_fixed_point(): return (temperature_p)",
                      temperature_p,
                      -1);

    harness_mock_read("bmp280_read_fixed_point(): return (pressure_p)",
                      pressure_p,
                      -1);

    harness_mock_read("bmp280_read_fixed_point(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bmp280_transport_i2c_init(struct i2c_driver_t *i2c_p,
                                         int i2c_address,
                                         int res)
{
    harness_mock_write("bmp280_transport_i2c_init(): return (i2c_p)",
                       i2c_p,
                       sizeof(*i2c_p));

    harness_mock_write("bmp280_transport_i2c_init(i2c_address)",
                       &i2c_address,
                       sizeof(i2c_address));

    harness_mock_write("bmp280_transport_i2c_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_transport_i2c_init)(struct bmp280_transport_i2c_t *self_p,
                                                           struct i2c_driver_t *i2c_p,
                                                           int i2c_address)
{
    int res;

    harness_mock_read("bmp280_transport_i2c_init(): return (i2c_p)",
                      i2c_p,
                      -1);

    harness_mock_assert("bmp280_transport_i2c_init(i2c_address)",
                        &i2c_address);

    harness_mock_read("bmp280_transport_i2c_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_bmp280_transport_spi_init(struct spi_driver_t *spi_p,
                                         int res)
{
    harness_mock_write("bmp280_transport_spi_init(spi_p)",
                       spi_p,
                       sizeof(*spi_p));

    harness_mock_write("bmp280_transport_spi_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(bmp280_transport_spi_init)(struct bmp280_transport_spi_t *self_p,
                                                           struct spi_driver_t *spi_p)
{
    int res;

    harness_mock_assert("bmp280_transport_spi_init(spi_p)",
                        spi_p);

    harness_mock_read("bmp280_transport_spi_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
