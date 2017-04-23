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

struct sem_t {
    /** Number of used resources. */
    int count;
    /** Maximum number of resources. */
    int count_max;
    /** Wait list. */
    struct thrd_prio_list_t waiters;
};

/**
 * Initialize the semaphore module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
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
 * @param[in] self_p Semaphore to take.
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
