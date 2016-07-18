/**
 * @file drivers/i2c.h
 * @version 2.0.0
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

#ifndef __DRIVERS_I2C_PORT_H__
#define __DRIVERS_I2C_PORT_H__

/* Predefined baudrates. */
#define I2C_PORT_BAUDRATE_1MBPS    0x00
#define I2C_PORT_BAUDRATE_400KBPS  0x0c
#define I2C_PORT_BAUDRATE_100KBPS  0x48

struct i2c_device_t {
    struct i2c_driver_t *drv_p;
};

struct i2c_driver_t {
    struct i2c_device_t *dev_p;
    int address;
    int twbr;
    volatile ssize_t size;
    uint8_t *buf_p;
    struct thrd_t *thrd_p;
};

#endif
