/**
 * @file drivers/ports/stm32f3/pin_port.h
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
};

struct pin_driver_t {
    const struct pin_device_t *dev_p;
};

static inline int pin_port_device_set_mode(const struct pin_device_t *dev_p,
                                           int mode)           
{
    return (-1);
}

static inline int pin_port_device_read(const struct pin_device_t *dev_p)
{
    return (-1);
}

static inline int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    return (-1);
}

static inline int pin_port_device_write_low(const struct pin_device_t *dev_p) 
{
    return (-1);
}

#endif
