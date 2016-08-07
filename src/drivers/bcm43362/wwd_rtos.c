/**
 * @file bcm43362.c
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

#include "simba.h"

#include "RTOS/wwd_rtos_interface.h"

wwd_result_t host_rtos_create_thread(host_thread_type_t* thread,
                                     void(*entry_function)(uint32_t arg),
                                     const char* name,
                                     void* stack,
                                     uint32_t stack_size,
                                     uint32_t priority)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_create_thread_with_arg(host_thread_type_t* thread,
                                              void(*entry_function)(uint32_t),
                                              const char* name,
                                              void* stack,
                                              uint32_t stack_size,
                                              uint32_t priority,
                                              uint32_t arg)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_finish_thread(host_thread_type_t* thread)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_delete_terminated_thread(host_thread_type_t* thread)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_join_thread(host_thread_type_t* thread)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_init_semaphore(host_semaphore_type_t* semaphore)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_get_semaphore(host_semaphore_type_t* semaphore,
                                     uint32_t timeout_ms,
                                     wiced_bool_t will_set_in_isr)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_set_semaphore(host_semaphore_type_t* semaphore,
                                     wiced_bool_t called_from_ISR)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_deinit_semaphore(host_semaphore_type_t* semaphore)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_time_t host_rtos_get_time(void)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_delay_milliseconds(uint32_t num_ms)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_init_queue(host_queue_type_t* queue,
                                  void* buffer,
                                  uint32_t buffer_size,
                                  uint32_t message_size)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_push_to_queue(host_queue_type_t* queue,
                                     void* message,
                                     uint32_t timeout_ms)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_pop_from_queue(host_queue_type_t* queue,
                                      void* message,
                                      uint32_t timeout_ms)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_rtos_deinit_queue(host_queue_type_t* queue)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}
