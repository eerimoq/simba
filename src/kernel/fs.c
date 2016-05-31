/**
 * @file fs.c
 * @version 0.5.0
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
#include <ctype.h>

#define FS_COMMAND_ARGS_MAX 16
#define FS_COMMAND_MAX 64

#define FS_NAME_MAX 64

struct state_t {
    struct fs_command_t *commands_p;
    struct fs_counter_t *counters_p;
    struct fs_parameter_t *parameters_p;
};

static struct fs_command_t cmd_counters_list;
static struct fs_command_t cmd_counters_reset;
static struct fs_command_t cmd_parameters_list;

static struct state_t state;

static int counter_get(struct fs_counter_t *counter_p,
                       chan_t *chout_p)
{
    std_fprintf(chout_p,
                FSTR("%08lx%08lx\r\n"),
                (long)(counter_p->value >> 32),
                (long)(counter_p->value & 0xffffffff));
    
    return (0);
}

static int counter_set(struct fs_counter_t *counter_p)
{
    counter_p->value = 0;

    return (0);
}

static int counters_list(int argc,
                         const char *argv[],
                         chan_t *chout_p,
                         chan_t *chin_p,
                         void *arg_p,
                         void *call_arg_p)
{
    struct fs_counter_t *counter_p;
    char buf[FS_NAME_MAX];

    std_fprintf(chout_p,
                FSTR("NAME                                                 VALUE\r\n"));

    counter_p = state.counters_p;
    
    while (counter_p != NULL) {
        std_strcpy(buf, counter_p->command.path_p);
        std_fprintf(chout_p, FSTR("%-53s"), buf);
        counter_p->command.callback(FS_ARGC_GET,
                                    NULL,
                                    chout_p,
                                    NULL,
                                    counter_p->command.arg_p,
                                    arg_p);

        counter_p = counter_p->next_p;
    }

    return (0);
}

static int counters_reset(int argc,
                          const char *argv[],
                          chan_t *chout_p,
                          chan_t *chin_p,
                          void *arg_p,
                          void *call_arg_p)
{
    struct fs_counter_t *counter_p;

    counter_p = state.counters_p;
    
    while (counter_p != NULL) {
        counter_p->command.callback(0,
                                    NULL,
                                    chout_p,
                                    NULL,
                                    counter_p->command.arg_p,
                                    arg_p);

        counter_p = counter_p->next_p;
    }

    return (0);
}

int parameters_list(int argc,
                    const char *argv[],
                    chan_t *chout_p,
                    chan_t *chin_p,
                    void *arg_p,
                    void *call_arg_p)
{
    struct fs_parameter_t *parameter_p;
 
    parameter_p = state.parameters_p;
    
    while (parameter_p != NULL) {
        std_fprintf(chout_p, parameter_p->command.path_p);
        std_fprintf(chout_p, FSTR(" "));
        parameter_p->command.callback(FS_ARGC_GET,
                                      NULL,
                                      chout_p,
                                      NULL,
                                      parameter_p->command.arg_p,
                                      call_arg_p);

        parameter_p = parameter_p->next_p;
    }

    return (0);
}

/**
 * Remove whitespaces from beginning and end of string. Replace sequence
 * of whitespaces within the string with a single space.
 */
static void pack(char *string_p)
{
    char *write_p;
    char *read_p;

    write_p = string_p;
    read_p = string_p;

    if (*read_p == '\0') {
        return;
    }

    /* Remove beginning whitespaces.*/
    while (isspace((int)*read_p)) {
        read_p++;
    }

    /* Merge internal whitespaces.*/
    while (*read_p != '\0') {
        *write_p++ = *read_p++;
        while (isspace((int)*read_p) && isspace((int)read_p[1])) {
            read_p++;
        }
    }

    /* Remove trailing whitespace.*/
    if (isspace((int)write_p[-1])) {
        write_p--;
    }

    *write_p = '\0';
}

static int command_parse(char *command_p, const char *argv[])
{
    int argc;

    pack(command_p);
    argc = 0;
    argv[argc++] = command_p;

    while (*command_p != '\0') {
        if (*command_p == ' ') {
            *command_p = '\0';
            command_p++;

            if (argc == FS_COMMAND_ARGS_MAX) {
                return (-E2BIG);
            }

            argv[argc++] = command_p;
        }
        command_p++;
    }

    return (argc);
}

static int counter_cmd(int argc,
                       const char *argv[],
                       chan_t *chout_p,
                       chan_t *chin_p,
                       void *cmd_arg_p,
                       void *arg_p)
{
    if (argc == FS_ARGC_GET) {
        return (counter_get(cmd_arg_p, chout_p));
    } else {
        return (counter_set(cmd_arg_p));
    }
}

int fs_module_init()
{
    state.commands_p = NULL;
    state.counters_p = NULL;
    state.parameters_p = NULL;

    fs_command_init(&cmd_counters_list,
                    FSTR("/kernel/fs/counters_list"),
                    counters_list,
                    NULL);
    fs_command_register(&cmd_counters_list);

    fs_command_init(&cmd_counters_reset,
                    FSTR("/kernel/fs/counters_reset"), 
                    counters_reset,
                    NULL);
    fs_command_register(&cmd_counters_reset);

    fs_command_init(&cmd_parameters_list,
                    FSTR("/kernel/fs/parameters_list"), 
                    parameters_list,
                    NULL);
    fs_command_register(&cmd_parameters_list);

    return (0);
}

int fs_call(char *command_p,
            chan_t *chin_p,
            chan_t *chout_p,
            void *arg_p)
{
    ASSERTN(command_p != NULL, EINVAL);

    int argc, skip_slash;
    const char *argv[FS_COMMAND_ARGS_MAX];
    struct fs_command_t *current_p;

    argc = command_parse(command_p, argv);

    if (argc < 0) {
        return (argc);
    }

    /* Find given command. */
    current_p = state.commands_p;
    skip_slash = (argv[0][0] != '/');

    while (current_p != NULL) {
        if (std_strcmp(argv[0], &current_p->path_p[skip_slash]) == 0) {
            return (current_p->callback(argc,
                                        argv,
                                        chout_p,
                                        chin_p,
                                        current_p->arg_p,
                                        arg_p));
        }

        current_p = current_p->next_p;
    }

    std_fprintf(chout_p, FSTR("%s: command not found\r\n"), argv[0]);

    return (-ENOENT);
}

int fs_list(const char *path_p,
            const char *filter_p,
            chan_t *chout_p)
{
    ASSERTN(path_p != NULL, EINVAL);
    ASSERTN(chout_p != NULL, EINVAL);

    int buf_length, path_offset, filter_offset, path_length;
    struct fs_command_t *command_p;
    char buf[64], next_char;

    filter_offset = path_length = strlen(path_p);
    path_offset = 0;

    if (path_p[0] != '/') {
        path_offset = 1;
        filter_offset++;
    }

    if (path_length > 0) {
        if (path_p[path_length - 1] == '/') {
            path_length--;
        } else {
            filter_offset++;
        }
    }

    buf_length = -1;

    /* Find all paths matching given path and filter and output the
       file or folder matching the filter. */
    command_p = state.commands_p;

    while (command_p != NULL) {
        /* Path match? */
        if (std_strncmp(&command_p->path_p[path_offset],
                        path_p,
                        path_length) == 0) {
            /* Filter match? */
            if ((filter_p == NULL)
                || (std_strncmp(&command_p->path_p[filter_offset],
                                filter_p,
                                strlen(filter_p)) == 0)) {
                /* Output new files and folders. */
                if ((buf_length == -1)
                    || (std_strncmp(&command_p->path_p[filter_offset],
                                    buf,
                                    buf_length) != 0)) {
                    /* Get file or folder name. */
                    buf_length = 0;

                    do {
                        next_char = command_p->path_p[filter_offset + buf_length];
                        
                        buf[buf_length] = next_char;
                        buf_length++;
                    } while ((next_char != '\0') && (next_char != '/'));

                    buf[buf_length] = '\0';

                    std_fprintf(chout_p, FSTR("%s\r\n"), buf);
                }
            }
        }

        command_p = command_p->next_p;
    }

    return (0);
}

int fs_auto_complete(char *path_p)
{
    ASSERTN(path_p != NULL, EINVAL);

    char next_char;
    int mismatch, path_length, offset, size;
    struct fs_command_t *command_p, *next_p;

    offset = 0;
    size = path_length = strlen(path_p);

    /* Skip the leading slash in the registered commands if it is
       missing in the input path. */
    if (path_p[0] != '/') {
        offset = 1;
    }

    /* Find the first command matching given path. */
    command_p = state.commands_p;

    while (command_p != NULL) {
        if (std_strncmp(&command_p->path_p[offset],
                        path_p,
                        size) == 0) {
            break;
        }

        command_p = command_p->next_p;
    }

    /* No command matching the path. */
    if (command_p == NULL) {
        return (-ENOENT);
    }

    /* Auto-complete given path. Compare the next character each
       iteration.

       Example:
       path_p = "/tm"
       commands = ["/tmp/foo", "/tmp/bar", "/zoo/lander"]
       auto-completed = "/tmp/"
    */
    while (1) {
        mismatch = 0;
        next_char = command_p->path_p[offset + size];

        /* It's a match if all commands matching path has the same
           next character. */
        next_p = command_p;

        while ((next_p != NULL)
               && (std_strncmp(&next_p->path_p[offset],
                               path_p,
                               size) == 0)) {
            if (next_p->path_p[offset + size] != next_char) {
                mismatch = 1;
                break;
            }

            next_p = next_p->next_p;
        }

        /* Completion happend? */
        if (mismatch == 0) {
            path_p[size] = next_char;
            size++;

            /* Auto-complete one directory at a time. */
            if (next_char == '/') {
                break;
            } else if (next_char == '\0') {
                /* Append a space on commands. */
                path_p[size - 1] = ' ';
                path_p[size] = '\0';
                break;
            }
        } else {
            break;
        }
    }

    path_p[size] = '\0';

    return (size - path_length);
}

void fs_split(char *buf_p, char **path_pp, char **cmd_pp)
{
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(path_pp != NULL, EINVAL);
    ASSERTN(*path_pp != NULL, EINVAL);
    ASSERTN(cmd_pp != NULL, EINVAL);
    ASSERTN(*cmd_pp != NULL, EINVAL);

    *path_pp = buf_p;
    *cmd_pp = NULL;

    while (*buf_p != '\0') {
        if (*buf_p == '/') {
            *cmd_pp = buf_p;
        }

        buf_p++;
    }

    if (*cmd_pp != NULL) {
        **cmd_pp = '\0';
        (*cmd_pp)++;
    } else {
        *cmd_pp = *path_pp;
        *path_pp = "";
    }
}

void fs_merge(char *path_p, char *cmd_p)
{
    ASSERTN(path_p != NULL, EINVAL);
    ASSERTN(cmd_p != NULL, EINVAL);

    if (*path_p != '\0') {
        *--cmd_p = '/';
    }
}

int fs_command_init(struct fs_command_t *self_p,
                    const FAR char *path_p,
                    fs_callback_t callback,
                    void *arg_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(path_p != NULL, EINVAL);
    ASSERTN(callback != NULL, EINVAL);

    self_p->next_p = NULL;
    self_p->path_p = path_p;
    self_p->callback = callback;
    self_p->arg_p = arg_p;

    return (0);
}

int fs_command_register(struct fs_command_t *command_p)
{
    ASSERTN(command_p != NULL, EINVAL);

    int res = -1;
    struct fs_command_t *current_p, *prev_p;

    /* Insert in alphabetical order. */
    prev_p = NULL;
    current_p = state.commands_p;

    while (current_p != NULL) {
        if (std_strcmp_f(command_p->path_p, current_p->path_p) < 0) {
            break;
        }

        prev_p = current_p;
        current_p = current_p->next_p;
    }

    /* Insert into the list. */
    command_p->next_p = current_p;

    if (prev_p == NULL) {
        state.commands_p = command_p;
    } else {
        prev_p->next_p = command_p;
    }

    return (res);
}

int fs_command_deregister(struct fs_command_t *command_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

int fs_counter_init(struct fs_counter_t *self_p,
                    const FAR char *path_p,
                    uint64_t value)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(path_p != NULL, EINVAL);

    fs_command_init(&self_p->command,
                    path_p,
                    counter_cmd,
                    self_p);

    self_p->value = value;
    self_p->next_p = NULL;

    return (0);
}

int fs_counter_increment(struct fs_counter_t *self_p,
                         uint64_t value)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->value += value;

    return (0);
}

int fs_counter_register(struct fs_counter_t *counter_p)
{
    ASSERTN(counter_p != NULL, EINVAL);

    /* Insert counter into the command list and the counter list. */
    fs_command_register(&counter_p->command);

    counter_p->next_p = state.counters_p;
    state.counters_p = counter_p;

    return (0);
}

int fs_counter_deregister(struct fs_counter_t *counter_p)
{
    ASSERTN(0, ENOSYS);

    return (0);
}

int fs_parameter_init(struct fs_parameter_t *self_p,
                      const FAR char *path_p,
                      fs_callback_t callback,
                      void *value_p)
{
    ASSERTN(self_p != NULL, ENOSYS);
    ASSERTN(path_p != NULL, ENOSYS);
    ASSERTN(callback != NULL, ENOSYS);

    fs_command_init(&self_p->command,
                    path_p,
                    callback,
                    value_p);

    self_p->next_p = NULL;

    return (0);
}

int fs_parameter_register(struct fs_parameter_t *parameter_p)
{
    ASSERTN(parameter_p != NULL, ENOSYS);

    /* Insert counter into the command list and the counter list. */
    fs_command_register(&parameter_p->command);

    parameter_p->next_p = state.parameters_p;
    state.parameters_p = parameter_p;

    return (0);
}

int fs_parameter_deregister(struct fs_parameter_t *parameter_p)
{
    ASSERTN(0, ENOSYS);

    return (0);
}

int fs_cmd_parameter_int(int argc,
                         const char *argv[],
                         chan_t *chout_p,
                         chan_t *chin_p,
                         void *arg_p,
                         void *call_arg_p)
{
    int *value_p = arg_p;
    long value;

    if (argc == FS_ARGC_GET) {
        std_fprintf(chout_p, FSTR("%d\r\n"), *value_p);
    } else {
        if (std_strtol(argv[1], &value)) {
            std_fprintf(chout_p, FSTR("bad value '%s'\r\n"), argv[1]);
            return (1);
        }
        
        *value_p = value;
    }
    
    return (0);
}
