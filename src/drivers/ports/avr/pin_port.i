/**
 * @file drivers/linux/pin_port.i
 * @version 0.2.0
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

#define PIN(sfr) ((sfr) + 0)
#define DDR(sfr) ((sfr) + 1)
#define PORT(sfr) ((sfr) + 2)

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
    return ((*PIN(self_p->dev_p->sfr_p) & self_p->dev_p->mask) != 0);
}

static int pin_port_write(struct pin_driver_t *self_p, int value)
{
    if (value != 0) {
        *PORT(self_p->dev_p->sfr_p) |= self_p->dev_p->mask;
    } else {
        *PORT(self_p->dev_p->sfr_p) &= ~(self_p->dev_p->mask);
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
    if (mode == PIN_OUTPUT) {
        *DDR(self_p->dev_p->sfr_p) |= self_p->dev_p->mask;
    } else {
        *DDR(self_p->dev_p->sfr_p) &= ~(self_p->dev_p->mask);
    }

    return (0);
}
