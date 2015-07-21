/**
 * @file kernel/sys.h
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

#ifndef __KERNEL_SYS_H__
#define __KERNEL_SYS_H__

#include "simba.h"
#include "sys_port.h"

/* Default system tick frequency. */
#ifndef SYS_TICK_FREQUENCY
#    define SYS_TICK_FREQUENCY 100
#endif

/* Convertion macro for time to system ticks. */
#define T2ST(time)                                                      \
    (((time)->seconds * SYS_TICK_FREQUENCY) +                           \
     (((time)->nanoseconds / 1000) * SYS_TICK_FREQUENCY) / 1000000)

typedef unsigned long sys_tick_t;

struct sys_t {
    sys_tick_t tick;
    void (*on_fatal_callback)(int error);
    chan_t *std_out_p;
};

extern struct sys_t sys;

#define SYS_TICK_MAX (~0)

/**
 * Initialize module.
 * @return zero(0) or negative error code.
 */
int sys_module_init(void);

/**
 * Start system.
 * @return zero(0) or negative error code.
 */
int sys_start(void);

/**
 * Stop system.
 * @return Never returns.
 */
void sys_stop(int error);

/**
 * Set on fatal callback.
 * @param[in] callback Called on fatal error.
 */
void sys_set_on_fatal_callback(void (*callback)(int error));

/**
 * Set standard output channel.
 * @param[in] chan Standard output channel.
 * @return void.
 */
void sys_set_stdout(chan_t *chan);

/**
 * Get standard output channel.
 * @return Standard output channel or NULL.
 */
chan_t *sys_get_stdout(void);

#endif
