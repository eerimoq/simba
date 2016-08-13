/**
 * @file thrd.c
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

struct thrd_scheduler_t {
    struct thrd_t *current_p;
    struct thrd_t *ready_p;
};

struct get_by_name_t {
    const char *name_p;
    struct thrd_t *thrd_p;
};

static volatile struct thrd_scheduler_t scheduler = {
    .current_p = NULL,
    .ready_p = NULL
};

#if CONFIG_THRD_ENV == 1

static struct thrd_environment_variable_t main_variables[4];
static struct sem_t env_sem;

static int init_env(struct thrd_environment_variable_t *variables_p,
                    int length)
{
    scheduler.current_p->env.variables_p = variables_p;
    scheduler.current_p->env.number_of_variables = 0;
    scheduler.current_p->env.max_number_of_variables = length;

    return (0);
}

static int set_env(const char *name_p, const char *value_p)
{
    int i;
    struct thrd_environment_t *env_p;
    struct thrd_environment_variable_t *variable_p;

    env_p = &scheduler.current_p->env;
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

static const char *get_env(const char *name_p)
{
    int i;
    struct thrd_environment_t *env_p;
    struct thrd_environment_variable_t *variable_p;
    struct thrd_t *thrd_p;

    thrd_p = scheduler.current_p;

    while (thrd_p != NULL) {
        env_p = &thrd_p->env;
        variable_p = &env_p->variables_p[0];

        /* Search for the variable. */
        for (i = 0; i < env_p->number_of_variables; i++) {
            if (strcmp(variable_p->name_p, name_p) == 0) {
                return (variable_p->value_p);
            }
            
            variable_p++;
        }

        thrd_p = thrd_p->parent.thrd_p;
    }

    return (NULL);
}

#endif

/* Forward declarations for thrd_port. */
static void scheduler_ready_push(struct thrd_t *thrd_p);

static void thrd_reschedule(void);

static void terminate(void);

#if (CONFIG_MONITOR_THREAD == 1) || (CONFIG_FS_CMD_THRD_LIST == 1) || (CONFIG_FS_CMD_THRD_SET_LOG_MASK == 1)

static int iterate_thread_children(struct thrd_t *thrd_p,
                                   int (*callback)(void *arg_p,
                                                   struct thrd_t *thrd_p),
                                   void *arg_p);

static int iterate_threads(int (*callback)(void *arg_p,
                                           struct thrd_t *thrd_p),
                           void *arg_p);

#endif

#include "thrd_port.i"

#if CONFIG_MONITOR_THREAD == 1
#    include "thrd/thrd_monitor.i"
#endif

/* Stacks. */
static THRD_STACK(idle_thrd_stack, THRD_IDLE_STACK_MAX);

static void terminate(void)
{
    /* The thread is terminated. */
    sys_lock();
    thrd_self()->state = THRD_STATE_TERMINATED;
    thrd_reschedule();
    sys_unlock();
}

/**
 * Push a thread on the list of threads that are ready to be
 * scheduled. The ready list is a linked list with the highest
 * priority thread in the first element. The pushed thread is added
 * _after_ any already pushed threads with the same priority.
 *
 * @param[in] thrd_p Thread to push to the the ready list.
 */
static void scheduler_ready_push(struct thrd_t *thrd_p)
{
    struct thrd_t *ready_p;

    /* Add in prio order, with highest prio first. */
    ready_p = scheduler.ready_p;

    while (ready_p != NULL) {
        if (thrd_p->prio < ready_p->prio) {
            /* Insert before the 'ready_p' thrd. */
            if (ready_p->prev_p != NULL) {
                ready_p->prev_p->next_p = thrd_p;
            } else {
                scheduler.ready_p = thrd_p;
            }

            thrd_p->prev_p = ready_p->prev_p;
            thrd_p->next_p = ready_p;
            ready_p->prev_p = thrd_p;
            return;
        }

        /* End of ready list. */
        if (ready_p->next_p == NULL) {
            ready_p->next_p = thrd_p;
            thrd_p->prev_p = ready_p;
            thrd_p->next_p = NULL;
            return;
        }

        ready_p = ready_p->next_p;
    }

    /* Empty list. */
    scheduler.ready_p = thrd_p;
    thrd_p->prev_p = NULL;
    thrd_p->next_p = NULL;
}

/**
 * Pop the most important thread from the ready list.
 */
static struct thrd_t *scheduler_ready_pop(void)
{
    struct thrd_t *thrd_p;

    thrd_p = scheduler.ready_p;
    scheduler.ready_p = thrd_p->next_p;

    if (scheduler.ready_p != NULL) {
        scheduler.ready_p->prev_p = NULL;
    }

    thrd_p->prev_p = NULL;
    thrd_p->next_p = NULL;

    return (thrd_p);
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

    ASSERTN(out_p->stack_low_magic == THRD_STACK_LOW_MAGIC, ESTACK);

    in_p = scheduler_ready_pop();

    /* Swap threads. */
    in_p->state = THRD_STATE_CURRENT;

    if (in_p != out_p) {
        scheduler.current_p = in_p;
        thrd_port_cpu_usage_stop(out_p);
        thrd_port_swap(in_p, out_p);
        thrd_port_cpu_usage_start(out_p);
    }
}

#if CONFIG_PROFILE_STACK == 1

extern char __main_stack_end;

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

#if (CONFIG_MONITOR_THREAD == 1) || (CONFIG_FS_CMD_THRD_LIST == 1) || (CONFIG_FS_CMD_THRD_SET_LOG_MASK == 1)

static int iterate_thread_children(struct thrd_t *thrd_p,
                                   int (*callback)(void *arg_p,
                                                   struct thrd_t *thrd_p),
                                   void *arg_p)
{
    int res = 0;
    struct thrd_parent_t *child_p;
    struct list_sl_iterator_t iter;
    
    /* Call the callback. */
    res = callback(arg_p, thrd_p);

    if (res != 0) {
        return (res);
    }

    /* Iterate over all children. */
    LIST_SL_ITERATOR_INIT(&iter, &thrd_p->children);

    while (res == 0) {
        LIST_SL_ITERATOR_NEXT(&iter, &child_p);

        if (child_p == NULL) {
            break;
        }

        res = iterate_thread_children(container_of(child_p, struct thrd_t, parent),
                                      callback,
                                      arg_p);
    }

    return (res);
}

/**
 * Recursively call given callback for all threads, starting with the
 * main thread.
 */
static int iterate_threads(int (*callback)(void *arg_p,
                                           struct thrd_t *thrd_p),
                           void *arg_p)
{
    return (iterate_thread_children(&main_thrd, callback, arg_p));
}

#endif

#if CONFIG_FS_CMD_THRD_LIST == 1

static char *state_fmt[] = {
    "current",
    "ready",
    "suspended",
    "resumed",
    "terminated"
};

static struct fs_command_t cmd_list;

static int cmd_list_thrd_print(chan_t *chout_p,
                               struct thrd_t *thrd_p)
{
    std_fprintf(chout_p,
                FSTR("%16s %16s %12s %5d %4u%%"
#if CONFIG_PROFILE_STACK == 1
                     "    %6d/%6d"
#endif
                     "     0x%02x\r\n"),
                thrd_p->name_p,
                thrd_p->parent.thrd_p != NULL ?
                thrd_p->parent.thrd_p->name_p : "",
                state_fmt[thrd_p->state], thrd_p->prio,
                (unsigned int)thrd_p->cpu.usage,
#if CONFIG_PROFILE_STACK == 1
                thrd_get_used_stack(thrd_p),
                (int)thrd_p->stack_size,
#endif
                thrd_p->log_mask);

    return (0);
}

static int cmd_list_cb(int argc,
                       const char *argv[],
                       chan_t *chout_p,
                       chan_t *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    std_fprintf(chout_p,
                FSTR("            NAME           PARENT        STATE  PRIO   CPU"
#if CONFIG_PROFILE_STACK == 1
                     "  MAX-STACK-USAGE"
#endif
                     "  LOGMASK\r\n"));

    return (iterate_threads(cmd_list_thrd_print, chout_p));
}

#endif

#if CONFIG_FS_CMD_THRD_SET_LOG_MASK == 1

static struct fs_command_t cmd_set_log_mask;

static int get_by_name(void *arg_p,
                       struct thrd_t *thrd_p)
{
    struct get_by_name_t *data_p = arg_p;

    /* Thread found? */
    if (strcmp(thrd_p->name_p, data_p->name_p) == 0) {
        data_p->thrd_p = thrd_p;

        return (1);
    }

    return (0);
}

static struct thrd_t *thrd_get_by_name(const char *name_p)
{
    struct get_by_name_t data;

    data.name_p = name_p;
    data.thrd_p = NULL;

    iterate_threads(get_by_name, &data);

    return (data.thrd_p);
}

static int cmd_set_log_mask_cb(int argc,
                               const char *argv[],
                               chan_t *chout_p,
                               chan_t *chin_p,
                               void *arg_p,
                               void *call_arg_p)
{
    struct thrd_t *thrd_p;
    long mask;

    if (argc != 3) {
        std_fprintf(chout_p, FSTR("Usage: set_log_mask <thread name> <log mask>\r\n"));
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
#if CONFIG_PROFILE_STACK == 1
    char dummy = 0;
#endif

    /* Main function becomes a thrd. */
    main_thrd.prev_p = NULL;
    main_thrd.next_p = NULL;
    main_thrd.prio = 0;
    main_thrd.state = THRD_STATE_CURRENT;
    main_thrd.err = 0;
    main_thrd.log_mask = LOG_UPTO(INFO);
    main_thrd.timer_p = NULL;
    main_thrd.name_p = "main";
    main_thrd.parent.thrd_p = NULL;
    LIST_SL_INIT(&main_thrd.children);
    main_thrd.cpu.usage = 0;

#if CONFIG_THRD_ENV == 1
    main_thrd.env.variables_p = main_variables;
    main_thrd.env.number_of_variables = 0;
    main_thrd.env.max_number_of_variables = membersof(main_variables);
    sem_init(&env_sem, 0, 1);
#endif

#if CONFIG_ASSERT == 1
    main_thrd.stack_low_magic = THRD_STACK_LOW_MAGIC;
#endif

#if CONFIG_PROFILE_STACK == 1
    main_thrd.stack_size = (&__main_stack_end - (char *)(&main_thrd + 1));
    thrd_fill_pattern((char *)(&main_thrd + 1),
                      &dummy - (char *)(&main_thrd + 2));
#endif

    thrd_port_init_main(&main_thrd.port);
    scheduler.current_p = &main_thrd;

    thrd_spawn(idle_thrd, NULL, 127, idle_thrd_stack, sizeof(idle_thrd_stack));

#if CONFIG_MONITOR_THREAD == 1

    thrd_spawn(monitor_thrd,
               NULL,
               THRD_MONITOR_PRIO,
               monitor_thrd_stack,
               sizeof(monitor_thrd_stack));

#endif

#if CONFIG_FS_CMD_THRD_LIST == 1

    fs_command_init(&cmd_list,
                    FSTR("/kernel/thrd/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&cmd_list);

#endif

#if CONFIG_FS_CMD_THRD_SET_LOG_MASK == 1

    fs_command_init(&cmd_set_log_mask,
                    FSTR("/kernel/thrd/set_log_mask"),
                    cmd_set_log_mask_cb,
                    NULL);
    fs_command_register(&cmd_set_log_mask);

#endif

#if CONFIG_MONITOR_THREAD == 1

    fs_command_init(&cmd_monitor_set_period_ms,
                    FSTR("/kernel/thrd/monitor/set_period_ms"),
                    cmd_monitor_set_period_ms_cb,
                    NULL);
    fs_command_register(&cmd_monitor_set_period_ms);

    fs_command_init(&cmd_monitor_set_print,
                    FSTR("/kernel/thrd/monitor/set_print"),
                    cmd_monitor_set_print_cb,
                    NULL);
    fs_command_register(&cmd_monitor_set_print);

#endif

    return (0);
}

struct thrd_t *thrd_spawn(void *(*main)(void *),
                          void *arg_p,
                          int prio,
                          void *stack_p,
                          size_t stack_size)
{
    ASSERTN(main != NULL, EINVAL);
    ASSERTN(stack_p != NULL, EINVAL);
    ASSERTN(stack_size > sizeof(struct thrd_t) + 1, EINVAL);

    struct thrd_t *thrd_p;
    int err = 0;

    /* Initialize thrd structure in the beginning of the stack. */
    thrd_p = stack_p;
    thrd_p->prev_p = NULL;
    thrd_p->next_p = NULL;
    thrd_p->prio = prio;
    thrd_p->state = THRD_STATE_READY;
    thrd_p->err = 0;
    thrd_p->log_mask = LOG_UPTO(INFO);
    thrd_p->timer_p = NULL;
    thrd_p->name_p = "";
    thrd_p->parent.thrd_p = thrd_self();
    LIST_SL_INIT(&thrd_p->children);
    thrd_p->cpu.usage = 0.0f;

#if CONFIG_THRD_ENV == 1
    thrd_p->env.variables_p = NULL;
    thrd_p->env.number_of_variables = 0;
    thrd_p->env.max_number_of_variables = 0;
#endif

#if CONFIG_ASSERT == 1
    thrd_p->stack_low_magic = THRD_STACK_LOW_MAGIC;
#endif

#if CONFIG_PROFILE_STACK == 1
    thrd_p->stack_size = (stack_size - sizeof(*thrd_p));
    thrd_fill_pattern((char *)(thrd_p + 1), thrd_p->stack_size);
#endif

    LIST_SL_ADD_TAIL(&thrd_p->parent.thrd_p->children, &thrd_p->parent);

    err = thrd_port_spawn(thrd_p, main, arg_p, stack_p, stack_size);

    sys_lock();
    scheduler_ready_push(thrd_p);
    sys_unlock();

    return (err == 0 ? stack_p : NULL);
}

int thrd_suspend(struct time_t *timeout_p)
{
    int err;

    sys_lock();
    err = thrd_suspend_isr(timeout_p);
    sys_unlock();

    return (err);
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

    while (1) {
        sys_lock();

        if (thrd_p->state == THRD_STATE_TERMINATED) {
            sys_unlock();
            break;
        }

        sys_unlock();

        thrd_sleep_us(50000);
    }

    return (0);
}

int thrd_sleep_us(long microseconds)
{
    struct time_t timeout;
    int err;

    timeout.seconds = (microseconds / 1000000);
    timeout.nanoseconds = 1000 * (microseconds % 1000000);
    err = thrd_suspend(&timeout);

    return (err == -ETIMEDOUT ? 0 : -1);
}

struct thrd_t *thrd_self(void)
{
    return (scheduler.current_p);
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
    return (scheduler.current_p->log_mask);
}

int thrd_init_env(struct thrd_environment_variable_t *variables_p,
                  int length)
{
#if CONFIG_THRD_ENV == 1

    int res;

    sem_take(&env_sem, NULL);
    res = init_env(variables_p, length);
    sem_give(&env_sem, 1);

    return (res);

#else

    return (-1);

#endif
}

int thrd_set_env(const char *name_p, const char *value_p)
{
    ASSERTN(name_p != NULL, -EINVAL);

#if CONFIG_THRD_ENV == 1

    int res;

    sem_take(&env_sem, NULL);
    res = set_env(name_p, value_p);
    sem_give(&env_sem, 1);

    return (res);

#else

    return (-1);

#endif
}

const char *thrd_get_env(const char *name_p)
{
    ASSERTN(name_p != NULL, -EINVAL);

#if CONFIG_THRD_ENV == 1

    const char *value_p;

    sem_take(&env_sem, NULL);
    value_p = get_env(name_p);
    sem_give(&env_sem, 1);
    
    return (value_p);

#else
    
    return (NULL);

#endif
}

void thrd_tick_isr(void)
{
    thrd_port_tick();

#if CONFIG_PREEMPTIVE_SCHEDULER == 1
    THRD_RESCHEDULE_ISR;
#endif
}

int thrd_suspend_isr(struct time_t *timeout_p)
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
            if ((timeout_p->seconds == 0) && (timeout_p->nanoseconds == 0)) {
                return (-ETIMEDOUT);
            } else {
                thrd_p->timer_p = &timer;
                timer_init(&timer,
                           timeout_p,
                           thrd_port_suspend_timer_callback,
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
    int res = 1;

    thrd_p->err = err;

    if (thrd_p->state == THRD_STATE_SUSPENDED) {
        thrd_p->state = THRD_STATE_READY;

        if (thrd_p->timer_p != NULL) {
            timer_stop_isr(thrd_p->timer_p);
            thrd_p->timer_p = NULL;
        }

        scheduler_ready_push(thrd_p);
    } else if (thrd_p->state != THRD_STATE_TERMINATED) {
        thrd_p->state = THRD_STATE_RESUMED;
    } else {
        res = 0;
    }

    return (res);
}

int thrd_yield_isr(void)
{
    scheduler.current_p->state = THRD_STATE_READY;
    scheduler_ready_push(scheduler.current_p);
    thrd_reschedule();

    return (0);
}
