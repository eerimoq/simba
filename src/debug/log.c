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
#include <stdarg.h>

struct module_t {
    int initialized;
    struct log_handler_t handler;
    struct log_object_t object;
    struct sem_t sem;
#if CONFIG_FS_CMD_LOG_PRINT == 1
    struct fs_command_t cmd_print;
#endif
#if CONFIG_FS_CMD_LOG_LIST == 1
    struct fs_command_t cmd_list;
#endif
#if CONFIG_FS_CMD_LOG_SET_LOG_MASK == 1
    struct fs_command_t cmd_set_log_mask;
#endif
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
static struct module_t module;

#if CONFIG_FS_CMD_LOG_PRINT == 1

/**
 * The shell command callback for "/debug/log/print".
 */
static int cmd_print_cb(int argc,
                        const char *argv[],
                        void *out_p,
                        void *in_p,
                        void *arg_p,
                        void *call_arg_p)
{
    if (argc != 2) {
        std_fprintf(out_p, OSTR("Usage: print <string>\r\n"));

        return (-EINVAL);
    }

    /* Write the argument to the log. */
    log_object_print(&module.object, LOG_INFO, OSTR("%s\r\n"), argv[1]);

    return (0);
}

#endif

#if CONFIG_FS_CMD_LOG_LIST == 1

/**
 * The shell command callback for "/debug/log/print".
 */
static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct log_object_t *object_p;

    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: list\r\n"));

        return (-EINVAL);
    }

    sem_take(&module.sem, NULL);

    std_fprintf(out_p, OSTR("OBJECT-NAME       MASK\r\n"));

    object_p = &module.object;

    while (object_p != NULL) {
        std_fprintf(out_p,
                    OSTR("%-16s  0x%02x\r\n"),
                    object_p->name_p,
                    (int)object_p->mask);

        object_p = object_p->next_p;
    }

    sem_give(&module.sem, 1);

    return (0);
}

#endif

#if CONFIG_FS_CMD_LOG_SET_LOG_MASK == 1

/**
 * The shell command callback for "/debug/log/set_log_mask".
 */
static int cmd_set_log_mask_cb(int argc,
                               const char *argv[],
                               void *out_p,
                               void *in_p,
                               void *arg_p,
                               void *call_arg_p)
{
    struct log_object_t *object_p;
    long mask;
    int found;
    const char *name_p;

    if (argc != 3) {
        std_fprintf(out_p, OSTR("Usage: set_log_mask <obejct> <mask>\r\n"));

        return (-EINVAL);
    }

    if (std_strtol(argv[2], &mask) == NULL) {
        std_fprintf(out_p, OSTR("bad mask %s\r\n"), argv[2]);

        return (-EINVAL);
    }

    name_p = argv[1];
    found = 0;

    sem_take(&module.sem, NULL);

    object_p = &module.object;

    while (object_p != NULL) {
        if (strcmp(object_p->name_p, name_p) == 0) {
            (void)log_object_set_log_mask(object_p, mask);
            found = 1;

            break;
        }

        object_p = object_p->next_p;
    }

    sem_give(&module.sem, 1);

    if (found == 0) {
        std_fprintf(out_p,
                    OSTR("warning: no log object with name %s\r\n"),
                    name_p);

        return (-EINVAL);
    }

    return (0);
}

#endif

int log_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    sem_init(&module.sem, 0, 1);

    module.handler.chout_p = sys_get_stdout();
    module.handler.next_p = NULL;

    module.object.name_p = "log";
    module.object.mask = LOG_UPTO(INFO);
    module.object.next_p = NULL;

#if CONFIG_FS_CMD_LOG_PRINT == 1

    /* Setup shell commands. */
    fs_command_init(&module.cmd_print,
                    CSTR("/debug/log/print"),
                    cmd_print_cb,
                    NULL);
    fs_command_register(&module.cmd_print);

#endif

#if CONFIG_FS_CMD_LOG_LIST == 1

    fs_command_init(&module.cmd_list,
                    CSTR("/debug/log/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

#if CONFIG_FS_CMD_LOG_SET_LOG_MASK == 1

    fs_command_init(&module.cmd_set_log_mask,
                    CSTR("/debug/log/set_log_mask"),
                    cmd_set_log_mask_cb,
                    NULL);
    fs_command_register(&module.cmd_set_log_mask);

#endif

    return (0);
}

int log_add_handler(struct log_handler_t *handler_p)
{
    ASSERTN(handler_p != NULL, EINVAL);

    sem_take(&module.sem, NULL);

    handler_p->next_p = module.handler.next_p;
    module.handler.next_p = handler_p;

    sem_give(&module.sem, 1);

    return (0);
}

int log_remove_handler(struct log_handler_t *handler_p)
{
    ASSERTN(handler_p != NULL, EINVAL);

    struct log_handler_t *curr_p, *prev_p;

    sem_take(&module.sem, NULL);

    curr_p = module.handler.next_p;
    prev_p = &module.handler;

    while (curr_p != NULL) {
        if (curr_p == handler_p) {
            if (prev_p != NULL) {
                prev_p->next_p = curr_p->next_p;
            }

            curr_p->next_p = NULL;
            sem_give(&module.sem, 1);

            return (0);
        }
    }

    sem_give(&module.sem, 1);

    return (1);
}

int log_add_object(struct log_object_t *object_p)
{
    ASSERTN(object_p != NULL, EINVAL);

    sem_take(&module.sem, NULL);

    object_p->next_p = module.object.next_p;
    module.object.next_p = object_p;

    sem_give(&module.sem, 1);

    return (0);
}

int log_remove_object(struct log_object_t *object_p)
{
    ASSERTN(object_p != NULL, EINVAL);

    struct log_object_t *curr_p, *prev_p;

    sem_take(&module.sem, NULL);

    curr_p = module.object.next_p;
    prev_p = &module.object;

    while (curr_p != NULL) {
        if (curr_p == object_p) {
            if (prev_p != NULL) {
                prev_p->next_p = curr_p->next_p;
            }

            curr_p->next_p = NULL;
            sem_give(&module.sem, 1);

            return (0);
        }
    }

    sem_give(&module.sem, 1);

    return (1);
}

int log_set_default_handler_output_channel(void *chout_p)
{
    module.handler.chout_p = chout_p;

    return (0);
}

int log_handler_init(struct log_handler_t *self_p,
                     void *chout_p)
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

int log_object_is_enabled_for(struct log_object_t *self_p,
                              int level)
{
    if (self_p == NULL) {
        /* Use the thread log mask if no log object is given. */
        return ((thrd_get_log_mask() & (1 << level)) != 0);
    } else {
        return ((self_p->mask & (1 << level)) != 0);
    }
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
    void *chout_p;
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
    handler_p = &module.handler;

    sem_take(&module.sem, NULL);

    time_get(&now);

    while (handler_p != NULL) {
        chout_p = handler_p->chout_p;

        if (chout_p != NULL) {
            chan_control(chout_p, CHAN_CONTROL_LOG_BEGIN);

            /* Write the header. */
            std_fprintf(chout_p,
                        FSTR("%lu.%03lu:"),
                        now.seconds,
                        now.nanoseconds / 1000000ul);
            std_fprintf(chout_p, level_as_string[level]);
            std_fprintf(chout_p,
                        FSTR(":%s:%s: "),
                        thrd_get_name(),
                        name_p);

            /* Write the custom message. */
            va_start(ap, fmt_p);
            std_vfprintf(chout_p, fmt_p, &ap);
            va_end(ap);

            chan_control(chout_p, CHAN_CONTROL_LOG_END);

            count++;
        }

        handler_p = handler_p->next_p;
    }

    sem_give(&module.sem, 1);

    return (count);
}
