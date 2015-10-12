/**
 * @file drivers/linux/pin_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

static int pin_port_init(struct pin_driver_t *drv_p,
                         const struct pin_device_t *dev_p,
                         int mode)
{
    return (pin_set_mode(drv_p, mode));
}

static int pin_port_read(struct pin_driver_t *drv_p)
{
    return (-1);
}

static int pin_port_write(struct pin_driver_t *drv_p, int value)
{
    const struct pin_device_t *dev_p = drv_p->dev_p;

    if (value == 1) {
        dev_p->pio_p->SODR = dev_p->mask;
    } else {
        dev_p->pio_p->CODR = dev_p->mask;
    }

    return (0);
}

static int pin_port_toggle(struct pin_driver_t *drv_p)
{
    const struct pin_device_t *dev_p = drv_p->dev_p;

    if (dev_p->pio_p->ODSR & dev_p->mask) {
        dev_p->pio_p->CODR = dev_p->mask;
    } else {
        dev_p->pio_p->SODR = dev_p->mask;
    }

    return (0);
}

static int pin_port_set_mode(struct pin_driver_t *drv_p, int mode)
{
    const struct pin_device_t *dev_p = drv_p->dev_p;

#define REG_PIOB_OER ((volatile uint32_t *)0x400E1010U) /**< \brief (PIOB) Output Enable Register */

    if (mode == PIN_OUTPUT) {
        dev_p->pio_p->OER = dev_p->mask;
    } else {
        dev_p->pio_p->ODR = dev_p->mask;
    }

    return (0);
}
