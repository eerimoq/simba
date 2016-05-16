/**
 * @file pin.c
 * @version 0.4.0
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

#include "pin_port.i"

int pin_module_init(void)
{
    return (pin_port_module_init());
}

int pin_init(struct pin_driver_t *self_p,
             struct pin_device_t *dev_p,
             int mode)
{
    self_p->dev_p = dev_p;

    return (pin_port_init(self_p, dev_p, mode));
}

int pin_read(struct pin_driver_t *self_p)
{
    return (pin_port_read(self_p));
}

int pin_write(struct pin_driver_t *self_p, int value)
{
    return (pin_port_write(self_p, value));
}

int pin_toggle(struct pin_driver_t *self_p)
{
    return (pin_port_toggle(self_p));
}

int pin_set_mode(struct pin_driver_t *self_p, int mode)
{
    return (pin_port_set_mode(self_p, mode));
}
