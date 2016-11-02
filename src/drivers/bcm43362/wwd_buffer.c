/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
