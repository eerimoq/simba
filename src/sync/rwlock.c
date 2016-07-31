/**
 * @file rwlock.c
 * @version 4.0.0
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
