/**
 * @file drivers/flash_port.i
 * @version 5.0.0
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

int flash_port_module_init(void)
{
    return (0);
}

ssize_t flash_port_read(struct flash_driver_t *self_p,
                        void *dst_p,
                        size_t src,
                        size_t size)
{
    return (-1);
}

ssize_t flash_port_write(struct flash_driver_t *self_p,
                         size_t dst,
                         const void *src_p,
                         size_t size)
{
    return (-1);
}
