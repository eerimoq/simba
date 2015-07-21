/**
 * @file kernel/thrd.h
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

#ifndef __KERNEL_THRD_H__
#define __KERNEL_THRD_H__

#include "simba.h"

struct thrd_t;

/**
 * Initialize module.
 * @return zero(0) or negative error code
 */
int thrd_module_init(void);

/**
 * Spawn a thread with given entry function and argument.
 * @param[in] entry Thread entry function.
 * @param[in] arg_p Entry function argument.
 * @param[in] prio Thread scheduling priority. [ -127..127 ], where
 *                 a lower number has higher priority.
 * @param[in] stack_p Stack pointer.
 * @param[in] stack_size Stack size.
 * @return Thread id, or NULL on error.
 */
struct thrd_t *thrd_spawn(void *(*entry)(void *),
                          void *arg_p,
                          int prio,
                          void *stack_p,
                          size_t stack_size);

/**
 * Suspend given thread and wait to be resumed or timeout.
 * @param[in] timeout_p Timeout.
 * @return zero(0) or negative error code.
 */
int thrd_suspend(struct time_t *timeout_p);

/**
 * Resume given thread. If resumed thread is not yet suspended
 * it will not be suspended on next suspend attempt.
 * @param[in] thrd_p Thread id.
 * @param[in] err Error code to be returned by thrd_suspend().
 * @return zero(0) or negative error code.
 */
int thrd_resume(struct thrd_t *thrd_p, int err);

/**
 * Sleep for given number of microseconds.
 * @param[in] useconds Microseconds to sleep.
 * @return zero(0) or negative error code.
 */
int thrd_usleep(long useconds);

/**
 * Get current thread's id.
 * @return Thread id.
 */
struct thrd_t *thrd_self(void);

/**
 * Set name of given thread.
 * @param[in] name_p New thread name.
 * @return zero(0) or negative error code.
 */
int thrd_set_name(const char *name_p);

/**
 * Set log mask for given thread.
 * @param[in] thrd_p Thread.
 * @param[in] mask Log mask. See log module for available levels.
 * @return Old log mask.
 */
int thrd_set_log_mask(struct thrd_t *thrd_p, int mask);

/**
 * Get log mask for given thread.
 * @param[in] thrd Thread.
 * @return Log mask.
 */
int thrd_get_log_mask(void);

/**
 * Resume given suspended thread from interrupt context.
 * @param[in] thrd_p Thread id.
 * @param[in] err Error code to be returned by thrd_suspend().
 * @return zero(0) or negative error code.
 */
int thrd_resume_irq(struct thrd_t *thrd_p, int err);

#endif
