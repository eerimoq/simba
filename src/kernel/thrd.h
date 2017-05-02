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

#ifndef __KERNEL_THRD_H__
#define __KERNEL_THRD_H__

#include "simba.h"
#include "thrd_port.h"

/**
 * Macro to declare a thread stack with given name and size. All
 * thread stacks must be defined using this macro.
 *
 * @param[in] name The name of the stack. A variable is declared with
 *                 this name that should be passed to `thrd_spawn()`.
 * @param[in] size Size of the stack in bytes.
 */
#define THRD_STACK(name, size) THRD_PORT_STACK(name, size)

/**
 * Push all callee-save registers not part of the context struct. The
 * preemptive scheduler requires this macro before the
 * `thrd_yield_isr()` function is called from interrupt context.
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


/**
 * A thread environment variable.
 */
struct thrd_environment_variable_t {
    const char *name_p;
    const char *value_p;
};

struct thrd_environment_t {
    struct thrd_environment_variable_t *variables_p;
    size_t number_of_variables;
    size_t max_number_of_variables;
};

struct thrd_t {
    struct {
        struct thrd_prio_list_elem_t elem;
    } scheduler;
    struct thrd_port_t port;
    int prio;
    int state;
    int err;
    int log_mask;
    struct timer_t *timer_p;
    const char *name_p;
    struct thrd_t *next_p;
# if CONFIG_THRD_TERMINATE == 1
    struct sem_t join_sem;
#endif
    struct {
#if CONFIG_THRD_CPU_USAGE == 1
        struct {
#if CONFIG_FLOAT == 1
            float usage;
#else
            int usage;
#endif
        } cpu;
#endif
#if CONFIG_THRD_SCHEDULED == 1
        uint32_t scheduled;
#endif
    } statistics;
#if CONFIG_THRD_ENV == 1
    struct thrd_environment_t env;
#endif
    size_t stack_size;
#if CONFIG_PANIC_ASSERT == 1
    uint16_t stack_low_magic;
#endif
};

/**
 * Initialize the thread module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
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
int thrd_suspend(const struct time_t *timeout_p);

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
 * Pauses the current thread for given number of seconds.
 *
 * @param[in] seconds Seconds to sleep.
 *
 * @return zero(0) or negative error code.
 */
int thrd_sleep(float seconds);

/**
 * Pauses the current thread for given number of milliseconds.
 *
 * @param[in] ms Milliseconds to sleep.
 *
 * @return zero(0) or negative error code.
 */
int thrd_sleep_ms(int ms);

/**
 * Pauses the current thread for given number of microseconds.
 *
 * @param[in] us Microseconds to sleep.
 *
 * @return zero(0) or negative error code.
 */
int thrd_sleep_us(long us);

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
 * Get the pointer to given thread.
 *
 * @return Thraed pointer or NULL if the thread was not found.
 */
struct thrd_t *thrd_get_by_name(const char *name_p);

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
 * Set the priority of given thread.
 *
 * @param[in] thrd_p Thread to set the priority for.
 * @param[in] prio Priority.
 *
 * @return zero(0) or negative error code.
 */
int thrd_set_prio(struct thrd_t *thrd_p, int prio);

/**
 * Get the priority of the current thread.
 *
 * @return Priority of current thread.
 */
int thrd_get_prio(void);

/**
 * Initialize the global environment variables storage. These
 * variables are shared among all threads.
 *
 * @param[in] variables_p Variables array.
 * @param[in] length Length of the variables array.
 *
 * @return zero(0) or negative error code.
 */
int thrd_init_global_env(struct thrd_environment_variable_t *variables_p,
                         int length);

/**
 * Set the value of given environment variable. The pointers to given
 * name and value are stored in the current global environment array.
 *
 * @param[in] name_p Name of the environment variable to set.
 * @param[in] value_p Value of the environment variable. Set to NULL
 *                    to remove the variable.
 *
 * @return zero(0) or negative error code.
 */
int thrd_set_global_env(const char *name_p, const char *value_p);

/**
 * Get the value of given environment variable in the global
 * environment array.
 *
 * @param[in] name_p Name of the environment variable to get.
 *
 * @return Value of given environment variable or NULL if it is not
 *         found.
 */
const char *thrd_get_global_env(const char *name_p);

/**
 * Initialize the current threads' environment variables storage.
 *
 * @param[in] variables_p Variables are to be used by this therad.
 * @param[in] length Length of the variables array.
 *
 * @return zero(0) or negative error code.
 */
int thrd_init_env(struct thrd_environment_variable_t *variables_p,
                  int length);

/**
 * Set the value of given environment variable. The pointers to given
 * name and value are stored in the current threads' environment
 * array.
 *
 * @param[in] name_p Name of the environment variable to set.
 * @param[in] value_p Value of the environment variable. Set to NULL
 *                    to remove the variable.
 *
 * @return zero(0) or negative error code.
 */
int thrd_set_env(const char *name_p, const char *value_p);

/**
 * Get the value of given environment variable. If given variable is
 * not found in the current threads' environment array, the
 * global environment array is searched.
 *
 * @param[in] name_p Name of the environment variable to get.
 *
 * @return Value of given environment variable or NULL if it is not
 *         found.
 */
const char *thrd_get_env(const char *name_p);

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
int thrd_suspend_isr(const struct time_t *timeout_p);

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

/**
 * Allocate a thread stack of given size.
 *
 * @return The pointer to allocated thread stack, or NULL on error.
 */
void *thrd_stack_alloc(size_t size);

/**
 * Free given thread stack.
 *
 * @return zero(0) or negative error code.
 */
int thrd_stack_free(void *stack_p);

/**
 * Get the pointer to given threads' bottom of stack.
 *
 * @return The pointer to given threds' bottom of stack, or NULL on
 * error.
 */
const void *thrd_get_bottom_of_stack(struct thrd_t *thrd_p);

/**
 * Get the pointer to given threads' top of stack.
 *
 * @return The pointer to given threds' top of stack, or NULL on
 * error.
 */
const void *thrd_get_top_of_stack(struct thrd_t *thrd_p);

/**
 * Initialize given prio list.
 */
int thrd_prio_list_init(struct thrd_prio_list_t *self_p);

/**
 * Push given element on given priority list. The priority list is a
 * linked list with the highest priority thread first. The pushed
 * element is added _after_ any already pushed elements with the same
 * thread priority.
 *
 * @param[in] self_p Priority list to push on.
 * @param[in] elem_p Element to push.
 *
 * @return void.
 */
void thrd_prio_list_push_isr(struct thrd_prio_list_t *self_p,
                             struct thrd_prio_list_elem_t *elem_p);

/**
 * Pop the highest priority element from given priority list.
 *
 * @param[in] self_p Priority list to pop from.
 *
 * @return Poped element or NULL if the list was empty.
 */
struct thrd_prio_list_elem_t *thrd_prio_list_pop_isr(
    struct thrd_prio_list_t *self_p);

/**
 * Remove given element from given priority list.
 *
 * @param[in] self_p Priority list to remove given element from.
 * @param[in] elem_p Element to remove.
 *
 * @return zero(0) or negative error code.
 */
int thrd_prio_list_remove_isr(struct thrd_prio_list_t *self_p,
                              struct thrd_prio_list_elem_t *elem_p);

#endif
