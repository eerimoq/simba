/**
 * @file sem.c
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

struct sem_elem_t {
    struct sem_elem_t *next_p;
    struct sem_elem_t *prev_p;
    struct thrd_t *thrd_p;
};

static struct spin_lock_t lock;

int sem_module_init()
{
    return (spin_init(&lock));
}

int sem_init(struct sem_t *sem_p,
             int count)
{
    sem_p->count = count;
    sem_p->head_p = NULL;

    return (0);
}

int sem_get(struct sem_t *sem_p,
            struct time_t *timeout_p)
{
    int err = 0;
    struct sem_elem_t elem;
    spin_irq_t irq;

    spin_lock(&lock, &irq);

    if (sem_p->count == 0) {
        elem.thrd_p = thrd_self();
        elem.next_p = sem_p->head_p;
        elem.prev_p = NULL;
        sem_p->head_p = &elem;
        spin_unlock(&lock, &irq);
        err = thrd_suspend(timeout_p);
        spin_lock(&lock, &irq);

        if (err == -ETIMEDOUT) {
            if (elem.prev_p != NULL) {
                elem.prev_p->next_p = elem.next_p;
            } else {
                sem_p->head_p = elem.next_p;
            }

            if (elem.next_p != NULL) {
                elem.next_p->prev_p = elem.prev_p;
            }
        }
    } else {
        sem_p->count--;
    }

    spin_unlock(&lock, &irq);

    return (err);
}

int sem_put(struct sem_t *sem_p,
            int count)
{
    struct sem_elem_t *elem_p;
    spin_irq_t irq;

    spin_lock(&lock, &irq);

    sem_p->count += count;

    if (sem_p->head_p != NULL) {
        sem_p->count--;
        elem_p = sem_p->head_p;
        sem_p->head_p = elem_p->next_p;

        if (elem_p->next_p != NULL) {
            elem_p->next_p->prev_p = NULL;
        }

        spin_unlock(&lock, &irq);
        thrd_resume(elem_p->thrd_p, 0);
        spin_lock(&lock, &irq);
    }

    spin_unlock(&lock, &irq);

    return (0);
}
