/**
 * @file kernel/shell.h
 * @version 3.0.0
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

#ifndef __KERNEL_SHELL_H__
#define __KERNEL_SHELL_H__

#include "simba.h"

struct shell_history_elem_t {
    struct shell_history_elem_t *next_p;
    struct shell_history_elem_t *prev_p;
    char buf[1];
};

struct shell_line_t {
    char buf[CONFIG_SHELL_COMMAND_MAX];
    int length;
    int cursor;
};

struct shell_t {
    chan_t *chin_p;
    chan_t *chout_p;
    void *arg_p;
    const char *name_p;
    const char *username_p;
    const char *password_p;
    struct shell_line_t line;
    struct shell_line_t prev_line;
    int carriage_return_received;
    int newline_received;
    int authorized;

#if CONFIG_SHELL_MINIMAL == 0

    struct {
        struct shell_history_elem_t *head_p;
        struct shell_history_elem_t *tail_p;
        struct shell_history_elem_t *current_p;
        struct shell_line_t pattern;
        struct shell_line_t match;
        /* Command line when first UP was pressed. */
        struct shell_line_t line;
        int line_valid;
        struct {
            struct circular_heap_t heap;
            uint8_t buf[CONFIG_SHELL_HISTORY_SIZE];
        } heap;
    } history;

#endif
};


/**
 * Initialize the shell module.
 *
 * @return zero(0) or negative error code.
 */
int shell_module_init(void);

/**
 * Initialize a shell with given parameters.
 *
 * @param[in] chin_p The shell input channel. The shell waits for
 *                   commands on this channel.
 * @param[in] chout_p The shell output channel. The shell writes
 *                    responses on this channel.
 * @param[in] arg_p User supplied argument passed to all commands.
 * @param[in] name_p The shell thread name.
 * @param[in] username_p Shell login username, or NULL if no username
 *                       is required to use the shell.
 * @param[in] password_p Shell login password. This field is unused if
 *                       `username_p` is NULL.
 */
int shell_init(struct shell_t *self_p,
               chan_t *chin_p,
               chan_t *chout_p,
               void *arg_p,
               const char *name_p,
               const char *username_p,
               const char *password_p);

/**
 * The shell main function that listens for commands on the input
 * channel and send response on the output channel. All received
 * commands are passed to the debug file system function `fs_call()`
 * for execution.
 *
 * Here is an example of using the shell to list and execute debug
 * file system commands.
 *
 * @rst
 * .. code-block:: text
 *
 *    $ <tab>
 *    drivers/
 *    kernel/
 *    $ kernel/ <tab>
 *    fs/
 *    sys/
 *    thrd/
 *    $ kernel/thrd/list
 *                NAME           PARENT        STATE  PRIO   CPU  LOGMASK
 *                main                       current     0    0%     0x3f
 *                idle             main        ready   127    0%     0x3f
 *             monitor             main        ready   -80    0%     0x3f
 *    $
 * @endrst
 *
 * @param[in] arg_p Pointer to the shell arguemnt struct `struct
 *                  shell_args_t`. See the struct definition for a
 *                  description of it's content.
 *
 * @return Never returns.
 */
void *shell_main(void *arg_p);

#endif
