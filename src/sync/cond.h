/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

#ifndef __SYNC_COND_H__
#define __SYNC_COND_H__

#include "simba.h"

struct cond_t {
    /** Wait list. */
    struct thrd_prio_list_t waiters;
};

/**
 * Initialize the condition variable module. This function must be
 * called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code
 */
int cond_module_init(void);

/**
 * Initialize given condition variable object.
 *
 * @param[in] self_p Condition variable to initialize.
 *
 * @return zero(0) or negative error code.
 */
int cond_init(struct cond_t *self_p);

/**
 * Wait until given condition variable is unblocked or an timeout
 * occurs. Given mutex must be locked when this function is called,
 * and it is still locked when this function returns.
 *
 * @param[in] self_p Condition variable to wait for.
 * @param[in] mutex_p Mutex.
 *
 * @return zero(0) or negative error code.
 */
int cond_wait(struct cond_t *self_p,
              struct mutex_t *mutex_p,
              struct time_t *timeout_p);

/**
 * Unblock one thread waiting on given condition variable. This
 * function is a no-op if no threads are waiting on given condition
 * variable.
 *
 * @param[in] self_p Condition variable.
 *
 * @return One(1) if a thread was unblocked, zero(0) if no thread was
 *         unblocked, or negative error code.
 */
int cond_signal(struct cond_t *self_p);

/**
 * Unblock all threads waiting on given condition variable.
 *
 * @param[in] self_p Condition variable.
 *
 * @return Number of unblocked threads or negative error code.
 */
int cond_broadcast(struct cond_t *self_p);

#endif
