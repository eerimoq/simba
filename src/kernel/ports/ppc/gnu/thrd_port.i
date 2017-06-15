/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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

static struct thrd_t main_thrd __attribute__ ((section (".main_stack")));
extern char __main_stack_end;

extern void thrd_port_swap(struct thrd_t *in_p,
                           struct thrd_t *out_p);

extern void thrd_port_main(void);

static void thrd_port_cpu_usage_start(struct thrd_t *thrd_p);

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p);

static struct thrd_t *thrd_port_get_main_thrd(void)
{
    return (&main_thrd);
}

static char *thrd_port_get_main_thrd_stack_top(void)
{
    return (&__main_stack_end);
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
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

    /* Prepare the context on the stack. */
    memset(context_p, 0, sizeof(*context_p));
    context_p->r13 = (uint32_t)main;
    context_p->r14 = (uint32_t)arg_p;
    context_p->lr = (uint32_t)thrd_port_main;

    return (0);
}

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
    /* Unlock the system to handle the interrupt. */
    sys_unlock();

    /* Wait for an interrupt to occur. */
    asm volatile ("wait");

    /* Add this thread to the ready list and reschedule. */
    sys_lock();

    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);
    thrd_reschedule();
}

static void thrd_port_on_suspend_timer_expired(struct thrd_t *thrd_p)
{
}

static void thrd_port_tick(void)
{
}

static void thrd_port_cpu_usage_start(struct thrd_t *thrd_p)
{
    thrd_p->port.cpu.start = SPC5_STM->CNT;
}

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p)
{
    thrd_p->port.cpu.period.time += (SPC5_STM->CNT - thrd_p->port.cpu.start);
}

#if CONFIG_MONITOR_THREAD == 1

#    if CONFIG_FLOAT == 1

static cpu_usage_t thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    return (((cpu_usage_t)100 * thrd_p->port.cpu.period.time)
            / (SPC5_STM->CNT - thrd_p->port.cpu.period.start));
}

#    else

static cpu_usage_t thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    uint32_t time;
    uint32_t period;

    time = (thrd_p->port.cpu.period.time / 128);
    period = ((SPC5_STM->CNT - thrd_p->port.cpu.period.start) / 128);

    return DIV_ROUND(((cpu_usage_t)100 * time), period);
}

#    endif

static void thrd_port_cpu_usage_reset(struct thrd_t *thrd_p)
{
    thrd_p->port.cpu.period.start = SPC5_STM->CNT;
    thrd_p->port.cpu.period.time = 0;
}

#endif

static const void *thrd_port_get_bottom_of_stack(struct thrd_t *thrd_p)
{
    char dummy;
    const void *bottom_p;

    if (thrd_p == thrd_self()) {
        bottom_p = (const void *)&dummy;
    } else {
        sys_lock();
        bottom_p = thrd_p->port.context_p;
        sys_unlock();
    }

    return (bottom_p);
}

static const void *thrd_port_get_top_of_stack(struct thrd_t *thrd_p)
{
    return ((void *)((uintptr_t)thrd_p + thrd_p->stack_size));
}
