/**
 * @file timer.c
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

static void timer_insert(struct timer_t *timer_p)
{
    struct timer_t *elem_p, *prev_p;

    /* Find element preceeding this timer.*/
    elem_p = list.head_p;
    prev_p = NULL;

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

int timer_module_init(void)
{
    return (0);
}

void timer_tick(void)
{
    struct timer_t *timer_p;

    sys_lock_irq();

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
            timer_insert(timer_p);
        }
    }

 out:
    sys_unlock_irq();
}

int timer_set(struct timer_t *timer_p,
              struct time_t *timeout_p,
              void (*callback)(void *arg_p),
              void *arg_p,
              int flags)
{
    int err;

    sys_lock();

    err = timer_set_irq(timer_p, timeout_p, callback, arg_p, flags);

    sys_unlock();

    return (err);
}

int timer_set_irq(struct timer_t *timer_p,
                  struct time_t *timeout_p,
                  void (*callback)(void *arg_p),
                  void *arg_p,
                  int flags)
{
    /* Initiate timer. */
    timer_p->timeout = T2ST(timeout_p);

    if (timer_p->timeout == 0) {
        timer_p->timeout = 1;
    }

    timer_p->delta = timer_p->timeout;
    timer_p->flags = flags;
    timer_p->callback = callback;
    timer_p->arg_p = arg_p;

    timer_insert(timer_p);

    return (0);
}

int timer_cancel(struct timer_t *timer_p)
{
    return (1);
}
