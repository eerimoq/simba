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
 * Initialize the timer module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
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
               const struct time_t *timeout_p,
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
 * expired or was never started.
 *
 * @param[in] self_p Timer object to stop.
 *
 * @return true(1) if the timer was stopped, false(0) if the timer
 *         already expired or was never started, and otherwise
 *         negative error code.
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
