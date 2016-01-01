/**
 * @file drivers/flash.c
 * @version 0.1.0
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

#include "flash_port.i"

int flash_module_init(void)
{
    return (flash_port_module_init());
}

int flash_init(struct flash_driver_t *self_p,
               struct flash_device_t *dev_p)
{
    self_p->dev_p = dev_p;

    return (0);
}

ssize_t flash_read(struct flash_driver_t *self_p,
                   void *dst_p,
                   size_t src,
                   size_t size)
{
    return (flash_port_read(self_p, dst_p, src, size));
}

ssize_t flash_write(struct flash_driver_t *self_p,
                    size_t dst,
                    const void *src_p,
                    size_t size)
{
    return (flash_port_write(self_p, dst, src_p, size));
}
