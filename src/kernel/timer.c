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

#include "simba.h"

struct module_t {
    struct timer_t *head_p;    /* List of timers sorted by expiry
                                  tick. */
    struct timer_t tail_timer; /* Tail element of timer list. */
};

static struct module_t module = {
    .head_p = &module.tail_timer,
    .tail_timer = {
        .next_p = NULL,
        .delta = SYS_TICK_MAX
    }
};

/**
 * Insert given timer in the list of active timers.
 */
static void RAM_CODE timer_insert_isr(struct timer_t *timer_p)
{
    struct timer_t *elem_p;
    struct timer_t *prev_p;

    /* Find element preceeding this timer. */
    elem_p = module.head_p;
    prev_p = NULL;

    /* Find the element to insert this timer before. Delta is
       initially the timeout. */
    while (elem_p->delta < timer_p->delta) {
        timer_p->delta -= elem_p->delta;
        prev_p = elem_p;
        elem_p = elem_p->next_p;
    }

    /* Adjust the next timer for this timers delta. Do not adjust the
       tail timer. */
    if (elem_p != &module.tail_timer) {
        elem_p->delta -= timer_p->delta;
    }

    /* Insert the new timer into list. */
    timer_p->next_p = elem_p;

    if (prev_p == NULL) {
        module.head_p = timer_p;
    } else {
        prev_p->next_p = timer_p;
    }
}

/**
 * Remove given timer from the list of active timers.
 */
static int timer_remove_isr(struct timer_t *timer_p)
{
    struct timer_t *elem_p;
    struct timer_t *prev_p;

    /* Find element preceeding this timer.*/
    elem_p = module.head_p;
    prev_p = NULL;

    while (elem_p != NULL) {
        if (elem_p == timer_p) {
            /* Remove the timer from the list. */
            if (prev_p != NULL) {
                prev_p->next_p = elem_p->next_p;
            } else {
                module.head_p = elem_p->next_p;
            }

            /* Add the delta timeout to the next timer. */
            if (elem_p->next_p != &module.tail_timer) {
                elem_p->next_p->delta += elem_p->delta;
            }

            return (1);
        }

        prev_p = elem_p;
        elem_p = elem_p->next_p;
    }

    return (0);
}

int timer_module_init(void)
{
    return (0);
}

void RAM_CODE timer_tick_isr(void)
{
    struct timer_t *timer_p;

    sys_lock_isr();

    /* Return if no timers are active.*/
    if (module.head_p != &module.tail_timer) {
        /* Fire all expired timers.*/
        module.head_p->delta--;

        while (module.head_p->delta == 0) {
            timer_p = module.head_p;
            module.head_p = timer_p->next_p;
            timer_p->callback(timer_p->arg_p);

            /* Re-set periodic timers. */
            if (timer_p->flags & TIMER_PERIODIC) {
                timer_p->delta = timer_p->timeout;
                timer_insert_isr(timer_p);
            }
        }
    }

    sys_unlock_isr();
}

int timer_init(struct timer_t *self_p,
               const struct time_t *timeout_p,
               timer_callback_t callback,
               void *arg_p,
               int flags)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(timeout_p != NULL, EINVAL);
    ASSERTN(callback != NULL, EINVAL);

    self_p->timeout = t2st(timeout_p);

    if (self_p->timeout == 0) {
        self_p->timeout = 1;
    }

    self_p->delta = self_p->timeout;
    self_p->flags = flags;
    self_p->callback = callback;
    self_p->arg_p = arg_p;

    return (0);
}

int timer_start(struct timer_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    sys_lock();
    timer_start_isr(self_p);
    sys_unlock();

    return (0);
}

int RAM_CODE timer_start_isr(struct timer_t *self_p)
{
    timer_insert_isr(self_p);

    return (0);
}

int timer_stop(struct timer_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res;

    sys_lock();
    res = timer_stop_isr(self_p);
    sys_unlock();

    return (res);
}

int timer_stop_isr(struct timer_t *self_p)
{
    return (timer_remove_isr(self_p));
}
