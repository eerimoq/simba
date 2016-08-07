/**
 * @file drivers/ports/stm32f1/pin_port.h
 * @version 5.0.0
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

#ifndef __DRIVERS_PIN_PORT_H__
#define __DRIVERS_PIN_PORT_H__

struct pin_device_t {
    volatile struct stm32_gpio_t *regs_p;
    int bit;
};

struct pin_driver_t {
    struct pin_device_t *dev_p;
};

static inline int pin_port_device_set_mode(const struct pin_device_t *dev_p,
                                           int mode)           
{
    int shift;
    volatile uint32_t *cr_p;

    shift = (4 * dev_p->bit);
    cr_p = &dev_p->regs_p->CRL;

    if (dev_p->bit >= 8) {
        shift -= 32;
        cr_p++;
    }

    if (mode == PIN_OUTPUT) {
        *cr_p = bits_insert_32(*cr_p, shift, 4, 0x1);
    } else {
        *cr_p = bits_insert_32(*cr_p, shift, 4, 0x4);
    }

    return (0);
}

static inline int pin_port_device_read(const struct pin_device_t *dev_p)
{
    return (-1);
}

static inline int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    dev_p->regs_p->BSRR = (1 << dev_p->bit);
    
    return (0);
}

static inline int pin_port_device_write_low(const struct pin_device_t *dev_p) 
{
    dev_p->regs_p->BSRR = (1 << (16 + dev_p->bit));
    
    return (0);
}

#endif
