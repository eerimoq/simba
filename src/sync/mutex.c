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

int mutex_module_init(void)
{
    return (0);
}

int mutex_init(struct mutex_t *self_p)
{
    self_p->is_locked = 0;
    thrd_prio_list_init(&self_p->waiters);

    return (0);
}

int mutex_lock(struct mutex_t *self_p)
{
    int res;

    sys_lock();
    res = mutex_lock_isr(self_p);
    sys_unlock();

    return (res);
}

int mutex_unlock(struct mutex_t *self_p)
{
    int res;

    sys_lock();
    res = mutex_unlock_isr(self_p);
    sys_unlock();

    return (res);
}

int mutex_lock_isr(struct mutex_t *self_p)
{
    struct thrd_prio_list_elem_t elem;

    if (self_p->is_locked == 1) {
        elem.thrd_p = thrd_self();
        thrd_prio_list_push_isr(&self_p->waiters, &elem);
        thrd_suspend_isr(NULL);
    } else {
        self_p->is_locked = 1;
    }

    return (0);
}

int mutex_unlock_isr(struct mutex_t *self_p)
{
    struct thrd_prio_list_elem_t *elem_p;

    elem_p = thrd_prio_list_pop_isr(&self_p->waiters);

    if (elem_p != NULL) {
        thrd_resume_isr(elem_p->thrd_p, 0);
    } else {
        self_p->is_locked = 0;
    }

    return (0);
}
