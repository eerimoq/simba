/**
 * @file log.c
 * @version 5.0.0
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
#include <stdarg.h>

struct state_t {
    struct log_handler_t handler;
    struct log_object_t object;
    struct sem_t sem;
};

static FAR const char level_fatal[] = "fatal";
static FAR const char level_error[] = "error";
static FAR const char level_warning[] = "warning";
static FAR const char level_info[] = "info";
static FAR const char level_debug[] = "debug";

/* Level strings array. */
static const char FAR *level_as_string[] = {
    level_fatal,
    level_error,
    level_warning,
    level_info,
    level_debug
};

/* The module state. */
static struct state_t state;

#if CONFIG_FS_CMD_LOG_PRINT == 1

static struct fs_command_t cmd_print;

/**
 * The shell command callback for "/kernel/log/print".
 */
static int cmd_print_cb(int argc,
                        const char *argv[],
                        chan_t *out_p,
                        chan_t *in_p,
                        void *arg_p,
                        void *call_arg_p)
{
    if (argc != 2) {
        std_fprintf(out_p, FSTR("Usage: print <string>\r\n"));

        return (-EINVAL);
    }

    /* Write the argument to the log. */
    log_object_print(&state.object, LOG_INFO, FSTR("%s\r\n"), argv[1]);

    return (0);
}

#endif

#if CONFIG_FS_CMD_LOG_LIST == 1

static struct fs_command_t cmd_list;

/**
 * The shell command callback for "/kernel/log/print".
 */
static int cmd_list_cb(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct log_object_t *object_p;

    if (argc != 1) {
        std_fprintf(out_p, FSTR("Usage: list\r\n"));

        return (-EINVAL);
    }

    sem_take(&state.sem, NULL);

    std_fprintf(out_p, FSTR("OBJECT-NAME       MASK\r\n"));

    object_p = &state.object;

    while (object_p != NULL) {
        std_fprintf(out_p,
                    FSTR("%-16s  0x%02x\r\n"),
                    object_p->name_p,
                    (int)object_p->mask);

        object_p = object_p->next_p;
    }

    sem_give(&state.sem, 1);

    return (0);
}

#endif

#if CONFIG_FS_CMD_LOG_SET_LOG_MASK == 1

static struct fs_command_t cmd_set_log_mask;

/**
 * The shell command callback for "/kernel/log/set_log_mask".
 */
static int cmd_set_log_mask_cb(int argc,
                               const char *argv[],
                               chan_t *out_p,
                               chan_t *in_p,
                               void *arg_p,
                               void *call_arg_p)
{
    struct log_object_t *object_p;
    long mask;
    int found;
    const char *name_p;

    if (argc != 3) {
        std_fprintf(out_p, FSTR("Usage: set_log_mask <obejct> <mask>\r\n"));

        return (-EINVAL);
    }

    if (std_strtol(argv[2], &mask) == NULL) {
        std_fprintf(out_p, FSTR("bad mask %s\r\n"), argv[2]);

        return (-EINVAL);
    }

    name_p = argv[1];
    found = 0;

    sem_take(&state.sem, NULL);

    object_p = &state.object;

    while (object_p != NULL) {
        if (strcmp(object_p->name_p, name_p) == 0) {
            log_object_set_log_mask(object_p, mask);
            found = 1;

            break;
        }

        object_p = object_p->next_p;
    }

    sem_give(&state.sem, 1);

    if (found == 0) {
        std_fprintf(out_p,
                    FSTR("warning: no log object with name %s\r\n"),
                    name_p);

        return (-EINVAL);
    }

    return (0);
}

#endif

int log_module_init()
{
    sem_init(&state.sem, 0, 1);

    state.handler.chout_p = sys_get_stdout();
    state.handler.next_p = NULL;

    state.object.name_p = "log";
    state.object.mask = LOG_UPTO(INFO);
    state.object.next_p = NULL;

#if CONFIG_FS_CMD_LOG_PRINT == 1

    /* Setup shell commands. */
    fs_command_init(&cmd_print,
                    FSTR("/kernel/log/print"),
                    cmd_print_cb,
                    NULL);
    fs_command_register(&cmd_print);

#endif

#if CONFIG_FS_CMD_LOG_LIST == 1

    fs_command_init(&cmd_list,
                    FSTR("/kernel/log/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&cmd_list);

#endif

#if CONFIG_FS_CMD_LOG_SET_LOG_MASK == 1

    fs_command_init(&cmd_set_log_mask,
                    FSTR("/kernel/log/set_log_mask"),
                    cmd_set_log_mask_cb,
                    NULL);
    fs_command_register(&cmd_set_log_mask);

#endif

    return (0);
}

int log_add_handler(struct log_handler_t *handler_p)
{
    ASSERTN(handler_p != NULL, EINVAL);

    sem_take(&state.sem, NULL);

    handler_p->next_p = state.handler.next_p;
    state.handler.next_p = handler_p;

    sem_give(&state.sem, 1);

    return (0);
}

int log_remove_handler(struct log_handler_t *handler_p)
{
    ASSERTN(handler_p != NULL, EINVAL);

    struct log_handler_t *curr_p, *prev_p;

    sem_take(&state.sem, NULL);

    curr_p = state.handler.next_p;
    prev_p = &state.handler;

    while (curr_p != NULL) {
        if (curr_p == handler_p) {
            if (prev_p != NULL) {
                prev_p->next_p = curr_p->next_p;
            }

            curr_p->next_p = NULL;
            sem_give(&state.sem, 1);

            return (0);
        }
    }

    sem_give(&state.sem, 1);

    return (1);
}

int log_add_object(struct log_object_t *object_p)
{
    ASSERTN(object_p != NULL, EINVAL);

    sem_take(&state.sem, NULL);

    object_p->next_p = state.object.next_p;
    state.object.next_p = object_p;

    sem_give(&state.sem, 1);

    return (0);
}

int log_remove_object(struct log_object_t *object_p)
{
    ASSERTN(object_p != NULL, EINVAL);

    struct log_object_t *curr_p, *prev_p;

    sem_take(&state.sem, NULL);

    curr_p = state.object.next_p;
    prev_p = &state.object;

    while (curr_p != NULL) {
        if (curr_p == object_p) {
            if (prev_p != NULL) {
                prev_p->next_p = curr_p->next_p;
            }

            curr_p->next_p = NULL;
            sem_give(&state.sem, 1);

            return (0);
        }
    }

    sem_give(&state.sem, 1);

    return (1);
}

int log_set_default_handler_output_channel(chan_t *chout_p)
{
    state.handler.chout_p = chout_p;

    return (0);
}

int log_handler_init(struct log_handler_t *self_p,
                     chan_t *chout_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(chout_p != NULL, EINVAL);

    self_p->chout_p = chout_p;
    self_p->next_p = NULL;

    return (0);
}

int log_object_init(struct log_object_t *self_p,
                    const char *name_p,
                    char mask)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(name_p != NULL, EINVAL);

    self_p->name_p = name_p;
    self_p->mask = mask;

    return (0);
}

int log_object_set_log_mask(struct log_object_t *self_p,
                            char mask)
{
    self_p->mask = (mask & LOG_ALL);

    return (0);
}

char log_object_get_log_mask(struct log_object_t *self_p)
{
    return (self_p->mask);
}

int log_object_print(struct log_object_t *self_p,
                     int level,
                     const char *fmt_p,
                     ...)
{
    ASSERTN(fmt_p != NULL, EINVAL);

    va_list ap;
    struct time_t now;
    struct log_handler_t *handler_p;
    chan_t *chout_p;
    int count;
    const char *name_p;

    /* Level filtering. */
    if (self_p == NULL) {
        /* Use the thread log mask if no log object is given. */
        if ((thrd_get_log_mask() & (1 << level)) == 0) {
            return (0);
        }

        name_p = "default";
    } else {
        if ((self_p->mask & (1 << level)) == 0) {
            return (0);
        }

        name_p = self_p->name_p;
    }

    /* Print the formatted log entry to all handlers. */
    count = 0;
    handler_p = &state.handler;

    sem_take(&state.sem, NULL);

    time_get(&now);

    while (handler_p != NULL) {
        chout_p = handler_p->chout_p;

        if (chout_p != NULL) {
            /* Write the header. */
            std_fprintf(chout_p, FSTR("%lu:"), now.seconds);
            std_fprintf(chout_p, level_as_string[level]);
            std_fprintf(chout_p,
                        FSTR(":%s:%s: "),
                        thrd_get_name(),
                        name_p);

            /* Write the custom message. */
            va_start(ap, fmt_p);
            std_vfprintf(chout_p, fmt_p, &ap);
            va_end(ap);

            count++;
        }

        handler_p = handler_p->next_p;
    }

    sem_give(&state.sem, 1);

    return (count);
}
