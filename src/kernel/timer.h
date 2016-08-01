/**
 * @file kernel/timer.h
 * @version 4.1.0
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

#ifndef __KERNEL_TIMER_H__
#define __KERNEL_TIMER_H__

#include "simba.h"

/**
 * A timer is "single shot" per default. Initialize a timer with this
 * flag set in the ``flags`` argument to configure it as periodic.
 *
 * A periodic timer will call the function callback periodically. This
 * continues until the timer is stopped.
 */
#define TIMER_PERIODIC 0x1

/** Time callback prototype. */
typedef void (*timer_callback_t)(void *arg_p);

/* Timer. */
struct timer_t {
    struct timer_t *next_p;
    sys_tick_t delta;
    sys_tick_t timeout;
    int flags;
    timer_callback_t callback;
    void *arg_p;
};

/**
 * Initialize the timer module.
 *
 * @return zero(0) or negative error code.
 */
int timer_module_init(void);

/**
 * Initialize given timer object with given timeout and expiry
 * callback. The timer resolution directly depends on the system tick
 * frequency and is rounded up to the closest possible value. This
 * applies to both single shot and periodic timers.
 *
 * @param[in] self_p Timer object to initialize with given parameters.
 * @param[in] timeout_p The timer timeout value.
 * @param[in] callback Functaion called when the timer expires. Called
 *                     from interrupt context.
 * @param[in] arg_p Function callback argument. Passed to the callback
 *                  when the timer expires.
 * @param[in] flags Set TIMER_PERIODIC for periodic timer.
 *
 * @return zero(0) or negative error code.
 */
int timer_init(struct timer_t *self_p,
               struct time_t *timeout_p,
               timer_callback_t callback,
               void *arg_p,
               int flags);

/**
 * Start given initialized timer object.
 *
 * @param[in] self_p Timer object to start.
 *
 * @return zero(0) or negative error code.
 */
int timer_start(struct timer_t *self_p);

/**
 * See `timer_start()` for a description.
 *
 * This function may only be called from an isr or with the system
 * lock taken (see `sys_lock()`).
 */
int timer_start_isr(struct timer_t *self_p);

/**
 * Stop given timer object. This has no effect on a timer that already
 * expired or was never started. The return code is 0 if the timer was
 * stopped and -1 otherwise.
 *
 * @param[in] self_p Timer object to stop.
 *
 * @return zero(0) if the timer was stopped and -1 if the timer has
 *         already expired or was never started.
 */
int timer_stop(struct timer_t *self_p);

/**
 * See `timer_stop()` for description.
 *
 * This function may only be called from an isr or with the system
 * lock taken (see `sys_lock()`).
 */
int timer_stop_isr(struct timer_t *self_p);

#endif
