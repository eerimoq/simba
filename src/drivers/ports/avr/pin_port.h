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
