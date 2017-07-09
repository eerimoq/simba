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
 * This file is part of the Simba project.
 */

#include "simba.h"

int test_get_temp_humid(struct harness_t *harness_p)
{
    struct sht3xd_driver_t sht3xd;
    struct i2c_driver_t i2c;
    float temp, humid;
    uint32_t serial;

    BTASSERT(i2c_init(&i2c, &i2c_device[0], I2C_BAUDRATE_100KBPS, -1) == 0);
    BTASSERT(sht3xd_init(&sht3xd, &i2c, SHT3X_DIS_I2C_ADDR_A) == 0);
    BTASSERT(sht3xd_start(&sht3xd) == 0);

    serial = 0;
    BTASSERT(sht3xd_get_serial(&sht3xd, &serial) == 0);
    BTASSERT(serial != 0);

    BTASSERT(sht3xd_get_temp_humid(&sht3xd, &temp, &humid) == 0);
    BTASSERT(temp > 0);
    BTASSERT(temp < 50);
    BTASSERT(humid >= 0);
    BTASSERT(humid <= 100);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_get_temp_humid, "test_get_temp_humid" },
        { NULL, NULL }
    };

    sys_start();
    sht3xd_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
