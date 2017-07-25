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
    volatile struct nrf5_gpio_t *regs_p;
    int pin;
};

struct pin_driver_t {
    const struct pin_device_t *dev_p;
};

extern struct pin_device_t pin_device[PIN_DEVICE_MAX];

static inline int pin_port_device_set_mode(const struct pin_device_t *dev_p,
                                           int mode)
{
    int res;
    int index;

    index = indexof(dev_p, pin_device);
    
    switch (mode) {

    case PIN_OUTPUT:
        dev_p->regs_p->PIN_CNF[index] = NRF5_GPIO_PIN_CNF_DIR_OUTPUT;
        break;

    default:
        res = -ENOSYS;
    }

    return (res);
}

static inline int pin_port_device_read(const struct pin_device_t *dev_p)
{
    return (-ENOSYS);
}

static inline int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    dev_p->regs_p->OUTCLR = BIT(dev_p->pin);

    return (0);
}

static inline int pin_port_device_write_low(const struct pin_device_t *dev_p)
{
    dev_p->regs_p->OUTSET = BIT(dev_p->pin);

    return (0);
}

#endif
