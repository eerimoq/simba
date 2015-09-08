/**
 * @file linux/gnu/thrd_port.i
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

#include <avr/sleep.h>

#define THRD_IDLE_STACK_MAX 128

static struct thrd_t main_thrd __attribute__ ((section (".noinit")));

/**
 * @param[in] in Thread to swap in (r24).
 * @param[in] out Thread to swap out (r22).
 */
__attribute__((naked))
static void thrd_port_swap(struct thrd_t *in,
                           struct thrd_t *out)
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
    out->port.context = (void *)SP;

    /* Restore 'in' stack pointer.*/
    SP = (int)in->port.context;

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

static void thrd_port_entry(void)
{
    sys_unlock();

    /* Call entry function with argument. */
    asm volatile ("movw r24, r4");
    asm volatile ("movw r30, r2");
    asm volatile ("icall");
}

static int thrd_port_spawn(struct thrd_t *thrd,
                           void *(*entry)(void *),
                           void *arg,
                           void *stack,
                           size_t stack_size)
{
    struct thrd_port_context_t *context;

    context = (stack + stack_size - sizeof(*context));
    context->r2  = (int)entry;
    context->r3  = (int)entry >> 8;
    context->r4  = (int)arg;
    context->r5  = (int)arg >> 8;
#if defined(__AVR_3_BYTE_PC__)
    context->pc_3rd_byte = (int)((long)(int)thrd_port_entry >> 16);
#endif
    context->pcl = (int)thrd_port_entry >> 8;
    context->pch = (int)thrd_port_entry;
    thrd->port.context = context;

    return (0);
}

static void thrd_port_kill(struct thrd_t *thrd)
{
}

static void thrd_port_idle_wait(void)
{
    asm volatile ("sleep" : : : "memory");
}

extern struct uart_driver_t uart;

static void thrd_port_suspend_timer_callback(void *arg)
{    
    struct thrd_t *thrd = arg;

    // Push thread on scheduler ready queue.
    thrd->state = THRD_STATE_READY;
    scheduler_ready_push(thrd);
}

static void thrd_port_tick(void)
{
}
