/**
 * @file drivers/linux/pin_port.i
 * @version 4.1.0
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

static int pin_port_module_init(void)
{
    return (0);
}

static int pin_port_init(struct pin_driver_t *self_p,
                         const struct pin_device_t *dev_p,
                         int mode)
{
    return (pin_set_mode(self_p, mode));
}

static int pin_port_read(struct pin_driver_t *self_p)
{
    return (pin_port_device_read(self_p->dev_p));
}

static int pin_port_write(struct pin_driver_t *self_p, int value)
{
    if (value != 0) {
        pin_device_write_high(self_p->dev_p);
    } else {
        pin_device_write_low(self_p->dev_p);
    }

    return (0);
}

static int pin_port_toggle(struct pin_driver_t *self_p)
{
    *PORT(self_p->dev_p->sfr_p) ^= self_p->dev_p->mask;

    return (0);
}

static int pin_port_set_mode(struct pin_driver_t *self_p, int mode)
{
    return (pin_device_set_mode(self_p->dev_p, mode));
}
