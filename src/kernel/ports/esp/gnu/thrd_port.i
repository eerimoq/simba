/**
 * @file linux/gnu/thrd_port.i
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

#define THRD_IDLE_STACK_MAX    768
#define THRD_MONITOR_STACK_MAX 256

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static struct thrd_t main_thrd __attribute__ ((section (".simba_main_thrd")));

xSemaphoreHandle thrd_idle_sem;

/**
 * @param[in] in_p Thread to swap in (a2).
 * @param[in] out_p Thread to swap out (a3).
 */
void thrd_port_swap(struct thrd_t *in_p,
                    struct thrd_t *out_p)
{
    /* Use 32 bytes on the stack. */
    asm volatile("addi	a1, a1, -32");

    /* Store the out-threads' registers on its stack. */
    asm volatile("s32i.n a15, a1, 16");
    asm volatile("s32i.n a14, a1, 12");
    asm volatile("s32i.n a13, a1,  8");
    asm volatile("s32i.n a12, a1,  4");
    asm volatile("s32i.n  a0, a1,  0");

    /* Save 'out_p' stack pointer. */
    asm volatile ("s32i.n a1, %0, 8" : "=r" (out_p));

    /* Restore 'in_p' stack pointer. */
    asm volatile ("l32i.n a1, %0, 8" : : "r" (in_p));

    /* Load the in-threads' registers from its stack. */
    asm volatile("l32i.n  a0, a1,  0");
    asm volatile("l32i.n a12, a1,  4");
    asm volatile("l32i.n a13, a1,  8");
    asm volatile("l32i.n a14, a1, 12");
    asm volatile("l32i.n a15, a1, 16");

    /* Restore the stack. */
    asm volatile("addi	a1, a1, 32");
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
    vSemaphoreCreateBinary(thrd_idle_sem);
    xSemaphoreTake(thrd_idle_sem, portMAX_DELAY);
}

static void thrd_port_main(void)
{
    sys_unlock();

    /* Call main function with argument. */
    asm volatile ("mov a3, a12");
    asm volatile ("mov a2, a13");
    asm volatile ("callx0 a3");

    /* Thread termination. */
    terminate();
}

static int thrd_port_spawn(struct thrd_t *thrd_p,
                           void *(*main)(void *),
                           void *arg_p,
                           void *stack_p,
                           size_t stack_size)
{
    struct thrd_port_context_t *context_p;

    /* Put the context at the top of the stack. */
    context_p = (struct thrd_port_context_t *)
        (((char *)stack_p) + stack_size - sizeof(*context_p) - 32);
    context_p->a0 = (uint32_t)thrd_port_main;
    context_p->a12 = (uint32_t)main;
    context_p->a13 = (uint32_t)arg_p;

    thrd_p->port.context_p = context_p;

    return (0);
}

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
    /* Yield the Simba FreeRTOS thread and wait for an interrupt to
     * occur. The interrupt handlers signals on this semaphore when a
     * thread has been resumed and should be scheduled. */
    xSemaphoreTake(thrd_idle_sem, portMAX_DELAY);

    /* Add this thread to the ready list and reschedule. */
    sys_lock();
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);
    thrd_reschedule();
    sys_unlock();
}

static void thrd_port_suspend_timer_callback(void *arg_p)
{
    struct thrd_t *thrd_p = arg_p;

    /* Push thread on scheduler ready queue. */
    thrd_p->err = -ETIMEDOUT;
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);
}

static void thrd_port_tick(void)
{
    xSemaphoreGiveFromISR(thrd_idle_sem, NULL);
}

static void thrd_port_cpu_usage_start(struct thrd_t *thrd_p)
{
}

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p)
{
}

#if !defined(THRD_NMONITOR)

static float thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    return (0.0);
}

static void thrd_port_cpu_usage_reset(struct thrd_t *thrd_p)
{
}

#endif
