/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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

/**
 * Convertion from the time struct to system ticks.
 */
static inline sys_tick_t t2st(struct time_t *time_p)
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
    void (*on_fatal_callback)(int error);
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
 * @return Never returns.
 */
void sys_stop(int error);

/**
 * Reboot the system. Sets all registers to their known, default
 * values and restarts the application. Also known as a soft reset.
 *
 * @return Never returns.
 */
void sys_reboot(void);

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
void sys_set_stdin(void *chan_p);

/**
 * Get the standard input channel.
 *
 * @return Standard input channel or NULL.
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
 * @return Standard output channel or NULL.
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
 * Get a pointer to the application information buffer.
 *
 * The buffer contains various information about the application; for
 * example the application name and the build date.
 *
 * @return The pointer to the application information buffer.
 */
far_string_t sys_get_info(void);

/**
 * Get a pointer to the application configuration buffer.
 *
 * The buffer contains a string of all configuration variables and
 * their values.
 *
 * @return The pointer to the application configuration buffer.
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
