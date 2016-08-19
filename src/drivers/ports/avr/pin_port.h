/**
 * @file drivers/ports/avr/pin_port.h
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

#include <avr/io.h>

struct pin_device_t {
    volatile uint8_t* sfr_p;
    uint8_t mask;
};

struct pin_driver_t {
    const struct pin_device_t *dev_p;
};

#define PIN(sfr) ((sfr) + 0)
#define DDR(sfr) ((sfr) + 1)
#define PORT(sfr) ((sfr) + 2)

static inline int pin_port_device_set_mode(const struct pin_device_t *dev_p,
                                           int mode)           
{
    if (mode == PIN_OUTPUT) {                           
        *DDR((dev_p)->sfr_p) |= (dev_p)->mask;         
    } else {                                            
        *DDR((dev_p)->sfr_p) &= ~((dev_p)->mask);       
    }
 
    return (0);
}

static inline int pin_port_device_read(const struct pin_device_t *dev_p)
{
    return ((*PIN((dev_p)->sfr_p) & (dev_p)->mask) != 0);
}

static inline int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    *PORT((dev_p)->sfr_p) |= (dev_p)->mask;
    
    return (0);
}

static inline int pin_port_device_write_low(const struct pin_device_t *dev_p) 
{
    *PORT((dev_p)->sfr_p) &= ~((dev_p)->mask);

    return (0);
}

#endif
