/**
 * @file arm/gnu/thrd_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#define THRD_IDLE_STACK_MAX 1024

static struct thrd_t main_thrd;

static inline uint32_t __get_sp(void)
{
    register uint32_t reg asm("sp");
 
    return (reg);
}

static inline void __set_sp(uint32_t value)
{
  register uint32_t reg asm("sp");
  reg = value;
  (void)reg;
}

static void thrd_port_swap(struct thrd_t *in_p,
                           struct thrd_t *out_p)
{
    /* Store registers. */
    asm volatile ("push {r4-r11}");
    
    /* Save 'out_p' stack pointer. */
    out_p->port.context_p = (void *)__get_sp();
    
    /* Restore 'in_p' stack pointer. */
    __set_sp((uint32_t)in_p->port.context_p);

    /* Load registers. */
    asm volatile ("pop {r4-r11}");
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
}

static void thrd_port_entry(void)
{
    sys_unlock();

    /* Call entry function with argument. */
    /* asm volatile ("movw r24, r4"); */
    /* asm volatile ("movw r30, r2"); */
    /* asm volatile ("icall"); */

    /* The thread is terminated. */
    sys_lock();
    thrd_self()->state = THRD_STATE_TERMINATED;
    thrd_reschedule();
    sys_unlock();
}

static int thrd_port_spawn(struct thrd_t *thrd,
                           void *(*entry)(void *),
                           void *arg,
                           void *stack,
                           size_t stack_size)
{
    struct thrd_port_context_t *context_p;

    context_p = (stack + stack_size - sizeof(*context_p));
    context_p->r9 = (uint32_t)entry;
    context_p->r10 = (uint32_t)arg;
    context_p->r11 = (uint32_t)thrd_port_entry;
    thrd->port.context_p = context_p;

    return (0);
}

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
}

static void thrd_port_suspend_timer_callback(void *arg_p)
{
    struct thrd_t *thrd = arg_p;

    // Push thread on scheduler ready queue.
    thrd->state = THRD_STATE_READY;
    scheduler_ready_push(thrd);
}

static void thrd_port_tick(void)
{
}
