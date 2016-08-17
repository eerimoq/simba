/**
 * @file drivers/linux/pin_port.i
 * @version 7.0.0
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

static int pin_port_init(struct pin_driver_t *drv,
                         const struct pin_device_t *dev,
                         int mode)
{
    return (0);
}

static int pin_port_read(struct pin_driver_t *drv)
{
    return (0);
}

static int pin_port_write(struct pin_driver_t *drv, int value)
{
    return (0);
}

static int pin_port_toggle(struct pin_driver_t *drv)
{
    return (0);
}

static int pin_port_set_mode(struct pin_driver_t *drv, int mode)
{
    return (0);
}
