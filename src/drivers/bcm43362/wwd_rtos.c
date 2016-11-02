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
