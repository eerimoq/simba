/**
 * @file sd.c
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

#include "sd_port.i"

int sd_init(struct sd_driver_t *drv_p)
{
    return (sd_port_init(drv_p));
}

ssize_t sd_write(struct sd_driver_t *drv_p,
                 uint32_t dst_block,
                 const void *src_p)
{
    return (sd_port_write(drv_p, dst_block, src_p));
}

ssize_t sd_read(struct sd_driver_t *drv_p,
                void *dst_p,
                uint32_t src_block)
{
    return (sd_port_read(drv_p, dst_p, src_block));
}
