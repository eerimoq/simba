/**
 * @file bcm43362.c
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

#include "network/wwd_buffer_interface.h"

wwd_result_t host_platform_bus_init()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_bus_deinit()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_bus_enable_interrupt()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_bus_disable_interrupt()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

void host_platform_bus_buffer_freed(wwd_buffer_dir_t direction)
{
    ASSERTN(0, ENOSYS);
}
