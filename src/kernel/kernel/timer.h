/**
 * @file kernel/timer.h
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

#ifndef __KERNEL_TIMER_H__
#define __KERNEL_TIMER_H__

#include "simba.h"

/* Flags to `timer_set()`. */
#define TIMER_PERIODIC 0x1

/* Timer. */
struct timer_t {
    struct timer_t *next_p;
    sys_tick_t delta;
    sys_tick_t timeout;
    int flags;
    void (*callback)(void *arg_p);
    void *arg_p;
};

/**
 * Set a timer with given timeout and expiry callback. The timer
 * resolution directly depends on the system tick frequency. The
 * timeout is rounded up to the closest system tick. This applies to
 * both single shot and periodic timers.
 *
 * @param[in] self_p Timer object to be started with given timeout.
 * @param[in] timeout_p Time until timer expiry.
 * @param[in] callback Timer expiry callback. Called from interrupt
 *                     context.
 * @param[in] arg_p Timer expiry callback argument.
 * @param[in] flags Set TIMER_PERIODIC for periodic timer.
 *
 * @return zero(0) or negative error code.
 */
int timer_set(struct timer_t *self_p,
              struct time_t *timeout_p,
              void (*callback)(void *arg_p),
              void *arg_p,
              int flags);

/**
 * Cancel the timer.
 *
 * @param[in] self_p Timer object.
 *
 * @return zero(0) or negative error code.
 */
int timer_cancel(struct timer_t *self_p);

/**
 * See `timer_set()` for a description.
 *
 * This function may only be called with the system lock taken (see
 * `sys_lock()`).
 */
int timer_set_isr(struct timer_t *self_p,
                  struct time_t *timeout_p,
                  void (*callback)(void *arg_p),
                  void *arg_p,
                  int flags);

#endif
