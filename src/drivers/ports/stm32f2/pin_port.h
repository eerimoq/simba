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
    if (mode == PIN_OUTPUT) {
        dev_p->regs_p->MODER =
            bits_insert_32(dev_p->regs_p->MODER, 2 * dev_p->bit, 2, 0x1);
    } else {
        dev_p->regs_p->MODER =
            bits_insert_32(dev_p->regs_p->MODER, 2 * dev_p->bit, 2, 0x0);
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
