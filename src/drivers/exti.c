/**
 * @file exti.c
 * @version 1.1.0
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

#include "simba.h"

#include "exti_port.i"

int exti_module_init()
{
    return (exti_port_module_init());
}

int exti_init(struct exti_driver_t *self_p,
              struct exti_device_t *dev_p,
              int trigger,
              void (*on_interrupt)(void *arg_p),
              void *arg_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);
    ASSERTN(on_interrupt != NULL, EINVAL);

    self_p->dev_p = dev_p;
    self_p->trigger = trigger;
    self_p->on_interrupt = on_interrupt;
    self_p->arg_p = arg_p;

    return (0);
}

int exti_start(struct exti_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (exti_port_start(self_p));
}

int exti_stop(struct exti_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (exti_port_stop(self_p));
}

int exti_clear(struct exti_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (exti_port_clear(self_p));
}
