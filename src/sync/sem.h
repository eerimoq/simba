/**
 * @file sync/sem.h
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

#ifndef __SYNC_SEM_H__
#define __SYNC_SEM_H__

#include "simba.h"

/**
 * Compile-time declaration of a semaphore.
 *
 * @param[in] name Semaphore to initialize.
 * @param[in] count Initial count. Set the initial count to the same
 *                  value as count_max to initialize the semaphore
 *                  with all resources used.
 * @param[in] count_max Maximum number of users holding the semaphore
 *                      at the same time.
 */
#define SEM_INIT_DECL(name, _count, _count_max)         \
    struct sem_t name = { .count = _count,              \
                          .count_max = _count_max,      \
                          .head_p = NULL }

struct sem_elem_t;

struct sem_t {
    /** Number of used resources. */
    int count;
    /** Maximum number of resources. */
    int count_max;
    /** Wait list. */
    struct sem_elem_t *head_p;
};

/**
 * Initialize the semaphore module.
 *
 * @return zero(0) or negative error code
 */
int sem_module_init(void);

/**
 * Initialize given semaphore object. Maximum count is the number of
 * resources that can be taken at any given moment.
 *
 * @param[in] self_p Semaphore to initialize.
 * @param[in] count Initial taken resource count. Set the initial
 *                  count to the same value as count_max to initialize
 *                  the semaphore with all resources taken.
 * @param[in] count_max Maximum number of resources that can be taken
 *                      at any given moment.
 *
 * @return zero(0) or negative error code.
 */
int sem_init(struct sem_t *self_p,
             int count,
             int count_max);

/**
 * Take given semaphore. If the semaphore count is zero the calling
 * thread will be suspended until count is incremented by
 * `sem_give()`.
 *
 * @param[in] self_p Semaphore to get.
 * @param[in] timeout_p Timeout.
 *
 * @return zero(0) or negative error code.
 */
int sem_take(struct sem_t *self_p,
             struct time_t *timeout_p);

/**
 * Give given count to given semaphore. Any suspended thread waiting
 * for this semaphore, in `sem_take()`, is resumed. This continues
 * until the semaphore count becomes zero or there are no threads in
 * the suspended list.
 *
 * Giving a count greater than the currently taken count is allowed
 * and results in all resources available. This is especially useful
 * for binary semaphores where `sem_give()` if often called more often
 * than `sem_take()`.
 *
 * @param[in] self_p Semaphore to give count to.
 * @param[in] count Count to give.
 *
 * @return zero(0) or negative error code.
 */
int sem_give(struct sem_t *self_p,
             int count);

/**
 * Give given count to given semaphore from isr or with the system
 * lock taken.
 *
 * @param[in] self_p Semaphore to give count to.
 * @param[in] count Count to give.
 *
 * @return zero(0) or negative error code.
 */
int sem_give_isr(struct sem_t *self_p,
                 int count);

#endif
