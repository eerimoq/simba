/**
 * @file wwd_buffer.c
 * @version 2.0.0
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

wwd_result_t host_buffer_init(void* native_arg)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_buffer_get(wiced_buffer_t* buffer,
                             wwd_buffer_dir_t direction,
                             unsigned short size, wiced_bool_t wait)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

void host_buffer_release(wiced_buffer_t buffer, wwd_buffer_dir_t direction)
{
    ASSERTN(0, ENOSYS);
}

uint8_t* host_buffer_get_current_piece_data_pointer(wiced_buffer_t buffer)
{
    ASSERTN(0, ENOSYS);
    
    return (NULL);
}

uint16_t host_buffer_get_current_piece_size(wiced_buffer_t buffer)
{
    ASSERTN(0, ENOSYS);
    
    return (0);
}

wwd_result_t host_buffer_set_size(wiced_buffer_t buffer,
                                  unsigned short size)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wiced_buffer_t host_buffer_get_next_piece(wiced_buffer_t buffer)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_buffer_add_remove_at_front(wiced_buffer_t* buffer,
                                             int32_t add_remove_amount)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_buffer_check_leaked()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_buffer_add_application_defined_pool(void* pool_in,
                                                      wwd_buffer_dir_t direction)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}
