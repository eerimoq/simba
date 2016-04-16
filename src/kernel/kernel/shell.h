/**
 * @file kernel/shell.h
 * @version 0.3.0
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

struct shell_args_t {
    /** The shell input channel. The shell waits for commands on this
        channel. */
    chan_t *chin_p;
    /** The shell output channel. The shell writes responses on this
        channel. */
    chan_t *chout_p;
    /** User supplied argument passed to all commands. */
    void *arg_p;
    /** The shell thread name. */
    const char *name_p;
    /** Shell login username, or NULL if no username is required to
        use the shell. */
    const char *username_p;
    /** Shell login password. This field is unused if `username_p` is
        NULL. */
    const char *password_p;
};

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
