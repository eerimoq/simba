/**
 * @file i2c.c
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

#include "simba.h"

#include "i2c_port.i"

int i2c_module_init()
{
    return (i2c_port_module_init());
}

int i2c_init(struct i2c_driver_t *self_p,
             struct i2c_device_t *dev_p,
             int baudrate,
             int address)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    return (i2c_port_init(self_p, dev_p, baudrate, address));
}

int i2c_start(struct i2c_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (i2c_port_start(self_p));
}

int i2c_stop(struct i2c_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (i2c_port_stop(self_p));
}

ssize_t i2c_read(struct i2c_driver_t *self_p,
                 int address,
                 void *buf_p,
                 size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(address < 128, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_read(self_p, address, buf_p, size));
}

ssize_t i2c_write(struct i2c_driver_t *self_p,
                  int address,
                  const void *buf_p,
                  size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(address < 128, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_write(self_p, address, buf_p, size));
}

ssize_t i2c_slave_read(struct i2c_driver_t *self_p,
                       void *buf_p,
                       size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_slave_read(self_p, buf_p, size));
}

ssize_t i2c_slave_write(struct i2c_driver_t *self_p,
                        const void *buf_p,
                        size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_slave_write(self_p, buf_p, size));
}
