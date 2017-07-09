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

#include "simba.h"

/* Thread states. */
enum thrd_state_t {
    THRD_STATE_CURRENT = 0,
    THRD_STATE_READY,
    THRD_STATE_SUSPENDED,
    THRD_STATE_RESUMED,
    THRD_STATE_TERMINATED
};

/* Stack usage and debugging. */
#define THRD_STACK_LOW_MAGIC      0x1337
#define THRD_FILL_PATTERN           0x19

struct module_t {
    int initialized;
    struct {
        struct thrd_t *current_p;
        struct thrd_prio_list_t ready;
    } scheduler;
    struct thrd_t *threads_p;
#if CONFIG_THRD_ENV == 1
    struct {
        struct thrd_environment_variable_t global_variables[4];
        struct thrd_environment_t global;
        struct sem_t sem;
    } env;
#endif
#if CONFIG_FS_CMD_THRD_LIST == 1
    struct fs_command_t cmd_list;
#endif
#if CONFIG_FS_CMD_THRD_SET_LOG_MASK == 1
    struct fs_command_t cmd_set_log_mask;
#endif
#if CONFIG_MONITOR_THREAD == 1
    struct fs_command_t cmd_monitor_set_period_ms;
    struct fs_command_t cmd_monitor_set_print;
#endif
};

static struct module_t module;

#if CONFIG_THRD_STACK_HEAP == 1
static struct heap_t stack_heap;
static THRD_STACK(stack_heap_buffer, CONFIG_THRD_STACK_HEAP_SIZE);
static size_t stack_heap_fixed_buffer_sizes[HEAP_FIXED_SIZES_MAX] = { 0, };
#endif

#if CONFIG_THRD_ENV == 1

static int set_env(struct thrd_environment_t *env_p,
                   const char *name_p,
                   const char *value_p)
{
    int i;
    struct thrd_environment_variable_t *variable_p;

    variable_p = &env_p->variables_p[0];

    /* Does the variable already exist in the list? */
    for (i = 0; i < env_p->number_of_variables; i++) {
        if (strcmp(variable_p->name_p, name_p) == 0) {
            if (value_p != NULL) {
                /* Replace the value. */
                variable_p->value_p = value_p;
            } else {
                /* Remove the variable. */
                env_p->number_of_variables--;
                *variable_p = env_p->variables_p[env_p->number_of_variables];
            }

            return (0);
        }

        variable_p++;
    }

    /* New variable. Is there free space? */
    if (env_p->number_of_variables == env_p->max_number_of_variables) {
        return (-1);
    }

    /* Set the new variable. */
    variable_p->name_p = name_p;
    variable_p->value_p = value_p;
    env_p->number_of_variables++;

    return (0);
}

static const char *get_env(struct thrd_environment_t *env_p,
                           const char *name_p)
{
    int i;
    struct thrd_environment_variable_t *variable_p;

    variable_p = &env_p->variables_p[0];

    for (i = 0; i < env_p->number_of_variables; i++) {
        if (strcmp(variable_p->name_p, name_p) == 0) {
            return (variable_p->value_p);
        }

        variable_p++;
    }

    return (NULL);
}

#endif

/* Forward declarations for thrd_port. */
static void scheduler_ready_push(struct thrd_t *thrd_p);

static void thrd_reschedule(void);

void terminate(void);

#include "thrd_port.i"

#if CONFIG_MONITOR_THREAD == 1 && CONFIG_THRD_CPU_USAGE == 1
#    include "thrd/thrd_monitor.i"
#endif

/* Stacks. */
static THRD_STACK(idle_thrd_stack, CONFIG_THRD_IDLE_STACK_SIZE);

/**
 * The thread is terminated.
 */
void terminate(void)
{
#if CONFIG_THRD_TERMINATE == 1
    struct thrd_t *thrd_p, *prev_p;

    /* Remove the thread from the global list of threads. */
    sys_lock();

    thrd_p = module.threads_p;
    prev_p = NULL;

    while (thrd_p != NULL) {
        if (thrd_p == thrd_self()) {
            if (prev_p != NULL) {
                prev_p->next_p = thrd_p->next_p;
            } else {
                module.threads_p = thrd_p->next_p;
            }

            break;
        }

        prev_p = thrd_p;
        thrd_p = thrd_p->next_p;
    }

    sem_give_isr(&thrd_self()->join_sem, 1);
    thrd_self()->state = THRD_STATE_TERMINATED;
    thrd_reschedule();

    /* Should never come here. */
#endif

    sys_stop(1);
}

/**
 * Timer callback for threads suspended with a timeout.
 */
static void on_suspend_timer_expired(void *arg_p)
{
    struct thrd_t *thrd_p;

    thrd_p = arg_p;

    /* The timer is no longer in use. */
    thrd_p->timer_p = NULL;

    /* Push thread on scheduler ready queue. */
    thrd_p->err = -ETIMEDOUT;
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);

    thrd_port_on_suspend_timer_expired(thrd_p);
}

/**
 * Push a thread on the list of threads that are ready to be
 * scheduled.
 *
 * @param[in] thrd_p Thread to push to the the ready list.
 *
 * @return void.
 */
static void scheduler_ready_push(struct thrd_t *thrd_p)
{
    thrd_prio_list_push_isr(&module.scheduler.ready, &thrd_p->scheduler.elem);
}

/**
 * Pop the most important thread from the ready list.
 *
 * @return Thread to swap to.
 */
static struct thrd_t *scheduler_ready_pop(void)
{
    return (thrd_prio_list_pop_isr(&module.scheduler.ready)->thrd_p);
}

/**
 * Perform a rescheduling to let the currently most important thread
 * to run.
 *
 * This function must be called with the system lock taken or from an
 * isr.
 */
static void thrd_reschedule(void)
{
    struct thrd_t *in_p, *out_p;

    out_p = thrd_self();

    PANIC_ASSERTN(out_p->stack_low_magic == THRD_STACK_LOW_MAGIC, ESTACK);

    in_p = scheduler_ready_pop();

    /* Swap threads. */
    in_p->state = THRD_STATE_CURRENT;

    if (in_p != out_p) {
        module.scheduler.current_p = in_p;
        thrd_port_cpu_usage_stop(out_p);
        thrd_port_cpu_usage_start(in_p);
        thrd_port_swap(in_p, out_p);
#if CONFIG_THRD_SCHEDULED == 1
        out_p->statistics.scheduled++;
#endif
    }
}

#if CONFIG_PROFILE_STACK == 1

static void thrd_fill_pattern(char *from_p, size_t size)
{
  size_t i;

  for (i = 0; i < size; i++) {
    from_p[i] = THRD_FILL_PATTERN;
  }
}

#if CONFIG_FS_CMD_THRD_LIST == 1

static int thrd_get_used_stack(struct thrd_t *thrd_p)
{
  char *stack_p;
  size_t i;

  stack_p = (char *)&thrd_p[1];
  i = 0;

  /* Stack grows towards lower memory addresses, so start from the
     bottom.*/
  while ((i < thrd_p->stack_size) &&
         (stack_p[i] == THRD_FILL_PATTERN)) {
      i++;
  }

  return (thrd_p->stack_size - i);
}

#endif

#endif

#if CONFIG_FS_CMD_THRD_LIST == 1

static char *state_fmt[] = {
    "current",
    "ready",
    "suspended",
    "resumed",
    "terminated"
};

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct thrd_t *thrd_p;

    std_fprintf(chout_p,
                OSTR("                NAME        STATE  PRIO"
#if CONFIG_THRD_CPU_USAGE == 1
                     "   CPU"
#endif
#if CONFIG_THRD_SCHEDULED == 1
                     "   SCHEDULED"
#endif
#if CONFIG_PROFILE_STACK == 1
                     "  MAX-STACK-USAGE"
#endif
                     "  LOGMASK\r\n"));

    thrd_p = module.threads_p;

    while (thrd_p != NULL) {
        std_fprintf(chout_p,
                    OSTR("%20s %12s %5d"
#if CONFIG_THRD_CPU_USAGE == 1
                         " %4u%%"
#endif
#if CONFIG_THRD_SCHEDULED == 1
                         " %11u"
#endif
#if CONFIG_PROFILE_STACK == 1
                         "    %6d/%6d"
#endif
                         "     0x%02x\r\n"),
                    thrd_p->name_p,
                    state_fmt[thrd_p->state], thrd_p->prio,
#if CONFIG_THRD_CPU_USAGE == 1
#    if CONFIG_FLOAT == 1
                    (unsigned int)(thrd_p->statistics.cpu.usage + 0.5),
#    else
                    (unsigned int)thrd_p->statistics.cpu.usage,
#    endif
#endif
#if CONFIG_THRD_SCHEDULED == 1
                    (unsigned int)thrd_p->statistics.scheduled,
#endif
#if CONFIG_PROFILE_STACK == 1
                    thrd_get_used_stack(thrd_p),
                    (int)thrd_p->stack_size,
#endif
                    thrd_p->log_mask);

        thrd_p = thrd_p->next_p;
    }

    return (0);
}

#endif

#if CONFIG_FS_CMD_THRD_SET_LOG_MASK == 1

static int cmd_set_log_mask_cb(int argc,
                               const char *argv[],
                               void *chout_p,
                               void *chin_p,
                               void *arg_p,
                               void *call_arg_p)
{
    struct thrd_t *thrd_p;
    long mask;

    if (argc != 3) {
        std_fprintf(chout_p,
                    OSTR("Usage: set_log_mask <thread name> <log mask>\r\n"));
        return (-EINVAL);
    }

    thrd_p = thrd_get_by_name(argv[1]);

    if (thrd_p == NULL) {
        return (-ESRCH);
    }

    if (std_strtol(argv[2], &mask) == NULL) {
        return (-EINVAL);
    }

    thrd_p->log_mask = mask;

    return (0);
}

#endif

static void *idle_thrd(void *arg_p)
{
    struct thrd_t *thrd_p;

    thrd_set_name("idle");

    thrd_p = thrd_self();

    while (1) {
        thrd_port_idle_wait(thrd_p);
    }

    return (NULL);
}

int thrd_module_init(void)
{
    struct thrd_t *thrd_p;

    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    thrd_prio_list_init(&module.scheduler.ready);

#if CONFIG_THRD_STACK_HEAP == 1
    heap_init(&stack_heap,
              &stack_heap_buffer[0],
              sizeof(stack_heap_buffer),
              &stack_heap_fixed_buffer_sizes[0]);
#endif

#if CONFIG_THRD_ENV == 1
    module.env.global.variables_p = module.env.global_variables;
    module.env.global.number_of_variables = 0;
    module.env.global.max_number_of_variables =
        membersof(module.env.global_variables);
    sem_init(&module.env.sem, 0, 1);
#endif

#if CONFIG_PROFILE_STACK == 1
    char dummy = 0;
#endif

    /* Main function becomes a thrd. */
    thrd_p = thrd_port_get_main_thrd();
    thrd_p->scheduler.elem.thrd_p = thrd_p;
    thrd_p->prio = 0;
    thrd_p->state = THRD_STATE_CURRENT;
    thrd_p->err = 0;
    thrd_p->log_mask = CONFIG_THRD_DEFAULT_LOG_MASK;
    thrd_p->timer_p = NULL;
    thrd_p->name_p = "main";
    thrd_p->next_p = NULL;
    thrd_p->stack_size = (thrd_port_get_main_thrd_stack_top() - (char *)(thrd_p + 1));

#if CONFIG_THRD_TERMINATE == 1
    sem_init(&thrd_p->join_sem, 1, 1);
#endif

#if CONFIG_THRD_CPU_USAGE == 1
    thrd_p->statistics.cpu.usage = 0.0f;
#endif

#if CONFIG_THRD_SCHEDULED == 1
    thrd_p->statistics.scheduled = 0;
#endif

#if CONFIG_THRD_ENV == 1
    thrd_p->env.variables_p = NULL;
    thrd_p->env.number_of_variables = 0;
    thrd_p->env.max_number_of_variables = 0;
#endif

#if CONFIG_PANIC_ASSERT == 1
    thrd_p->stack_low_magic = THRD_STACK_LOW_MAGIC;
#endif

#if CONFIG_PROFILE_STACK == 1
    thrd_fill_pattern((char *)(thrd_p + 1), &dummy - (char *)(thrd_p + 2));
#endif

    module.scheduler.current_p = thrd_p;
    module.threads_p = thrd_p;

    thrd_port_init_main(&thrd_p->port);
    thrd_spawn(idle_thrd, NULL, 127, idle_thrd_stack, sizeof(idle_thrd_stack));

#if CONFIG_MONITOR_THREAD == 1
    thrd_spawn(monitor_thrd,
               NULL,
               THRD_MONITOR_PRIO,
               monitor_thrd_stack,
               sizeof(monitor_thrd_stack));
#endif

#if CONFIG_FS_CMD_THRD_LIST == 1
    fs_command_init(&module.cmd_list,
                    CSTR("/kernel/thrd/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);
#endif

#if CONFIG_FS_CMD_THRD_SET_LOG_MASK == 1
    fs_command_init(&module.cmd_set_log_mask,
                    CSTR("/kernel/thrd/set_log_mask"),
                    cmd_set_log_mask_cb,
                    NULL);
    fs_command_register(&module.cmd_set_log_mask);
#endif

#if CONFIG_MONITOR_THREAD == 1
    fs_command_init(&module.cmd_monitor_set_period_ms,
                    CSTR("/kernel/thrd/monitor/set_period_ms"),
                    cmd_monitor_set_period_ms_cb,
                    NULL);
    fs_command_register(&module.cmd_monitor_set_period_ms);

    fs_command_init(&module.cmd_monitor_set_print,
                    CSTR("/kernel/thrd/monitor/set_print"),
                    cmd_monitor_set_print_cb,
                    NULL);
    fs_command_register(&module.cmd_monitor_set_print);
#endif

    return (0);
}

struct thrd_t *thrd_spawn(void *(*main)(void *),
                          void *arg_p,
                          int prio,
                          void *stack_p,
                          size_t stack_size)
{
    ASSERTNRN(main != NULL, EINVAL);
    ASSERTNRN(stack_p != NULL, EINVAL);
    ASSERTNRN(stack_size > sizeof(struct thrd_t) + 1, EINVAL);

    struct thrd_t *thrd_p;
    int res = 0;

    /* Initialize thrd structure in the beginning of the stack. */
    thrd_p = stack_p;
    thrd_p->scheduler.elem.thrd_p = thrd_p;
    thrd_p->prio = prio;
    thrd_p->state = THRD_STATE_READY;
    thrd_p->err = 0;
    thrd_p->log_mask = CONFIG_THRD_DEFAULT_LOG_MASK;
    thrd_p->timer_p = NULL;
    thrd_p->name_p = "";
    thrd_p->stack_size = (stack_size - sizeof(*thrd_p));

#if CONFIG_THRD_TERMINATE == 1
    sem_init(&thrd_p->join_sem, 1, 1);
#endif

#if CONFIG_THRD_CPU_USAGE == 1
    thrd_p->statistics.cpu.usage = 0.0f;
#endif

#if CONFIG_THRD_SCHEDULED == 1
    thrd_p->statistics.scheduled = 0;
#endif

#if CONFIG_THRD_ENV == 1
    thrd_p->env.variables_p = NULL;
    thrd_p->env.number_of_variables = 0;
    thrd_p->env.max_number_of_variables = 0;
#endif

#if CONFIG_PANIC_ASSERT == 1
    thrd_p->stack_low_magic = THRD_STACK_LOW_MAGIC;
#endif

#if CONFIG_PROFILE_STACK == 1
    thrd_fill_pattern((char *)(thrd_p + 1), thrd_p->stack_size);
#endif

    thrd_p->next_p = module.threads_p;
    module.threads_p = thrd_p;

    res = thrd_port_spawn(thrd_p, main, arg_p, stack_p, stack_size);

    sys_lock();
    scheduler_ready_push(thrd_p);
    sys_unlock();

    return (res == 0 ? thrd_p : NULL);
}

int thrd_suspend(const struct time_t *timeout_p)
{
    int res;

    sys_lock();
    res = thrd_suspend_isr(timeout_p);
    sys_unlock();

    return (res);
}

int thrd_resume(struct thrd_t *thrd_p, int err)
{
    ASSERTN(thrd_p != NULL, EINVAL);

    int res;

    sys_lock();
    res = thrd_resume_isr(thrd_p, err);
    sys_unlock();

    return (res);
}

int thrd_yield(void)
{
    int res;

    sys_lock();
    res = thrd_yield_isr();
    sys_unlock();

    return (res);
}

int thrd_join(struct thrd_t *thrd_p)
{
    ASSERTN(thrd_p != NULL, EINVAL);

#if CONFIG_THRD_TERMINATE == 1
    sem_take(&thrd_p->join_sem, NULL);
    sem_give(&thrd_p->join_sem, 1);

    return (0);
#else
    return (-1);
#endif
}

int thrd_sleep(float seconds)
{
    return (thrd_sleep_us(1000000 * seconds));
}

int thrd_sleep_ms(int ms)
{
    return (thrd_sleep_us(1000L * ms));
}

int thrd_sleep_us(long us)
{
    struct time_t timeout;
    int res;

    timeout.seconds = (us / 1000000);
    timeout.nanoseconds = 1000 * (us % 1000000);
    res = thrd_suspend(&timeout);

    return (res == -ETIMEDOUT ? 0 : -1);
}

struct thrd_t *thrd_self(void)
{
    return (module.scheduler.current_p);
}

int thrd_set_name(const char *name_p)
{
    thrd_self()->name_p = name_p;

    return (0);
}

const char *thrd_get_name()
{
    return (thrd_self()->name_p);
}

struct thrd_t *thrd_get_by_name(const char *name_p)
{
    struct thrd_t *thrd_p;

    thrd_p = module.threads_p;

    while (thrd_p != NULL) {
        if (strcmp(thrd_p->name_p, name_p) == 0) {
            return (thrd_p);
        }

        thrd_p = thrd_p->next_p;
    }

    return (NULL);
}

int thrd_set_log_mask(struct thrd_t *thrd_p, int mask)
{
    ASSERTN(thrd_p != NULL, EINVAL);

    int old;

    old = thrd_p->log_mask;
    thrd_p->log_mask = mask;

    return (old);
}

int thrd_get_log_mask(void)
{
    return (module.scheduler.current_p->log_mask);
}

int thrd_set_prio(struct thrd_t *thrd_p, int prio)
{
    ASSERTN(thrd_p != NULL, EINVAL);

    thrd_p->prio = prio;

    return (0);
}

int thrd_get_prio(void)
{
    return (module.scheduler.current_p->prio);
}

int thrd_init_global_env(struct thrd_environment_variable_t *variables_p,
                         int length)
{
#if CONFIG_THRD_ENV == 1
    sem_take(&module.env.sem, NULL);
    module.env.global.variables_p = variables_p;
    module.env.global.number_of_variables = 0;
    module.env.global.max_number_of_variables = length;
    sem_give(&module.env.sem, 1);

    return (0);
#else
    return (-1);
#endif
}

int thrd_set_global_env(const char *name_p, const char *value_p)
{
    ASSERTN(name_p != NULL, EINVAL);

#if CONFIG_THRD_ENV == 1
    int res;

    sem_take(&module.env.sem, NULL);
    res = set_env(&module.env.global, name_p, value_p);
    sem_give(&module.env.sem, 1);

    return (res);
#else
    return (-1);
#endif
}

const char *thrd_get_global_env(const char *name_p)
{
    ASSERTNRN(name_p != NULL, EINVAL);

#if CONFIG_THRD_ENV == 1
    const char *value_p;

    sem_take(&module.env.sem, NULL);
    value_p  = get_env(&module.env.global, name_p);
    sem_give(&module.env.sem, 1);

    return (value_p);
#else
    return (NULL);
#endif
}

int thrd_init_env(struct thrd_environment_variable_t *variables_p,
                  int length)
{
#if CONFIG_THRD_ENV == 1
    module.scheduler.current_p->env.variables_p = variables_p;
    module.scheduler.current_p->env.number_of_variables = 0;
    module.scheduler.current_p->env.max_number_of_variables = length;

    return (0);
#else
    return (-1);
#endif
}

int thrd_set_env(const char *name_p, const char *value_p)
{
    ASSERTN(name_p != NULL, EINVAL);

#if CONFIG_THRD_ENV == 1
    return (set_env(&module.scheduler.current_p->env, name_p, value_p));
#else
    return (-1);
#endif
}

const char *thrd_get_env(const char *name_p)
{
    ASSERTNRN(name_p != NULL, EINVAL);

#if CONFIG_THRD_ENV == 1
    const char *value_p;

    value_p = get_env(&module.scheduler.current_p->env, name_p);

    if (value_p != NULL) {
        return (value_p);
    }

    return (thrd_get_global_env(name_p));
#else
    return (NULL);
#endif
}

void RAM_CODE thrd_tick_isr(void)
{
    thrd_port_tick();

#if CONFIG_PREEMPTIVE_SCHEDULER == 1
    THRD_RESCHEDULE_ISR;
#endif
}

int thrd_suspend_isr(const struct time_t *timeout_p)
{
    struct thrd_t *thrd_p;
    struct timer_t timer;

    thrd_p = thrd_self();

    /* Immediatly return if the thread is already resumed. */
    if (thrd_p->state == THRD_STATE_RESUMED) {
        thrd_p->state = THRD_STATE_READY;
        scheduler_ready_push(thrd_p);
    } else {
        thrd_p->state = THRD_STATE_SUSPENDED;

        if (timeout_p != NULL) {
            if ((timeout_p->seconds <= 0) && (timeout_p->nanoseconds <= 0)) {
                return (-ETIMEDOUT);
            } else {
                PANIC_ASSERT(thrd_p->timer_p == NULL);
                thrd_p->timer_p = &timer;
                timer_init(&timer,
                           timeout_p,
                           on_suspend_timer_expired,
                           thrd_p,
                           0);
                timer_start_isr(&timer);
            }
        }
    }

    thrd_reschedule();

    return (thrd_p->err);
}

int thrd_resume_isr(struct thrd_t *thrd_p, int err)
{
    int res;

    res = 0;
    thrd_p->err = err;

    if (thrd_p->state == THRD_STATE_SUSPENDED) {
        thrd_p->state = THRD_STATE_READY;

        if (thrd_p->timer_p != NULL) {
            err = timer_stop_isr(thrd_p->timer_p);
            PANIC_ASSERT(err == 1);
            thrd_p->timer_p = NULL;
        }

        scheduler_ready_push(thrd_p);
    } else if (thrd_p->state != THRD_STATE_TERMINATED) {
        thrd_p->state = THRD_STATE_RESUMED;
    } else {
        res = -1;
    }

    return (res);
}

int thrd_yield_isr(void)
{
    module.scheduler.current_p->state = THRD_STATE_READY;
    scheduler_ready_push(module.scheduler.current_p);
    thrd_reschedule();

    return (0);
}

void *thrd_stack_alloc(size_t size)
{
#if CONFIG_THRD_STACK_HEAP == 1
    return (heap_alloc(&stack_heap, size));
#else
    return (NULL);
#endif
}

int thrd_stack_free(void *stack_p)
{
#if CONFIG_THRD_STACK_HEAP == 1
    return (heap_free(&stack_heap, stack_p));
#else
    return (-1);
#endif
}

const void *thrd_get_bottom_of_stack(struct thrd_t *thrd_p)
{
    return (thrd_port_get_bottom_of_stack(thrd_p));
}

const void *thrd_get_top_of_stack(struct thrd_t *thrd_p)
{
    return (thrd_port_get_top_of_stack(thrd_p));
}

int thrd_prio_list_init(struct thrd_prio_list_t *self_p)
{
    self_p->head_p = NULL;

    return (0);
}

RAM_CODE void thrd_prio_list_push_isr(struct thrd_prio_list_t *self_p,
                                      struct thrd_prio_list_elem_t *elem_p)
{
    struct thrd_prio_list_elem_t *curr_p;
    struct thrd_prio_list_elem_t *prev_p;
    int prio;

    elem_p->next_p = NULL;

    if (self_p->head_p == NULL) {
        /* Empty list. */
        self_p->head_p = elem_p;
    } else {
        /* Add in prio order, with highest prio first. */
        curr_p = self_p->head_p;
        prev_p = NULL;
        prio = elem_p->thrd_p->prio;

        while (curr_p != NULL) {
            if (prio < curr_p->thrd_p->prio) {
                /* Insert before the 'curr_p' element. */
                if (prev_p != NULL) {
                    elem_p->next_p = prev_p->next_p;
                    prev_p->next_p = elem_p;
                } else {
                    elem_p->next_p = self_p->head_p;
                    self_p->head_p = elem_p;
                }

                return;
            }

            prev_p = curr_p;
            curr_p = curr_p->next_p;
        }

        /* End of the list. */
        prev_p->next_p = elem_p;
    }
}

RAM_CODE struct thrd_prio_list_elem_t *thrd_prio_list_pop_isr(
    struct thrd_prio_list_t *self_p)
{
    struct thrd_prio_list_elem_t *elem_p;

    elem_p = self_p->head_p;

    if (elem_p == NULL) {
        return (NULL);
    }

    self_p->head_p = elem_p->next_p;

    return (elem_p);
}

RAM_CODE int thrd_prio_list_remove_isr(struct thrd_prio_list_t *self_p,
                                       struct thrd_prio_list_elem_t *elem_p)
{
    struct thrd_prio_list_elem_t *curr_p;
    struct thrd_prio_list_elem_t *prev_p;

    curr_p = self_p->head_p;
    prev_p = NULL;

    while (curr_p != NULL) {
        if (curr_p == elem_p) {
            if (prev_p != NULL) {
                prev_p->next_p = elem_p->next_p;
            } else {
                self_p->head_p = elem_p->next_p;
            }

            return (0);
        }

        prev_p = curr_p;
        curr_p = curr_p->next_p;
    }

    return (-1);
}
