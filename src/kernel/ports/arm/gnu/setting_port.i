/**
 * @file arm/gnu/setting_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015, Erik Moqvist
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

extern char setting_area[];
static struct flash_driver_t drv;

static int setting_port_module_init(void)
{
    return (flash_init(&drv, &flash_0_dev));
}

static ssize_t setting_port_read(void *dst_p, size_t src, size_t size)
{
    return (flash_read(&drv, dst_p, (size_t)&setting_area[src], size));
}

static ssize_t setting_port_write(size_t dst, const void *src_p, size_t size)
{
    return (flash_write(&drv, (size_t)&setting_area[dst], src_p, size));
}
