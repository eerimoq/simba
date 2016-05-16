/**
 * @file kernel/rwlock.h
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

#ifndef __KERNEL_RWLOCK_H__
#define __KERNEL_RWLOCK_H__

#include "simba.h"

struct rwlock_t {
    int number_of_readers;
    int number_of_writers;
    volatile struct rwlock_elem_t *readers_p;
    volatile struct rwlock_elem_t *writers_p;
};

/**
 * Initialize the reader-writer lock module.
 *
 * @return zero(0) or negative error code
 */
int rwlock_module_init(void);

/**
 * Initialize given reader-writer lock object.
 *
 * @param[in] self_p Reader-writer lock to initialize.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_init(struct rwlock_t *self_p);

/**
 * Get given reader-writer lock. Multiple threads can have the reader
 * lock at the same time.
 *
 * @param[in] self_p Reader-writer lock to get.
 * @param[in] timeout_p Timeout.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_get(struct rwlock_t *self_p,
                      struct time_t *timeout_p);

/**
 * Put given reader-writer lock.
 *
 * @param[in] self_p Reader-writer lock to add count to.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_put(struct rwlock_t *self_p);

/**
 * Put given reader-writer lock from isr or with the system lock
 * taken.
 *
 * @param[in] self_p Reader-writer lock.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_put_isr(struct rwlock_t *self_p);

/**
 * Get given reader-writer lock as a writer. Only one thread can have
 * the lock at a time, including both readers and writers.
 *
 * @param[in] self_p Reader-writer lock to get.
 * @param[in] timeout_p Timeout.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_get(struct rwlock_t *self_p,
                      struct time_t *timeout_p);

/**
 * Put given reader-writer lock.
 *
 * @param[in] self_p Reader-writer lock.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_put(struct rwlock_t *self_p);

/**
 * Put given reader-writer lock from isr or with the system lock
 * taken.
 *
 * @param[in] self_p Reader-writer lock to add count to.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_put_isr(struct rwlock_t *self_p);

#endif
