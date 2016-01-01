/**
 * @file kernel/shell.h
 * @version 0.1.0
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

#ifndef __KERNEL_SHELL_H__
#define __KERNEL_SHELL_H__

#include "simba.h"

struct shell_args_t {
    chan_t *chin_p;
    chan_t *chout_p;
    const char *name_p;
    const char *username_p;
    const char *password_p;
};

/**
 * The shell main function that listens for commands on the input
 * channel and send response on the output channel. All received
 * commands are passed to the debug file system function `fs_call()`
 * for execution.
 *
 * @param[in] arg_p Pointer to 'struct shell_args_t' with shell
 *                  configuration.
 *
 * @return Never returns.
 */
void *shell_main(void *arg_p);

#endif
