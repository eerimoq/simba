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

#ifndef __SYNC_MUTEX_H__
#define __SYNC_MUTEX_H__

#include "simba.h"

struct mutex_t {
    /** Mutex lock state. */
    int8_t is_locked;
    /** Wait list. */
    struct thrd_prio_list_t waiters;
};

/**
 * Initialize the mutex module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code
 */
int mutex_module_init(void);

/**
 * Initialize given mutex object.
 *
 * @param[in] self_p Mutex to initialize.
 *
 * @return zero(0) or negative error code.
 */
int mutex_init(struct mutex_t *self_p);

/**
 * Lock given mutex.
 *
 * @param[in] self_p Mutex to lock.
 *
 * @return zero(0) or negative error code.
 */
int mutex_lock(struct mutex_t *self_p);

/**
 * Unlock given mutex.
 *
 * @param[in] self_p Mutex to unlock.
 *
 * @return zero(0) or negative error code.
 */
int mutex_unlock(struct mutex_t *self_p);

/**
 * Lock given mutex with the system lock taken.
 *
 * @param[in] self_p Mutex to lock.
 *
 * @return zero(0) or negative error code.
 */
int mutex_lock_isr(struct mutex_t *self_p);

/**
 * Unlock given mutex with the system lock taken.
 *
 * @param[in] self_p Mutex to unlock.
 *
 * @return zero(0) or negative error code.
 */
int mutex_unlock_isr(struct mutex_t *self_p);

#endif
