/**
 * @file timer.c
 * @version 2.0.0
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

struct timer_list_t {
    struct timer_t tail_timer; /* Tail element og timer list.*/
    struct timer_t *head_p;    /* List of timers sorted by expiry tick.*/
};

static struct timer_list_t list = {
    .tail_timer = {
        .next_p = NULL,
        .delta = SYS_TICK_MAX,
        .callback = NULL,
        .arg_p = NULL
    },
    .head_p = &list.tail_timer
};

/**
 * Insert given timer in the list of active timers.
 */
static void timer_insert_isr(struct timer_t *timer_p)
{
    struct timer_t *elem_p, *prev_p;

    /* Find element preceeding this timer.*/
    elem_p = list.head_p;
    prev_p = NULL;

    /* Delta is initially the timeout. */
    while (elem_p->delta < timer_p->delta) {
        timer_p->delta -= elem_p->delta;
        prev_p = elem_p;
        elem_p = elem_p->next_p;
    }

    /* Insert new timer into list.*/
    if (elem_p != &list.tail_timer) {
        elem_p->delta -= timer_p->delta;
    }

    timer_p->next_p = elem_p;

    if (prev_p != NULL) {
        prev_p->next_p = timer_p;
    } else {
        list.head_p = timer_p;
    }
}

/**
 * Remove given timer from the list of active timers.
 */
static int timer_remove_isr(struct timer_t *timer_p)
{
    struct timer_t *elem_p, *prev_p;

    /* Find element preceeding this timer.*/
    elem_p = list.head_p;
    prev_p = NULL;

    while (elem_p != NULL) {
        if (elem_p == timer_p) {
            if (prev_p != NULL) {
                prev_p->next_p = elem_p->next_p;
            } else {
                list.head_p = elem_p->next_p;
            }

            return (0);
        }

        prev_p = elem_p;
        elem_p = elem_p->next_p;
    }

    return (-1);
}

int timer_module_init(void)
{
    return (0);
}

void timer_tick_isr(void)
{
    struct timer_t *timer_p;

    sys_lock_isr();

    /* Return if no timers are active.*/
    if (list.head_p == &list.tail_timer) {
        goto out;
    }

    /* Fire all expired timers.*/
    list.head_p->delta--;

    while (list.head_p->delta == 0) {
        timer_p = list.head_p;
        list.head_p = timer_p->next_p;
        timer_p->callback(timer_p->arg_p);

        /* Re-set periodic timers. */
        if (timer_p->flags & TIMER_PERIODIC) {
            timer_p->delta = timer_p->timeout;
            timer_insert_isr(timer_p);
        }
    }

 out:
    sys_unlock_isr();
}

int timer_init(struct timer_t *self_p,
               struct time_t *timeout_p,
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

int timer_start_isr(struct timer_t *self_p)
{
    timer_insert_isr(self_p);

    return (0);
}

int timer_stop(struct timer_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int err = 0;

    sys_lock();
    err = timer_stop_isr(self_p);
    sys_unlock();

    return (err);
}

int timer_stop_isr(struct timer_t *self_p)
{
    return (timer_remove_isr(self_p));
}
