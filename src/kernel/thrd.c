/**
 * @file thrd.c
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

#include "simba.h"

/* Thread states. */
#define THRD_STATE_CURRENT      0
#define THRD_STATE_READY        1
#define THRD_STATE_SUSPENDED    2
#define THRD_STATE_KILLED       3

/* Stack usage and debugging. */
#define THRD_STACK_LOW_MAGIC 0x1337
#define THRD_FILL_PATTERN 0x19

#include "thrd_port.h"

static char *state_fmt[] = {
    "current",
    "ready",
    "suspended",
    "killed",
    "resumed",
};

FS_COMMAND_DEFINE("/kernel/thrd/list", thrd_cmd_list);
FS_COMMAND_DEFINE("/kernel/thrd/set_log_mask", thrd_cmd_set_log_mask);

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
    const char *name_p;
    struct thrd_parent_t parent;
    struct list_singly_linked_t children;
    uint32_t count;                        /* Number of times scheduled.*/
#if !defined(NPROFILESTACK)
    size_t stack_size;
#endif
#if !defined(NASSERT)
    uint16_t stack_low_magic;
#endif
};

struct thrd_scheduler_t {
    struct thrd_t *current_p;
    struct thrd_t *ready_p;
};

static struct thrd_scheduler_t scheduler = {
    .current_p = NULL,
    .ready_p = NULL
};

static void scheduler_ready_push(struct thrd_t *thrd_p);

#include "thrd_port.i"

static void scheduler_ready_push(struct thrd_t *thrd_p)
{
    struct thrd_t *ready_p;

    /* Add in prio order, with highest prio first. */
    ready_p = scheduler.ready_p;

    while (ready_p != NULL) {
        if (thrd_p->prio < ready_p->prio) {
            /* Insert before 'ready' thrd. */
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
        thrd_port_swap(in_p, out_p);
        in_p->count++;
    }
}

#if !defined(NPROFILESTACK)

extern char __main_stack_end;

static void thrd_fill_pattern(char *from_p, size_t size)
{
  size_t i;

  for (i = 0; i < size; i++) {
    from_p[i] = THRD_FILL_PATTERN;
  }
}

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

static void thrd_list_thrd(struct thrd_t *thrd_p, chan_t *chout_p)
{
    struct thrd_parent_t *child_p;
    struct list_sl_iterator_t iter;

    std_fprintf(chout_p,
                FSTR("%16s %16s %12s %5d %8lu"
#if !defined(NPROFILESTACK)
                     "    %6d/%6d"
#endif
                     "     0x%02x\r\n"),
                thrd_p->name_p,
                thrd_p->parent.thrd_p != NULL ?
                thrd_p->parent.thrd_p->name_p : "",
                state_fmt[thrd_p->state], thrd_p->prio,
                (unsigned long)thrd_p->count,
#if !defined(NPROFILESTACK)
                thrd_get_used_stack(thrd_p),
                (int)thrd_p->stack_size,
#endif
                thrd_p->log_mask);

    /* Children. */
    LIST_SL_ITERATOR_INIT(&iter, &thrd_p->children);

    while (1) {
        LIST_SL_ITERATOR_NEXT(&iter, &child_p);

        if (child_p == NULL) {
            break;
        }

        thrd_list_thrd(container_of(child_p, struct thrd_t, parent), chout_p);
    }
}

int thrd_cmd_list(int argc,
                  const char *argv[],
                  chan_t *chout,
                  chan_t *chin,
                  char *name)
{
    std_fprintf(chout,
                FSTR("            NAME           PARENT        STATE  PRIO     TIME"
#if !defined(NPROFILESTACK)
                     "  MAX-STACK-USAGE"
#endif
                     "  LOGMASK\r\n"));
    thrd_list_thrd(&main_thrd, chout);

    return (0);
}

static struct thrd_t *get_by_name(struct thrd_t *thrd_p,
                                  const char *name_p)
{
    struct thrd_parent_t *child_p;
    struct list_sl_iterator_t iter;

    /* Thread found? */
    if (strcmp(thrd_p->name_p, name_p) == 0) {
        return (thrd_p);
    }

    /* Iterate over children. */
    LIST_SL_ITERATOR_INIT(&iter, &thrd_p->children);

    thrd_p = NULL;

    while (thrd_p == NULL) {
        LIST_SL_ITERATOR_NEXT(&iter, &child_p);

        if (child_p == NULL) {
            break;
        }

        thrd_p = get_by_name(container_of(child_p, struct thrd_t, parent), name_p);
    }

    return (thrd_p);
}

static struct thrd_t *thrd_get_by_name(const char *name)
{
    return (get_by_name(&main_thrd, name));
}

int thrd_cmd_set_log_mask(int argc,
                          const char *argv[],
                          chan_t *chout_p,
                          chan_t *chin_p,
                          char *name_p)
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

    if (std_strtol(argv[2], &mask) != 0) {
        return (-EINVAL);
    }

    thrd_p->log_mask = mask;

    return (0);
}

static char idle_thrd_stack[THRD_IDLE_STACK_MAX];
static void *idle_thrd(void *arg_p)
{
    struct thrd_t *thrd_p;

    thrd_set_name("idle");

    thrd_p = thrd_self();

    /* Wait for interrupts. */
    while (1) {
        thrd_port_idle_wait();
        sys_lock();
        thrd_p->state = THRD_STATE_READY;
        scheduler_ready_push(thrd_p);
        thrd_reschedule();
        sys_unlock();
    }

    return (NULL);
}

int thrd_module_init(void)
{
#if !defined(NPROFILESTACK)
    char dummy;
#endif

    /* Main function becomes a thrd. */
    main_thrd.prev_p = NULL;
    main_thrd.next_p = NULL;
    main_thrd.prio = 0;
    main_thrd.state = THRD_STATE_CURRENT;
    main_thrd.err = 0;
    main_thrd.log_mask = LOG_UPTO(NOTICE);
    main_thrd.name_p = "main";
    main_thrd.parent.thrd_p = NULL;
    LIST_SL_INIT(&main_thrd.children);
    main_thrd.count = 0;
#if !defined(NASSERT)
    main_thrd.stack_low_magic = THRD_STACK_LOW_MAGIC;
#endif
#if !defined(NPROFILESTACK)
    main_thrd.stack_size = (&__main_stack_end - (char *)(&main_thrd + 1));
    thrd_fill_pattern((char *)(&main_thrd + 1),
                      &dummy - (char *)(&main_thrd + 2));
#endif
    thrd_port_init_main(&main_thrd.port);
    scheduler.current_p = &main_thrd;

    thrd_spawn(idle_thrd, NULL, 127, idle_thrd_stack, sizeof(idle_thrd_stack));

    return (0);
}

struct thrd_t *thrd_spawn(void *(*entry)(void *),
                          void *arg_p,
                          int prio,
                          void *stack_p,
                          size_t stack_size)
{
    struct thrd_t *thrd_p;
    int err = 0;

    /* Initialize thrd structure in the beginning of the stack. */
    thrd_p = stack_p;
    thrd_p->prev_p = NULL;
    thrd_p->next_p = NULL;
    thrd_p->prio = prio;
    thrd_p->state = THRD_STATE_READY;
    thrd_p->err = 0;
    thrd_p->log_mask = LOG_UPTO(NOTICE);
    thrd_p->name_p = "";
    thrd_p->parent.thrd_p = thrd_self();
    LIST_SL_INIT(&thrd_p->children);
    thrd_p->count = 0;
#if !defined(NASSERT)
    thrd_p->stack_low_magic = THRD_STACK_LOW_MAGIC;
#endif
#if !defined(NPROFILESTACK)
    thrd_p->stack_size = (stack_size - sizeof(*thrd_p));
    thrd_fill_pattern((char *)(thrd_p + 1), thrd_p->stack_size);
#endif
    LIST_SL_ADD_TAIL(&thrd_p->parent.thrd_p->children, &thrd_p->parent);

    err = thrd_port_spawn(thrd_p, entry, arg_p, stack_p, stack_size);
    sys_lock();
    scheduler_ready_push(thrd_p);
    sys_unlock();

    return (err == 0 ? stack_p : NULL);
}

int thrd_kill(struct thrd_t *thrd_p)
{
    thrd_port_kill(thrd_p);
    thrd_p->state = THRD_STATE_KILLED;

    return (0);
}

int thrd_suspend(struct time_t *timeout_p)
{
    int err;

    sys_lock();
    err = thrd_suspend_irq(timeout_p);
    sys_unlock();

    return (err);
}

int thrd_resume(struct thrd_t *thrd_p, int err)
{
    sys_lock();

    thrd_p->err = err;
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);
    thrd_reschedule();

    sys_unlock();

    return (0);
}

int thrd_resume_irq(struct thrd_t *thrd_p, int err)
{
    thrd_p->err = err;
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);

    return (0);
}

int thrd_usleep(long useconds)
{
    struct time_t timeout;
    int err;

    timeout.seconds = (useconds / 1000000);
    timeout.nanoseconds = 1000 * (useconds % 1000000);
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

int thrd_set_log_mask(struct thrd_t *thrd_p, int mask)
{
    int old;

    old = thrd_p->log_mask;
    thrd_p->log_mask = mask;

    return (old);
}

int thrd_get_log_mask()
{
    return (scheduler.current_p->log_mask);
}

void thrd_tick(void)
{
    thrd_port_tick();
}

int thrd_suspend_irq(struct time_t *timeout_p)
{
    struct thrd_t *thrd_p;
    struct timer_t timer;

    thrd_p = thrd_self();

    thrd_p->state = THRD_STATE_SUSPENDED;

    if (timeout_p != NULL) {
        if ((timeout_p->seconds == 0) && (timeout_p->nanoseconds == 0)) {
            return (-ETIMEDOUT);
        } else {
            timer_set_irq(&timer, timeout_p, thrd_port_suspend_timer_callback, thrd_p, 0);
        }
    }

    thrd_reschedule();

    return (thrd_p->err);
}
