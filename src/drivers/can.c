/**
 * @file can.c
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

#include "can_port.i"

int can_module_init(void)
{
    return (0);
}

int can_init(struct can_driver_t *drv_p,
             struct can_device_t *dev_p,
             int speed)
{
    chan_init(&drv_p->chout,
              NULL,
              (ssize_t (*)(chan_t *, const void *, size_t))write_cb,
              NULL);

    return (can_port_init(drv_p, dev_p, speed));
}

int can_start(struct can_driver_t *drv_p)
{
    return (0);
}

int can_stop(struct can_driver_t *drv_p)
{
    return (0);
}

ssize_t can_read(struct can_driver_t *drv_p,
                 struct can_frame_t *frame_p)
{
    return (0);
}

ssize_t can_write(struct can_driver_t *drv_p,
                  const struct can_frame_t *frame_p)
{
    return (0);
}
