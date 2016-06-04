/**
 * @file arm/gnu/thrd_port.i
 * @version 0.6.0
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

#define THRD_IDLE_STACK_MAX    256
#define THRD_MONITOR_STACK_MAX 512

static struct thrd_t main_thrd __attribute__ ((section (".main_stack")));

static void thrd_port_cpu_usage_start(struct thrd_t *thrd_p);

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p);

__attribute__((naked))
static void swap(void)
{
    /* Store registers. lr is the return address. */
    asm volatile ("push {lr}");
    asm volatile ("push {r4-r11}");

    /* Save 'out_p' stack pointer. */
    asm volatile ("mov %0, sp" : "=r" (scheduler.current_p->port.context_p));

    scheduler.current_p = scheduler.next_p;
    scheduler.next_p = NULL;

    /* Restore 'in_p' stack pointer. */
    asm volatile ("mov sp, %0" : : "r" (scheduler.current_p->port.context_p));

    asm volatile ("pop {r4-r11}");
    asm volatile ("pop {pc}");
}

ISR(pend_sv)
{
    swap();
}

/**
 * Trigger the pendsv interrupt that handles the rescheduling.
 */
static void thrd_port_swap(struct thrd_t *in_p,
                           struct thrd_t *out_p)
{
    scheduler.next_p = in_p;
    SAM_SCB->ICSR = SCB_ICSR_PENDSVSET;
    asm volatile ("cpsie i");
    asm volatile ("cpsid i");
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
    /* Configure the cpu usage timer counter. */
    pmc_peripheral_clock_enable(PERIPHERAL_ID_TC0);

    SAM_TC0->CHANNEL[0].CMR = TC_CMR_CAPTURE_TCCLKS(3);

    /* Start the timer counter. */
    SAM_TC0->CHANNEL[0].CCR = (TC_CCR_SWTRG | TC_CCR_CLKEN);
}

__attribute__((naked))
static void thrd_port_main(void)
{
    /* Enable interrupts. */
    asm volatile ("cpsie i");

    /* Call thread main function with argument. */
    asm volatile ("mov r0, r10");
    asm volatile ("blx r9");

    /* Call termination function. */
    asm volatile ("blx %0" : : "r" (terminate));
}

static int thrd_port_spawn(struct thrd_t *thrd_p,
                           void *(*main)(void *),
                           void *arg_p,
                           void *stack_p,
                           size_t stack_size)
{
    struct thrd_port_context_t *context_p;

    context_p = (stack_p + stack_size - sizeof(*context_p));
    thrd_p->port.context_p = context_p;

    /* Prepare the software context. */
    context_p->r9 = (uint32_t)main;
    context_p->r10 = (uint32_t)arg_p;

    /* 0xfffffff9 is used to return from an exception. See EXC_RETURN
       in ARM documentation. */
    context_p->lr_ex = 0xfffffff9;

    /* Prepare the hardware context. */
    context_p->pc = (uint32_t)thrd_port_main;
    context_p->psr = 0x01000000;

    return (0);
}

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
    /* Wait for an interrupt to occur. */
    asm volatile ("wfi");

    /* Unlock the system to handle the interrupt. */
    sys_unlock();

    /* Add this thread to the ready list and reschedule. */
    sys_lock();
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
    thrd_p->port.cpu.start = SAM_TC0->CHANNEL[0].CV;
}

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p)
{
    thrd_p->port.cpu.period.time += (SAM_TC0->CHANNEL[0].CV
                                     - thrd_p->port.cpu.start);
}

#if CONFIG_MONITOR_THREAD == 1

static float thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    return ((100.0 * thrd_p->port.cpu.period.time)
            / (SAM_TC0->CHANNEL[0].CV - thrd_p->port.cpu.period.start));
}

static void thrd_port_cpu_usage_reset(struct thrd_t *thrd_p)
{
    thrd_p->port.cpu.period.start = SAM_TC0->CHANNEL[0].CV;
    thrd_p->port.cpu.period.time = 0;
}

#endif
