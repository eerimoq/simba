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

#ifndef __DRIVERS_I2C_H__
#define __DRIVERS_I2C_H__

#include "simba.h"

#if CONFIG_SOFTWARE_I2C == 1
#    include "ports/software/i2c_port.h"
#else
#    include "i2c_port.h"
#endif

/* Predefined baudrates. */
#define I2C_BAUDRATE_3_2MBPS     I2C_PORT_BAUDRATE_3_2MBPS
#define I2C_BAUDRATE_1MBPS       I2C_PORT_BAUDRATE_1MBPS
#define I2C_BAUDRATE_400KBPS     I2C_PORT_BAUDRATE_400KBPS
#define I2C_BAUDRATE_100KBPS     I2C_PORT_BAUDRATE_100KBPS

extern struct i2c_device_t i2c_device[I2C_DEVICE_MAX];

/**
 * Initialize the i2c module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int i2c_module_init();

/**
 * Initialize given driver object. The same driver object is used for
 * both master and slave modes. Use `i2c_start()` to start the device
 * as a master, and `i2c_slave_start()` to start it as a slave.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] dev_p I2C device to use.
 * @param[in] baudrates Bus baudrate when in master mode. Unused in
 *            slave mode.
 * @param[in] address Slave address when in slave mode. Unused in
 *                    master mode.
 *
 * @return zero(0) or negative error code.
 */
int i2c_init(struct i2c_driver_t *self_p,
             struct i2c_device_t *dev_p,
             int baudrate,
             int address);

/**
 * Start given driver object in master mode. Enables data reception
 * and transmission, but does not start any transmission. Use
 * `i2c_read()` and `i2c_write()` to exchange data with the peer.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_start(struct i2c_driver_t *self_p);

/**
 * Stop given driver object. Disables data reception and transmission
 * in master mode.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_stop(struct i2c_driver_t *self_p);

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
ssize_t i2c_read(struct i2c_driver_t *self_p,
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
ssize_t i2c_write(struct i2c_driver_t *self_p,
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
int i2c_scan(struct i2c_driver_t *self_p, int address);

/**
 * Start given driver object in slave mode. Enables data reception and
 * transmission, but does not start any transmission. Data transfers
 * are started by calling the `i2c_slave_read()` and
 * `i2c_slave_write()`.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_slave_start(struct i2c_driver_t *self_p);

/**
 * Stop given driver object. Disables data reception and transmission
 * in slave mode.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_slave_stop(struct i2c_driver_t *self_p);

/**
 * Read into given buffer from the next master that addresses this
 * slave.
 *
 * @param[in] self_p Driver object.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t i2c_slave_read(struct i2c_driver_t *self_p,
                       void *buf_p,
                       size_t size);

/**
 * Write given buffer to the next master that addresses this
 * slave.
 *
 * @param[in] self_p Driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t i2c_slave_write(struct i2c_driver_t *self_p,
                        const void *buf_p,
                        size_t size);

#endif
