/**
 * @file kernel/sys.h
 * @version 6.0.0
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

#ifndef __KERNEL_SYS_H__
#define __KERNEL_SYS_H__

#include "simba.h"
#include "sys_port.h"

typedef uint64_t sys_tick_t;

/**
 * Convertion from the time struct to system ticks.
 */
static inline sys_tick_t t2st(struct time_t *time_p)
{
    return (((uint64_t)(time_p)->seconds * CONFIG_SYSTEM_TICK_FREQUENCY) +
            DIV_CEIL((DIV_CEIL((time_p)->nanoseconds, 1000)
                      * CONFIG_SYSTEM_TICK_FREQUENCY), 1000000));
}

/**
 * Convertion from system ticks to the time struct.
 */
static inline void st2t(sys_tick_t tick, struct time_t *time_p)
{
    time_p->seconds = (tick / CONFIG_SYSTEM_TICK_FREQUENCY);
    time_p->nanoseconds = (((1000000 * (tick % CONFIG_SYSTEM_TICK_FREQUENCY))
                            / CONFIG_SYSTEM_TICK_FREQUENCY) * 1000);
}

#define VERSION_STR STRINGIFY(VERSION)

struct sys_t {
    sys_tick_t tick;
    void (*on_fatal_callback)(int error);
    chan_t *stdin_p;
    chan_t *stdout_p;
    struct {
        uint32_t start;
        uint32_t time;
    } interrupt;
};

extern struct sys_t sys;

#define SYS_TICK_MAX (~0ULL)

/**
 * Initialize the sys module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int sys_module_init(void);

/**
 * Start the system and convert this context to the main thread.
 *
 * This function initializes a bunch of enabled features in the simba
 * platform. Many low level features (scheduling, timers, ...) are
 * always enabled, but higher level features are only enabled if
 * configured.
 *
 * This function **must** be the first function call in main().
 *
 * @return zero(0) or negative error code.
 */
int sys_start(void);

/**
 * Stop the system.
 *
 * @return Never returns.
 */
void sys_stop(int error);

/**
 * Set the on-fatal-callback function to given callback.
 *
 * The on-fatal-callback is called when a fatal error occurs. The
 * default on-fatal-callback is ``sys_stop()``.
 *
 * @param[in] callback Callback called when a fatal error occurs.
 *
 * @return void
 */
void sys_set_on_fatal_callback(void (*callback)(int error));

/**
 * Set the standard input channel.
 *
 * @param[in] chan_p New standard input channel.
 *
 * @return void.
 */
void sys_set_stdin(chan_t *chan_p);

/**
 * Get the standard input channel.
 *
 * @return Standard input channel or NULL.
 */
chan_t *sys_get_stdin(void);

/**
 * Set the standard output channel.
 *
 * @param[in] chan_p New standard output channel.
 *
 * @return void.
 */
void sys_set_stdout(chan_t *chan_p);

/**
 * Get the standard output channel.
 *
 * @return Standard output channel or NULL.
 */
chan_t *sys_get_stdout(void);

/**
 * Take the system lock. Turns off interrupts.
 *
 * @return void.
 */
void sys_lock(void);

/**
 * Release the system lock. Turn on interrupts.
 *
 * @return void.
 */
void sys_unlock(void);

/**
 * Take the system lock from isr. In many ports this has no effect.
 *
 * @return void.
 */
void sys_lock_isr(void);

/**
 * Release the system lock from isr. In many ports this function has
 * no effect.
 *
 * @return void.
 */
void sys_unlock_isr(void);

/**
 * Get a pointer to the application information buffer.
 *
 * The buffer contains various information about the application; for
 * example the application name and the build date.
 *
 * @return The pointer to the application information buffer.
 */
const FAR char *sys_get_info(void);

/**
 * Get a pointer to the application configuration buffer.
 *
 * The buffer contains a string of all configuration variables and
 * their values.
 *
 * @return The pointer to the application configuration buffer.
 */
const FAR char *sys_get_config(void);

/**
 * Get the current interrupt cpu usage counter.
 *
 * @return cpu usage, 0-100.
 */
float sys_interrupt_cpu_usage_get(void);

/**
 * Reset the interrupt cpu usage counter.
 */
void sys_interrupt_cpu_usage_reset(void);

#endif
