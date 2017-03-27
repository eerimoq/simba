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

#ifndef __SYNC_RWLOCK_H__
#define __SYNC_RWLOCK_H__

#include "simba.h"

struct rwlock_t {
    int number_of_readers;
    int number_of_writers;
    volatile struct rwlock_elem_t *readers_p;
    volatile struct rwlock_elem_t *writers_p;
};

/**
 * Initialize the reader-writer lock module. This function must be
 * called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
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
 * Take given reader-writer lock. Multiple threads can have the reader
 * lock at the same time.
 *
 * @param[in] self_p Reader-writer lock to take.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_take(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock.
 *
 * @param[in] self_p Reader-writer lock give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_give(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock from isr or with the system lock
 * taken.
 *
 * @param[in] self_p Reader-writer lock to give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_give_isr(struct rwlock_t *self_p);

/**
 * Take given reader-writer lock as a writer. Only one thread can have
 * the lock at a time, including both readers and writers.
 *
 * @param[in] self_p Reader-writer lock to take.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_take(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock.
 *
 * @param[in] self_p Reader-writer lock to give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_give(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock from isr or with the system lock
 * taken.
 *
 * @param[in] self_p Reader-writer lock to give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_give_isr(struct rwlock_t *self_p);

#endif
