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

#ifndef __DRIVERS_I2C_SOFT_H__
#define __DRIVERS_I2C_SOFT_H__

#include "simba.h"

struct i2c_soft_driver_t {
    struct pin_device_t *scl_p;
    struct pin_device_t *sda_p;
    long baudrate;
    long baudrate_us;
    long max_clock_stretching_us;
    long clock_stretching_sleep_us;
};

/**
 * Initialize the i2c soft module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int i2c_soft_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] scl_dev_p The I2C clock pin (SCL).
 * @param[in] sda_dev_p The I2C data pin (SDA).
 * @param[in] baudrate Bus baudrate.
 * @param[in] max_clock_stretching_us Maximum number of microseconds
 *                                    to wait for the clock stretching
 *                                    to end.
 * @param[in] clock_stretching_sleep_us SCL poll interval in number of
 *                                      microseconds waiting for clock
 *                                      stretching to end.
 *
 * @return zero(0) or negative error code.
 */
int i2c_soft_init(struct i2c_soft_driver_t *self_p,
                  struct pin_device_t *scl_dev_p,
                  struct pin_device_t *sda_dev_p,
                  long baudrate,
                  long max_clock_stretching_us,
                  long clock_stretching_sleep_us);

/**
 * Start given driver object. Enables data reception and transmission,
 * but does not start any transmission. Data tarnsfers are started by
 * calling the `i2c_soft_read()` and `i2c_soft_write()`.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_soft_start(struct i2c_soft_driver_t *self_p);

/**
 * Stop given driver object. Disables data reception and transmission.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_soft_stop(struct i2c_soft_driver_t *self_p);

/**
 * Read given number of bytes into given buffer from given slave.
 *
 * @param[in] self_p Driver object.
 * @param[in] address Slave address to read from.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t i2c_soft_read(struct i2c_soft_driver_t *self_p,
                      int address,
                      void *buf_p,
                      size_t size);

/**
 * Write given number of bytes from given buffer to given slave.
 *
 * @param[in] self_p Driver object.
 * @param[in] address Slave address to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t i2c_soft_write(struct i2c_soft_driver_t *self_p,
                       int address,
                       const void *buf_p,
                       size_t size);

/**
 * Scan the i2c bus for a slave with given address.
 *
 * @param[in] self_p Driver object.
 * @param[in] address Address of the slave to scan for.
 *
 * @return true(1) if a slave responded to given address, otherwise
 *         false(0) or negative error code.
 */
int i2c_soft_scan(struct i2c_soft_driver_t *self_p,
                  int address);

#endif
