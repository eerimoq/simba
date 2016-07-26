/**
 * @file kernel/thrd.h
 * @version 3.1.0
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

#ifndef __KERNEL_THRD_H__
#define __KERNEL_THRD_H__

#include "simba.h"
#include "thrd_port.h"

/**
 * Macro that shall be used to declare a thread stack.
 */
#define THRD_STACK(name, size) THRD_PORT_STACK(name, size)

/**
 * Push all callee-save registers not part of the context struct. The
 * preemptive scheduler requires this macro before the
 * thrd_yield_isr() function is called from interrupt context.
 */
#define THRD_CONTEXT_STORE_ISR THRD_PORT_CONTEXT_STORE_ISR

/**
 * Pop all callee-save registers not part of the context struct. The
 * preemptive scheduler requires this macro after the thrd_yield_isr()
 * function is called from interrupt context.
 */
#define THRD_CONTEXT_LOAD_ISR THRD_PORT_CONTEXT_LOAD_ISR

/**
 * Reschuedule from isr. Used by preemptive systems to interrupt low
 * priority threads in favour of high priority threads.
 */
#define THRD_RESCHEDULE_ISR                     \
    do {                                        \
        THRD_CONTEXT_STORE_ISR;                 \
        thrd_yield_isr();                       \
        THRD_CONTEXT_LOAD_ISR;                  \
    } while (0)    

struct thrd_parent_t {
    struct thrd_t *next_p;
    struct thrd_t *thrd_p;
};

struct thrd_t {
    struct thrd_t *prev_p;
    struct thrd_t *next_p;
    struct thrd_port_t port;
    int prio;
    int state;
    int err;
    int log_mask;
    struct timer_t *timer_p;
    const char *name_p;
    struct thrd_parent_t parent;
    struct list_singly_linked_t children;
    struct {
        float usage;
    } cpu;
#if CONFIG_PROFILE_STACK == 1
    size_t stack_size;
#endif
#if CONFIG_ASSERT == 1
    uint16_t stack_low_magic;
#endif
};

/**
 * Initialize the thread module.
 *
 * @return zero(0) or negative error code
 */
int thrd_module_init(void);

/**
 * Spawn a thread with given main (entry) function and argument. The
 * thread is initialized and added to the ready queue in the scheduler
 * for execution when prioritized.
 *
 * @param[in] main Thread main (entry) function. This function
 *                 normally contains an infinate loop waiting for
 *                 events to occur.
 * @param[in] arg_p Main function argument. Passed as arg_p to the
 *                  main function.
 * @param[in] prio Thread scheduling priority. [-127..127], where -127
 *                 is the highest priority and 127 is the lowest.
 * @param[in] stack_p Stack pointer. The pointer to a stack created
 *                    with the macro `THRD_STACK()`.
 * @param[in] stack_size The stack size in number of bytes.
 *
 * @return Thread id, or NULL on error.
 */
struct thrd_t *thrd_spawn(void *(*main)(void *),
                          void *arg_p,
                          int prio,
                          void *stack_p,
                          size_t stack_size);

/**
 * Suspend current thread and wait to be resumed or a timeout occurs
 * (if given).
 *
 * @param[in] timeout_p Time to wait to be resumed before a timeout
 *                      occurs and the function returns.
 *
 * @return zero(0), -ETIMEOUT on timeout or other negative error code.
 */
int thrd_suspend(struct time_t *timeout_p);

/**
 * Resume given thread. If resumed thread is not yet suspended it will
 * not be suspended on next suspend call to `thrd_suspend()` or
 * `thrd_suspend_isr()`.
 *
 * @param[in] thrd_p Thread id to resume.
 * @param[in] err Error code to be returned by `thrd_suspend()` or
 *                `thrd_suspend_isr()`.
 *
 * @return zero(0) or negative error code.
 */
int thrd_resume(struct thrd_t *thrd_p, int err);

/**
 * Put the currently executing thread on the ready list and
 * reschedule.
 *
 * This function is often called periodically from low priority work
 * heavy threads to give higher priority threads the chance to
 * execute.
 *
 * @return zero(0) or negative error code.
 */
int thrd_yield(void);

/**
 * Wait for given thread to terminate.
 *
 * @param[in] thrd_p Thread to wait for.
 *
 * @return zero(0) or negative error code.
 */
int thrd_join(struct thrd_t *thrd_p);

/**
 * Pauses the current thread for given number of microseconds.
 *
 * @param[in] microseconds Microseconds to sleep.
 *
 * @return zero(0) or negative error code.
 */
int thrd_sleep_us(long microseconds);

/**
 * Get current thread's id.
 *
 * @return Thread id.
 */
struct thrd_t *thrd_self(void);

/**
 * Set the name of the current thread.
 *
 * @param[in] name_p New thread name.
 *
 * @return zero(0) or negative error code.
 */
int thrd_set_name(const char *name_p);

/**
 * Get the name of the current thread.
 *
 * @return Current thread name.
 */
const char *thrd_get_name(void);

/**
 * Set the log mask of given thread.
 *
 * @param[in] thrd_p Thread to set the log mask of.
 * @param[in] mask Log mask. See the log module for available levels.
 *
 * @return Old log mask.
 */
int thrd_set_log_mask(struct thrd_t *thrd_p, int mask);

/**
 * Get the log mask of the current thread.
 *
 * @return Log mask of current thread.
 */
int thrd_get_log_mask(void);

/**
 * Suspend current thread with the system lock taken (see
 * `sys_lock()`) and wait to be resumed or a timeout occurs (if
 * given).
 *
 * @param[in] timeout_p Time to wait to be resumed before a timeout
 *                      occurs and the function returns.
 *
 * @return zero(0), -ETIMEOUT on timeout or other negative error code.
 */
int thrd_suspend_isr(struct time_t *timeout_p);

/**
 * Resume given thread from isr or with the system lock taken (see
 * `sys_lock()`). If resumed thread is not yet suspended it will not
 * be suspended on next suspend call to `thrd_suspend()` or
 * `thrd_suspend_isr()`.
 *
 * @param[in] thrd_p Thread id to resume.
 * @param[in] err Error code to be returned by `thrd_suspend()` or
 *                `thrd_suspend_isr()`.
 *
 * @return zero(0) or negative error code.
 */
int thrd_resume_isr(struct thrd_t *thrd_p, int err);

/**
 * Yield current thread from isr (preemptive scheduler only) or with
 * the system lock taken.
 *
 * @return zero(0) or negative error code.
 */
int thrd_yield_isr(void);

#endif
