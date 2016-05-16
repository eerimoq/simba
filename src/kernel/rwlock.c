/**
 * @file rwlock.c
 * @version 0.4.0
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
    self_p->number_of_readers = 0;
    self_p->number_of_writers = 0;
    self_p->readers_p = NULL;
    self_p->writers_p = NULL;

    return (0);
}

int rwlock_reader_get(struct rwlock_t *self_p,
                      struct time_t *timeout_p)
{
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

        res = thrd_suspend_isr(timeout_p);

        if (res == -ETIMEDOUT) {
            self_p->number_of_readers--;

            if (elem.prev_p != NULL) {
                elem.prev_p->next_p = elem.next_p;
            }

            if (elem.next_p != NULL) {
                elem.next_p->prev_p = elem.prev_p;
            }
        }
    }

    sys_unlock();

    return (res);
}

int rwlock_reader_put(struct rwlock_t *self_p)
{
    sys_lock();
    rwlock_reader_put_isr(self_p);
    sys_unlock();

    return (0);
}

int rwlock_reader_put_isr(struct rwlock_t *self_p)
{
    self_p->number_of_readers--;

    if (self_p->number_of_writers > 0) {
        if (thrd_resume_isr(self_p->writers_p->thrd_p, 0) == 1) {
            self_p->writers_p = self_p->writers_p->next_p;

            if (self_p->writers_p != NULL) {
                self_p->writers_p->prev_p = NULL;
            }
        }
    }

    return (0);
}

int rwlock_writer_get(struct rwlock_t *self_p,
                      struct time_t *timeout_p)
{
    struct rwlock_elem_t elem;
    int res = 0;

    sys_lock();

    self_p->number_of_writers++;

    /* Wait if the lock is taken by a reader. */
    if (self_p->number_of_readers > 0) {
        elem.thrd_p = thrd_self();
        elem.next_p = self_p->writers_p;
        elem.prev_p = NULL;
        self_p->writers_p = &elem;

        res = thrd_suspend_isr(timeout_p);

        if (res == -ETIMEDOUT) {
            self_p->number_of_writers--;

            if (elem.prev_p != NULL) {
                elem.prev_p->next_p = elem.next_p;
            }

            if (elem.next_p != NULL) {
                elem.next_p->prev_p = elem.prev_p;
            }
        }
    }

    sys_unlock();

    return (res);
}

int rwlock_writer_put(struct rwlock_t *self_p)
{
    sys_lock();
    rwlock_writer_put_isr(self_p);
    sys_unlock();

    return (0);
}

int rwlock_writer_put_isr(struct rwlock_t *self_p)
{
    volatile struct rwlock_elem_t *elem_p;

    self_p->number_of_writers--;

    if (self_p->number_of_writers > 0) {
        elem_p = self_p->writers_p;

        if (thrd_resume_isr(elem_p->thrd_p, 0) == 1) {
            self_p->writers_p = elem_p->next_p;

            if (self_p->writers_p != NULL) {
                self_p->writers_p->prev_p = NULL;
            }
        }
    } else if (self_p->number_of_readers > 0) {
        elem_p = self_p->readers_p;

        if (thrd_resume_isr(elem_p->thrd_p, 0) == 1) {
            self_p->readers_p = elem_p->next_p;

            if (self_p->readers_p != NULL) {
                self_p->readers_p->prev_p = NULL;
            }
        }
    }

    return (0);
}
