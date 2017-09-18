/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Google Inc.
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
 * This code was originally written by Simon L. B. Nielsen.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"
#include "sht3xd_mock.h"

int mock_write_sht3xd_module_init(int res)
{
    harness_mock_write("sht3xd_module_init()",
                       NULL,
                       0);

    harness_mock_write("sht3xd_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sht3xd_module_init)()
{
    int res;

    harness_mock_assert("sht3xd_module_init()",
                        NULL);

    harness_mock_read("sht3xd_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sht3xd_init(struct i2c_driver_t *i2c_p,
                           int i2c_addr,
                           int res)
{
    harness_mock_write("sht3xd_init(i2c_p)",
                       i2c_p,
                       sizeof(*i2c_p));

    harness_mock_write("sht3xd_init(i2c_addr)",
                       &i2c_addr,
                       sizeof(i2c_addr));

    harness_mock_write("sht3xd_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sht3xd_init)(struct sht3xd_driver_t *self_p,
                                             struct i2c_driver_t *i2c_p,
                                             int i2c_addr)
{
    int res;

    harness_mock_assert("sht3xd_init(i2c_p)",
                        i2c_p);

    harness_mock_assert("sht3xd_init(i2c_addr)",
                        &i2c_addr);

    harness_mock_read("sht3xd_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sht3xd_start(int res)
{
    harness_mock_write("sht3xd_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sht3xd_start)(struct sht3xd_driver_t *self_p)
{
    int res;

    harness_mock_read("sht3xd_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sht3xd_get_temp_humid(float *temp_p,
                                     float *humid_p,
                                     int res)
{
    harness_mock_write("sht3xd_get_temp_humid(): return (temp_p)",
                       temp_p,
                       sizeof(*temp_p));

    harness_mock_write("sht3xd_get_temp_humid(): return (humid_p)",
                       humid_p,
                       sizeof(*humid_p));

    harness_mock_write("sht3xd_get_temp_humid(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sht3xd_get_temp_humid)(struct sht3xd_driver_t *self_p,
                                                       float *temp_p,
                                                       float *humid_p)
{
    int res;

    harness_mock_read("sht3xd_get_temp_humid(): return (temp_p)",
                      temp_p,
                      -1);

    harness_mock_read("sht3xd_get_temp_humid(): return (humid_p)",
                      humid_p,
                      -1);

    harness_mock_read("sht3xd_get_temp_humid(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sht3xd_get_serial(uint32_t *serial_p,
                                 int res)
{
    harness_mock_write("sht3xd_get_serial(): return (serial_p)",
                       serial_p,
                       sizeof(*serial_p));

    harness_mock_write("sht3xd_get_serial(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sht3xd_get_serial)(struct sht3xd_driver_t *self_p,
                                                   uint32_t *serial_p)
{
    int res;

    harness_mock_read("sht3xd_get_serial(): return (serial_p)",
                      serial_p,
                      -1);

    harness_mock_read("sht3xd_get_serial(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
