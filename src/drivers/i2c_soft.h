/**
 * @file drivers/i2c_soft.h
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_I2C_SOFT_H__
#define __DRIVERS_I2C_SOFT_H__

#include "simba.h"

struct i2c_soft_driver_t {
    struct pin_device_t *scl_p;
    struct pin_device_t *sda_p;
    int baudrate;
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
int i2c_soft_module_init();

/**
 * Initialize given driver object. The same driver object is used for
 * both master and slave modes. Use `i2c_soft_start()` to start the device
 * as a master, and `i2c_soft_slave_start()` to start it as a slave.
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
                  int baudrate,
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
 * Read into given buffer from given slave address.
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
 * Write given buffer to given slave address.
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

#endif
