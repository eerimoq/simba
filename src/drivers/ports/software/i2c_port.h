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

/**
 * This port of the i2c module implements the i2c physical layer in
 * software, using the i2c_soft module.
 */

#ifndef __DRIVERS_I2C_SOFT_PORT_H__
#define __DRIVERS_I2C_SOFT_PORT_H__

/**
 * The device data structure contains arguments passed to
 * i2c_soft_init(), as these arguments are missing in the hardware i2c
 * init function, i2c_init().
 */
struct i2c_device_t {
    struct pin_device_t *scl_p;
    struct pin_device_t *sda_p;
    long max_clock_stretching_us;
    long clock_stretching_sleep_us;
};

struct i2c_driver_t {
    struct i2c_soft_driver_t soft;
};

/* Predefined baudrates. */
#define I2C_PORT_BAUDRATE_3_2MBPS             3200000
#define I2C_PORT_BAUDRATE_1MBPS               1000000
#define I2C_PORT_BAUDRATE_400KBPS              400000
#define I2C_PORT_BAUDRATE_100KBPS              100000

#endif
