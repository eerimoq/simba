/**
 * @file linux/gnu/thrd_port.i
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

#include <avr/sleep.h>

#define THRD_IDLE_STACK_MAX 156
#define THRD_MONITOR_STACK_MAX 256

static struct thrd_t main_thrd __attribute__ ((section (".noinit")));

/**
 * @param[in] in Thread to swap in (r24).
 * @param[in] out Thread to swap out (r22).
 */
__attribute__((naked))
static void thrd_port_swap(struct thrd_t *in_p,
                           struct thrd_t *out_p)
{
    /* Save all call-saved registers for the 'out' thrd.*/
    asm volatile ("push r2");
    asm volatile ("push r3");
    asm volatile ("push r4");
    asm volatile ("push r5");
    asm volatile ("push r6");
    asm volatile ("push r7");
    asm volatile ("push r8");
    asm volatile ("push r9");
    asm volatile ("push r10");
    asm volatile ("push r11");
    asm volatile ("push r12");
    asm volatile ("push r13");
    asm volatile ("push r14");
    asm volatile ("push r15");
    asm volatile ("push r16");
    asm volatile ("push r17");
    asm volatile ("push r28");
    asm volatile ("push r29");

    /* Save 'out' stack pointer.*/
    out_p->port.context_p = (void *)SP;

    /* Restore 'in' stack pointer.*/
    SP = (int)in_p->port.context_p;

    /* Restore all call-saved registers for the 'in' thrd.*/
    asm volatile ("pop  r29");
    asm volatile ("pop  r28");
    asm volatile ("pop  r17");
    asm volatile ("pop  r16");
    asm volatile ("pop  r15");
    asm volatile ("pop  r14");
    asm volatile ("pop  r13");
    asm volatile ("pop  r12");
    asm volatile ("pop  r11");
    asm volatile ("pop  r10");
    asm volatile ("pop  r9");
    asm volatile ("pop  r8");
    asm volatile ("pop  r7");
    asm volatile ("pop  r6");
    asm volatile ("pop  r5");
    asm volatile ("pop  r4");
    asm volatile ("pop  r3");
    asm volatile ("pop  r2");

    asm volatile("ret");
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
}

static void thrd_port_main(void)
{
    sys_unlock();

    /* Call main function with argument. */
    asm volatile ("movw r24, r4");
    asm volatile ("movw r30, r2");
    asm volatile ("icall");

    /* Thread termination. */
    terminate();
}

static int thrd_port_spawn(struct thrd_t *thrd,
                           void *(*main)(void *),
                           void *arg,
                           void *stack,
                           size_t stack_size)
{
    struct thrd_port_context_t *context_p;

    context_p = (stack + stack_size - sizeof(*context_p));
    context_p->r2  = (int)main;
    context_p->r3  = (int)main >> 8;
    context_p->r4  = (int)arg;
    context_p->r5  = (int)arg >> 8;
#if defined(__AVR_3_BYTE_PC__)
    context_p->pc_3rd_byte = (int)((long)(int)thrd_port_main >> 16);
#endif
    context_p->pcl = (int)thrd_port_main >> 8;
    context_p->pch = (int)thrd_port_main;
    thrd->port.context_p = context_p;

    return (0);
}

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
    /* NOTE: will enter sleep before any interrupt is taken. */
    asm volatile ("sei" : : : "memory");
    asm volatile ("sleep" : : : "memory");
    asm volatile ("cli" : : : "memory");

    /* Add this thread to the ready list and reschedule. */
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);
    thrd_reschedule();
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
}

static void thrd_port_cpu_usage_start(struct thrd_t *thrd_p)
{
}

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p)
{
}

#if CONFIG_MONITOR_THREAD == 1

static float thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    return (0.0);
}

static void thrd_port_cpu_usage_reset(struct thrd_t *thrd_p)
{
}

#endif
