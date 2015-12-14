/**
 * @file kernel/sem.h
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

#ifndef __KERNEL_SEM_H__
#define __KERNEL_SEM_H__

#include "simba.h"

#define SEM_INIT_DECL(name, _count)                             \
    struct sem_t name = { .count = _count, .head_p = NULL }

struct sem_elem_t;

struct sem_t {
    int count;
    struct sem_elem_t *head_p;
};

/**
 * Initialize module.
 *
 * @return zero(0) or negative error code
 */
int sem_module_init(void);

/**
 * Initialize given semaphore. Count is the number of threads that can
 * hold the semaphore at the same time.
 *
 * @param[in] self_p Semaphore to get.
 * @param[in] count Semaphore count.
 *
 * @return zero(0) or negative error code.
 */
int sem_init(struct sem_t *self_p,
             int count);

/**
 * Get given semaphore. If the semaphore count is zero the calling
 * thread will be suspended until count is incremented by a put call
 * by another thread/isr.
 *
 * @param[in] self_p Semaphore to get.
 * @param[in] timeout_p Timeout.
 *
 * @return zero(0) or negative error code.
 */
int sem_get(struct sem_t *self_p,
            struct time_t *timeout_p);

/**
 * Put given value on semaphore.
 *
 * @param[in] self_p Semaphore to add count to.
 * @param[in] value Count to add.
 *
 * @return zero(0) or negative error code.
 */
int sem_put(struct sem_t *self_p,
            int count);

/**
 * Put given value on semaphore for irq.
 *
 * @param[in] self_p Semaphore to add count to.
 * @param[in] value Count to add.
 *
 * @return zero(0) or negative error code.
 */
int sem_put_irq(struct sem_t *self_p,
                int count);

#endif
