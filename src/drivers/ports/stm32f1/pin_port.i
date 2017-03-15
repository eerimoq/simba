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

static int pin_port_module_init(void)
{
    return (0);
}

static int pin_port_init(struct pin_driver_t *self_p,
                         const struct pin_device_t *dev,
                         int mode)
{
    return (pin_set_mode(self_p, mode));
}

static int pin_port_read(struct pin_driver_t *self_p)
{
    return (0);
}

static int pin_port_write(struct pin_driver_t *self_p, int value)
{
    if (value != 0) {
        pin_device_write_high(self_p->dev_p);
    } else {
        pin_device_write_low(self_p->dev_p);
    }

    return (0);
}

static int pin_port_toggle(struct pin_driver_t *self_p)
{
    int value;

    value = ((self_p->dev_p->regs_p->ODR & (1 << self_p->dev_p->bit)) != 0);

    return (pin_port_write(self_p, !value));
}

static int pin_port_set_mode(struct pin_driver_t *self_p, int mode)
{
    return (pin_device_set_mode(self_p->dev_p, mode));
}
