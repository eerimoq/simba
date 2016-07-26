/**
 * @file drivers/linux/pin_port.i
 * @version 3.0.0
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
    /* PMC */
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOA);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOB);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOC);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOD);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOE);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOF);

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
    const struct pin_device_t *dev_p = self_p->dev_p;

    return ((dev_p->pio_p->PDSR & dev_p->mask) != 0);
}

static int pin_port_write(struct pin_driver_t *self_p, int value)
{
    const struct pin_device_t *dev_p = self_p->dev_p;

    if (value == 1) {
        dev_p->pio_p->SODR = dev_p->mask;
    } else {
        dev_p->pio_p->CODR = dev_p->mask;
    }

    return (0);
}

static int pin_port_toggle(struct pin_driver_t *self_p)
{
    const struct pin_device_t *dev_p = self_p->dev_p;

    if (dev_p->pio_p->ODSR & dev_p->mask) {
        dev_p->pio_p->CODR = dev_p->mask;
    } else {
        dev_p->pio_p->SODR = dev_p->mask;
    }

    return (0);
}

static int pin_port_set_mode(struct pin_driver_t *self_p, int mode)
{
    const struct pin_device_t *dev_p = self_p->dev_p;

    if (mode == PIN_OUTPUT) {
        dev_p->pio_p->OER = dev_p->mask;
    } else {
        dev_p->pio_p->ODR = dev_p->mask;
    }

    return (0);
}
