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

#include <avr/interrupt.h>

#define PIN(sfr) ((sfr) + 0)
#define DDR(sfr) ((sfr) + 1)
#define PORT(sfr) ((sfr) + 2)

#define EXTI_ISR(number)                                                \
    ISR(INT ## number ## _vect)                                         \
    {                                                                   \
        struct exti_driver_t *self_p = exti_device[number].drv_p;       \
                                                                        \
        if (self_p != NULL) {                                           \
            self_p->on_interrupt(self_p->arg_p);                        \
        }                                                               \
    }

#if (EXTI_DEVICE_MAX >= 1)
EXTI_ISR(0);
#endif
#if (EXTI_DEVICE_MAX >= 2)
EXTI_ISR(1);
#endif
#if (EXTI_DEVICE_MAX >= 3)
EXTI_ISR(2);
#endif
#if (EXTI_DEVICE_MAX >= 4)
EXTI_ISR(3);
#endif
#if (EXTI_DEVICE_MAX >= 5)
EXTI_ISR(4);
#endif
#if (EXTI_DEVICE_MAX >= 6)
EXTI_ISR(5);
#endif

static int exti_port_module_init()
{
    return (0);
}

static int exti_port_start(struct exti_driver_t *self_p)
{
    struct exti_device_t *dev_p = self_p->dev_p;

    dev_p->drv_p = self_p;
    *DDR(dev_p->pin_p->sfr_p) &= ~(dev_p->pin_p->mask);

    if (dev_p->id < 3) {
        EICRA |= (self_p->trigger << (2 * dev_p->id));
    } else {
#if (EXTI_DEVICE_MAX > 2)
        EICRB |= (self_p->trigger << (2 * (dev_p->id - 4)));
#endif
    }

    EIFR = _BV(dev_p->id);
    EIMSK |= _BV(dev_p->id);

    return (0);
}

static int exti_port_stop(struct exti_driver_t *self_p)
{
    struct exti_device_t *dev_p = self_p->dev_p;

    EIMSK &= ~_BV(dev_p->id);
    self_p->dev_p = NULL;

    return (0);
}

static int exti_port_clear(struct exti_driver_t *self_p)
{
    /* Clear the interrupt flag. */
    EIFR = _BV(self_p->dev_p->id);

    return (0);
}
