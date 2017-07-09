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

#ifndef __KERNEL_SYS_H__
#define __KERNEL_SYS_H__

#include "simba.h"
#include "sys_port.h"

#define VERSION_STR                        STRINGIFY(VERSION)
#define SYS_TICK_MAX                         ((sys_tick_t)-1)

typedef uint32_t sys_tick_t;

#if CONFIG_FLOAT == 1
typedef float cpu_usage_t;
#else
typedef uint32_t cpu_usage_t;
#endif

typedef void (*sys_on_fatal_fn_t)(int error) __attribute__ ((noreturn));

/**
 * System reset causes.
 */
enum sys_reset_cause_t {
    sys_reset_cause_unknown_t = 0,
    sys_reset_cause_power_on_t,
    sys_reset_cause_watchdog_timeout_t,
    sys_reset_cause_software_t,
    sys_reset_cause_external_t,
    sys_reset_cause_jtag_t,
#if defined(SYS_PORT_RESET_CAUSES)
    SYS_PORT_RESET_CAUSES,
#endif
    sys_reset_cause_max_t
};


/**
 * System reset cause strings map.
 */
extern const char *sys_reset_cause_string_map[sys_reset_cause_max_t];

/**
 * Convertion from the time struct to system ticks.
 */
static inline sys_tick_t t2st(const struct time_t *time_p)
{
    return (((sys_tick_t)(time_p)->seconds * CONFIG_SYSTEM_TICK_FREQUENCY) +
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

struct sys_t {
    sys_on_fatal_fn_t on_fatal_callback;
    void *stdin_p;
    void *stdout_p;
    struct {
        uint32_t start;
        uint32_t time;
    } interrupt;
};

extern struct sys_t sys;

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
 * @param[in] error Error code.
 *
 * @return Never returns.
 */
void sys_stop(int error) __attribute__ ((noreturn));

/**
 * System panic. Write given message, a backtrace and other port
 * specific debug information to the console and then reboot the
 * system.
 *
 * This function may be called from interrupt context and with the
 * system lock taken.
 *
 * @param[in] message_p Panic message to write to the console.
 *
 * @return Never returns.
 */
void sys_panic(const char *message_p) __attribute__ ((noreturn));

/**
 * Reboot the system. Also known as a soft reset.
 *
 * @return Never returns.
 */
void sys_reboot(void) __attribute__ ((noreturn));

/**
 * Store the backtrace in given buffer.
 *
 * @param[out] buf_p Buffer to store the backtrace in.
 * @param[in] size Size of the buffer.
 *
 * @return Backtrace depth.
 */
int sys_backtrace(void **buf_p, size_t size);

/**
 * Get the system reset cause.
 *
 * @return The reset cause.
 */
enum sys_reset_cause_t sys_reset_cause(void);

/**
 * Get the system uptime.
 *
 * @param[out] uptime_p System uptime.
 *
 * @return zero(0) or negative error code.
 */
int sys_uptime(struct time_t *uptime_p);

/**
 * Get the system uptime from interrupt context or with the system
 * lock taken.
 *
 * @param[out] uptime_p System uptime.
 *
 * @return zero(0) or negative error code.
 */
int sys_uptime_isr(struct time_t *uptime_p);

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
void sys_set_on_fatal_callback(sys_on_fatal_fn_t callback);

/**
 * Set the standard input channel.
 *
 * @param[in] chan_p New standard input channel.
 *
 * @return void.
 */
void sys_set_stdin(void *chan_p);

/**
 * Get the standard input channel.
 *
 * @return Standard input channel.
 */
void *sys_get_stdin(void);

/**
 * Set the standard output channel.
 *
 * @param[in] chan_p New standard output channel.
 *
 * @return void.
 */
void sys_set_stdout(void *chan_p);

/**
 * Get the standard output channel.
 *
 * @return Standard output channel.
 */
void *sys_get_stdout(void);

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
 * Get a pointer to the application information string.
 *
 * The buffer contains various information about the application; for
 * example the application name and the build date.
 *
 * @return The pointer to the application information string.
 */
far_string_t sys_get_info(void);

/**
 * Get a pointer to the application configuration string.
 *
 * The buffer contains a string of all configuration variables and
 * their values.
 *
 * @return The pointer to the application configuration string.
 */
far_string_t sys_get_config(void);

/**
 * Get the current interrupt cpu usage counter.
 *
 * @return cpu usage, 0-100.
 */
cpu_usage_t sys_interrupt_cpu_usage_get(void);

/**
 * Reset the interrupt cpu usage counter.
 */
void sys_interrupt_cpu_usage_reset(void);

#endif
