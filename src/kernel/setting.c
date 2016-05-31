/**
 * @file setting.c
 * @version 0.5.0
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

#include "setting_port.i"

int setting_module_init(void)
{
    return (setting_port_module_init());
}

ssize_t setting_read(void *dst_p, size_t src, size_t size)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(src >= 0, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (setting_port_read(dst_p, src, size));
}

ssize_t setting_write(size_t dst, const void *src_p, size_t size)
{
    ASSERTN(dst >= 0, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (setting_port_write(dst, src_p, size));
}
