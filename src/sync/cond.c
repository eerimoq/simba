/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

int cond_module_init()
{
    return (0);
}

int cond_init(struct cond_t *self_p)
{
    thrd_prio_list_init(&self_p->waiters);

    return (0);
}

int cond_wait(struct cond_t *self_p,
              struct mutex_t *mutex_p,
              struct time_t *timeout_p)
{
    int res;
    struct thrd_prio_list_elem_t elem;

    sys_lock();

    mutex_unlock_isr(mutex_p);

    elem.thrd_p = thrd_self();
    thrd_prio_list_push_isr(&self_p->waiters, &elem);

    res = thrd_suspend_isr(timeout_p);

    if (res == -ETIMEDOUT) {
        thrd_prio_list_remove_isr(&self_p->waiters, &elem);
    }

    mutex_lock_isr(mutex_p);

    sys_unlock();

    return (res);
}

int cond_signal(struct cond_t *self_p)
{
    struct thrd_prio_list_elem_t *elem_p;

    sys_lock();

    elem_p = thrd_prio_list_pop_isr(&self_p->waiters);

    if (elem_p != NULL) {
        thrd_resume_isr(elem_p->thrd_p, 0);
    }

    sys_unlock();

    return (elem_p != NULL);
}

int cond_broadcast(struct cond_t *self_p)
{
    int res;
    struct thrd_prio_list_elem_t *elem_p;

    sys_lock();

    res = 0;

    while ((elem_p = thrd_prio_list_pop_isr(&self_p->waiters)) != NULL) {
        thrd_resume_isr(elem_p->thrd_p, 0);
        res++;
    }

    sys_unlock();

    return (res);
}
