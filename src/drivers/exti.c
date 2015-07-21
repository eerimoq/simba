/**
 * @file exti.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

int exti_init(struct exti_driver_t *drv_p,
              struct exti_device_t *dev_p,
              int trigger,
              void (*on_interrupt)(void *arg_p),
              void *arg_p)
{
    drv_p->dev_p = dev_p;
    drv_p->trigger = trigger;
    drv_p->on_interrupt = on_interrupt;
    drv_p->arg_p = arg_p;

    return (0);
}

int exti_start(struct exti_driver_t *drv_p)
{
    return (exti_port_start(drv_p));
}

int exti_stop(struct exti_driver_t *drv_p)
{
    return (exti_port_stop(drv_p));
}
