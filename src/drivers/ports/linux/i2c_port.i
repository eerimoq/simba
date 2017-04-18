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

#include "socket_device.h"

static int i2c_port_module_init()
{
    return (0);
}

static int i2c_port_init(struct i2c_driver_t *self_p,
                         struct i2c_device_t *dev_p,
                         int baudrate,
                         int address)
{
    self_p->dev_p = dev_p;

    return (0);
}

static int i2c_port_start(struct i2c_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;

    return (0);
}

static int i2c_port_stop(struct i2c_driver_t *self_p)
{
    self_p->dev_p->drv_p = NULL;

    return (0);
}

static ssize_t i2c_port_read(struct i2c_driver_t *self_p,
                             int address,
                             void *buf_p,
                             size_t size)
{
    return (-1);
}

static ssize_t i2c_port_write(struct i2c_driver_t *self_p,
                              int address,
                              const void *buf_p,
                              size_t size)
{
    struct i2c_device_t *dev_p;
    ssize_t res;

    dev_p = self_p->dev_p;

    sys_lock();

    if (socket_device_is_i2c_device_connected_isr(dev_p) == 1) {
        res = socket_device_i2c_device_write_isr(dev_p,
                                                 address,
                                                 buf_p,
                                                 size);
    } else {
        res = size;
    }

    sys_unlock();

    return (res);
}

static int i2c_port_scan(struct i2c_driver_t *self_p,
                         int address)
{
    return (-1);
}

static int i2c_port_slave_start(struct i2c_driver_t *self_p)
{
    return (-1);
}

static int i2c_port_slave_stop(struct i2c_driver_t *self_p)
{
    return (-1);
}

static ssize_t i2c_port_slave_read(struct i2c_driver_t *self_p,
                                   void *buf_p,
                                   size_t size)
{
    return (-1);
}

static ssize_t i2c_port_slave_write(struct i2c_driver_t *self_p,
                                    const void *buf_p,
                                    size_t size)
{
    return (-1);
}
