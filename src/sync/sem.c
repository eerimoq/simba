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

int sem_module_init(void)
{
    return (0);
}

int sem_init(struct sem_t *self_p,
             int count,
             int count_max)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN((count >= 0) && (count <= count_max), EINVAL);
    ASSERTN(count_max > 0, EINVAL);

    self_p->count = count;
    self_p->count_max = count_max;

    thrd_prio_list_init(&self_p->waiters);

    return (0);
}

int sem_take(struct sem_t *self_p,
             struct time_t *timeout_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int err = 0;
    struct thrd_prio_list_elem_t elem;

    sys_lock();

    if (self_p->count == self_p->count_max) {
        elem.thrd_p = thrd_self();
        thrd_prio_list_push_isr(&self_p->waiters, &elem);
        err = thrd_suspend_isr(timeout_p);

        if (err == -ETIMEDOUT) {
            thrd_prio_list_remove_isr(&self_p->waiters, &elem);
        }
    } else {
        self_p->count++;
    }

    sys_unlock();

    return (err);
}

int sem_give(struct sem_t *self_p,
             int count)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(count >= 0, EINVAL);

    sys_lock();
    sem_give_isr(self_p, count);
    sys_unlock();

    return (0);
}

int sem_give_isr(struct sem_t *self_p,
                 int count)
{
    struct thrd_prio_list_elem_t *elem_p;

    self_p->count -= count;

    if (self_p->count < 0) {
        self_p->count = 0;
    }

    while ((self_p->count < self_p->count_max)
           && ((elem_p = thrd_prio_list_pop_isr(&self_p->waiters)) != NULL)) {
        self_p->count++;
        thrd_resume_isr(elem_p->thrd_p, 0);
    }

    return (0);
}
