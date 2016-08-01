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

int exti_port_init(struct exti_driver_t *drv,
                   struct exti_device_t *dev,
                   void (*on_interrupt)(struct exti_driver_t *drv))
{
    return (0);
}

int exti_port_start(struct exti_driver_t *drv)
{
    return (0);
}

int exti_port_stop(struct exti_driver_t *drv)
{
    return (0);
}
