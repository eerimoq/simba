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

struct rwlock_elem_t {
    struct thrd_t *thrd_p;
    volatile struct rwlock_elem_t *next_p;
    volatile struct rwlock_elem_t *prev_p;
};

int rwlock_module_init(void)
{
    return (0);
}

int rwlock_init(struct rwlock_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->number_of_readers = 0;
    self_p->number_of_writers = 0;
    self_p->readers_p = NULL;
    self_p->writers_p = NULL;

    return (0);
}

int rwlock_reader_take(struct rwlock_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    struct rwlock_elem_t elem;
    int res = 0;

    sys_lock();

    self_p->number_of_readers++;

    /* Wait if the lock is taken by a writer. */
    if (self_p->number_of_writers > 0) {
        elem.thrd_p = thrd_self();
        elem.next_p = self_p->readers_p;
        elem.prev_p = NULL;
        self_p->readers_p = &elem;

        thrd_suspend_isr(NULL);
    }

    sys_unlock();

    return (res);
}

int rwlock_reader_give(struct rwlock_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    sys_lock();
    rwlock_reader_give_isr(self_p);
    sys_unlock();

    return (0);
}

int rwlock_reader_give_isr(struct rwlock_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    volatile struct rwlock_elem_t *elem_p;

    self_p->number_of_readers--;

    if ((self_p->number_of_writers > 0)
        && (self_p->number_of_readers == 0)) {
        elem_p = self_p->writers_p;
        self_p->writers_p = elem_p->next_p;

        if (self_p->writers_p != NULL) {
            self_p->writers_p->prev_p = NULL;
        }

        thrd_resume_isr(elem_p->thrd_p, 0);
    }

    return (0);
}

int rwlock_writer_take(struct rwlock_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    struct rwlock_elem_t elem;
    int res = 0;

    sys_lock();

    self_p->number_of_writers++;

    /* Wait if the lock is taken by a reader or another writer. */
    if ((self_p->number_of_readers > 0)
        || (self_p->number_of_writers > 1)) {
        elem.thrd_p = thrd_self();
        elem.next_p = self_p->writers_p;
        elem.prev_p = NULL;
        self_p->writers_p = &elem;

        thrd_suspend_isr(NULL);
    }

    sys_unlock();

    return (res);
}

int rwlock_writer_give(struct rwlock_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    sys_lock();
    rwlock_writer_give_isr(self_p);
    sys_unlock();

    return (0);
}

int rwlock_writer_give_isr(struct rwlock_t *self_p)
{
    volatile struct rwlock_elem_t *elem_p;

    self_p->number_of_writers--;

    if (self_p->number_of_writers > 0) {
        elem_p = self_p->writers_p;
        self_p->writers_p = elem_p->next_p;

        if (self_p->writers_p != NULL) {
            self_p->writers_p->prev_p = NULL;
        }

        thrd_resume_isr(elem_p->thrd_p, 0);
    } else if (self_p->number_of_readers > 0) {
        elem_p = self_p->readers_p;

        do {
            self_p->readers_p = elem_p->next_p;

            if (self_p->readers_p != NULL) {
                self_p->readers_p->prev_p = NULL;
            }

            thrd_resume_isr(elem_p->thrd_p, 0);

            elem_p = elem_p->next_p;
        } while (elem_p != NULL);
    }

    return (0);
}
