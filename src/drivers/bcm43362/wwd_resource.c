/**
 * @file bcm43362.c
 * @version 6.0.0
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

#include "platform/wwd_resource_interface.h"

wwd_result_t host_platform_resource_size(wwd_resource_t resource,
                                         uint32_t* size_out)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_resource_read_direct(wwd_resource_t resource,
                                                const void** ptr_out)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_resource_read_indirect(wwd_resource_t resource,
                                                  uint32_t offset,
                                                  void* buffer,
                                                  uint32_t buffer_size,
                                                  uint32_t* size_out)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}
