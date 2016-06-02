/**
 * @file drivers/i2c.h
 * @version 0.5.0
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

#ifndef __DRIVERS_I2C_H__
#define __DRIVERS_I2C_H__

#include "simba.h"

#include "i2c_port.h"

/* Predefined baudrates. */
#define I2C_BAUDRATE_3_2MBPS     I2C_PORT_BAUDRATE_3_2MBPS
#define I2C_BAUDRATE_1MBPS       I2C_PORT_BAUDRATE_1MBPS
#define I2C_BAUDRATE_400K        I2C_PORT_BAUDRATE_400KBPS
#define I2C_BAUDRATE_100K        I2C_PORT_BAUDRATE_100KBPS

extern struct i2c_device_t i2c_device[I2C_DEVICE_MAX];

/**
 * Initialize the i2c module.
 *
 * @return zero(0) or negative error code.
 */
int i2c_module_init();

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] dev_p I2C device to use.
 * @param[in] address I2C local slave address. Set to -1 to disable
 *                    slave mode.
 *
 * @return zero(0) or negative error code.
 */
int i2c_init(struct i2c_driver_t *self_p,
             struct i2c_device_t *dev_p,
             int baudrate,
             int address);

/**
 * Start given driver object. Enables data reception and transmission.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_start(struct i2c_driver_t *self_p);

/**
 * Stop given driver object. Disables data reception and transmission.
 *
 * @param[in] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int i2c_stop(struct i2c_driver_t *self_p);

/**
 * Read into given buffer from given slave.
 *
 * @param[in] self_p Driver object.
 * @param[in] address Slave address.
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
 * Write given buffer to given slave.
 *
 * @param[in] self_p Driver object.
 * @param[in] address Slave address.
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
 * Read into given buffer from the next master that addresses this
 * slave.
 *
 * NOTE: This function may only be used if the driver was initialized
 *       with a local slave address.
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
 * NOTE: This function may only be used if the driver was initialized
 *       with a local slave address.
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
