/**
 * @file exti_port.i
 * @version 4.1.0
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
