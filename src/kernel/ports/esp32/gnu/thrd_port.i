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

#undef BIT

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/**
 * Registers are spilled to the save area on the stack.
 */
struct save_area_t {
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
};

extern void thrd_port_main();

xSemaphoreHandle thrd_idle_sem;

static struct thrd_t *main_thrd_p = NULL;
static char *main_stack_top_p = NULL;

void thrd_port_set_main_thrd(struct thrd_t *thrd_p)
{
    main_thrd_p = thrd_p;
}

static struct thrd_t *thrd_port_get_main_thrd(void)
{
    return (main_thrd_p);
}

void thrd_port_set_main_thrd_stack_top(void *top_p)
{
    main_stack_top_p = top_p;
}

static char *thrd_port_get_main_thrd_stack_top(void)
{
    return (main_stack_top_p);
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
    vSemaphoreCreateBinary(thrd_idle_sem);
    xSemaphoreTake(thrd_idle_sem, portMAX_DELAY);
}

static int thrd_port_spawn(struct thrd_t *thrd_p,
                           void *(*main)(void *),
                           void *arg_p,
                           void *stack_p,
                           size_t stack_size)
{
    struct thrd_port_context_t *context_p;
    struct save_area_t *save_area_p;

    /* Put the context at the top of the stack. */
    context_p = (struct thrd_port_context_t *)
        ((((uintptr_t)stack_p) + stack_size - sizeof(*context_p) - 32) & 0xfffffff0);

    context_p->ps = (PS_WOE | PS_UM | PS_CALLINC(1) | PS_INTLEVEL(3));
    context_p->a0 = (uint32_t)thrd_port_main;

    /* The window underflow handler will load those values from the
       stack into a0-a3 and then call thrd_port_main(). */
    save_area_p = (struct save_area_t *)context_p;
    save_area_p--;
    save_area_p->a0 = 0; /* thrd_port_main() will not return so the
                            return address may have any value. */
    save_area_p->a1 = (uint32_t)context_p;
    save_area_p->a2 = (uint32_t)arg_p;
    save_area_p->a3 = (uint32_t)main;

    thrd_p->port.context_p = context_p;

    return (0);
}

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
    /* Yield the Simba FreeRTOS thread and wait for an interrupt to
       occur. The interrupt handlers signals on this semaphore when a
       thread has been resumed and should be scheduled. */
    xSemaphoreTake(thrd_idle_sem, portMAX_DELAY);

    /* Add this thread to the ready list and reschedule. */
    sys_lock();
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);
    thrd_reschedule();
    sys_unlock();
}

static void thrd_port_on_suspend_timer_expired(struct thrd_t *thrd_p)
{
}

static void RAM_CODE thrd_port_tick(void)
{
    xSemaphoreGiveFromISR(thrd_idle_sem, NULL);
}

static void thrd_port_cpu_usage_start(struct thrd_t *thrd_p)
{
}

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p)
{
}

#if CONFIG_MONITOR_THREAD == 1

static cpu_usage_t thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    return (0);
}

static void thrd_port_cpu_usage_reset(struct thrd_t *thrd_p)
{
}

#endif

static const void *thrd_port_get_bottom_of_stack(struct thrd_t *thrd_p)
{
    const void *bottom_p;

    if (thrd_p == thrd_self()) {
        bottom_p = thrd_port_get_bottom_of_stack_current_thread();
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
