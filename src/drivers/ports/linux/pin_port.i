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

static int pin_port_module_init(void)
{
    return (socket_device_module_init());
}

static int pin_port_init(struct pin_driver_t *drv_p,
                         const struct pin_device_t *dev_p,
                         int mode)
{
    return (0);
}

static int pin_port_read(struct pin_driver_t *drv_p)
{
    return (0);
}

static int pin_port_write(struct pin_driver_t *drv_p, int value)
{
    if (value == 1) {
        return (pin_device_write_high(drv_p->dev_p));
    } else {
        return (pin_device_write_low(drv_p->dev_p));
    }
}

static int pin_port_toggle(struct pin_driver_t *drv_p)
{
    return (pin_write(drv_p, !drv_p->dev_p->value));
}

static int pin_port_set_mode(struct pin_driver_t *drv_p, int mode)
{
    return (0);
}

int pin_port_device_set_mode(const struct pin_device_t *dev_p,
                                           int mode)
{
    return (0);
}

int pin_port_device_read(const struct pin_device_t *dev_p)
{
    return (0);
}

int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    struct pin_device_t *d_p;

    d_p = (struct pin_device_t *)dev_p;
    d_p->value = 1;

    sys_lock();

    if (socket_device_is_pin_device_connected_isr(d_p) == 1) {
        socket_device_pin_device_write_isr(d_p, "high\r\n", 6);
    }

    sys_unlock();

    return (0);
}

int pin_port_device_write_low(const struct pin_device_t *dev_p)
{
    struct pin_device_t *d_p;

    d_p = (struct pin_device_t *)dev_p;
    d_p->value = 0;

    sys_lock();

    if (socket_device_is_pin_device_connected_isr(d_p) == 1) {
        socket_device_pin_device_write_isr(d_p, "low\r\n", 5);
    }

    sys_unlock();

    return (0);
}
