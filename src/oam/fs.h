/**
 * @file kernel/fs.h
 * @version 3.1.0
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

#ifndef __KERNEL_FS_H__
#define __KERNEL_FS_H__

#include "simba.h"

/**
 * Length of argv for get.
 */
#define FS_ARGC_GET 1

typedef int (*fs_callback_t)(int argc,
                             const char *argv[],
                             chan_t *out_p,
                             chan_t *in_p,
                             void *arg_p,
                             void *call_arg_p);

/* Command. */
struct fs_command_t {
    const FAR char *path_p;
    fs_callback_t callback;
    void *arg_p;
    void *next_p;
};

/* Counter. */
struct fs_counter_t {
    struct fs_command_t command;
    long long unsigned int value;
    void *next_p;
};

/* Parameter. */
struct fs_parameter_t {
    struct fs_command_t command;
    void *next_p;
};

/**
 * Initialize the file system module.
 *
 * @return zero(0) or negative error code.
 */
int fs_module_init(void);

/**
 * Call given file system command with given input and output
 * channels.
 *
 * @param[in] command_p Command string to call. May be modified by
 *                      this function.
 * @param[in] chin_p Input channel.
 * @param[in] chout_p Output channel.
 * @param[in] arg_p User argument.
 *
 * @return zero(0) or negative error code.
 */
int fs_call(char *command_p,
            chan_t *chin_p,
            chan_t *chout_p,
            void *arg_p);

/**
 * List files (callbacks) and directories in given path. Optionally
 * with given filter. The list is written to the output channel.
 *
 * @param[in] path_p Directory to list.
 * @param[in] filter_p Filter out files and folders.
 * @param[in] chout_p Output chan.
 *
 * @return zero(0) or negative error code.
 */
int fs_list(const char *path_p,
            const char *filter_p,
            chan_t *chout_p);

/**
 * Auto-complete given path.
 *
 * @param[in,out] path_p Path to auto-complete.
 *
 * @return >=1 if completion happened. Match length.
 *         0 if no completion happend,
 *         or negative error code.
 */
int fs_auto_complete(char *path_p);

/**
 * Split buffer into path and command inplace.
 *
 * @param[in] buf_p Buffer to split.
 * @param[out] path_pp Path or NULL if no path was found.
 * @param[out] cmd_pp Command or empty string.
 *
 * @return zero(0) or negative error code.
 */
void fs_split(char *buf_p, char **path_pp, char **cmd_pp);

/**
 * Merge path and command previously split using `fs_split()`.
 *
 * @param[in] path_p Path from spilt.
 * @param[in] cmd_p Command from split.
 *
 * @return zero(0) or negative error code.
 */
void fs_merge(char *path_p, char *cmd_p);

/**
 * Initialize given command.
 *
 * @param[in] self_p Command to initialize.
 * @param[in] path_p Path to register.
 * @param[in] callback Command callback function.
 * @param[in] arg_p Callback argument.
 *
 * @return zero(0) or negative error code.
 */
int fs_command_init(struct fs_command_t *self_p,
                    const FAR char *path_p,
                    fs_callback_t callback,
                    void *arg_p);

/**
 * Register given command.
 *
 * @param[in] self_p Command to register.
 *
 * @return zero(0) or negative error code.
 */
int fs_command_register(struct fs_command_t *command_p);

/**
 * Deregister given command.
 *
 * @param[in] self_p Command to deregister.
 *
 * @return zero(0) or negative error code.
 */
int fs_command_deregister(struct fs_command_t *command_p);

/**
 * Initialize given counter.
 *
 * @param[in] self_p Counter to initialize.
 * @param[in] path_p Path to register.
 * @param[in] value Initial value of the counter.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_init(struct fs_counter_t *self_p,
                    const FAR char *path_p,
                    uint64_t value);

/**
 * Increment given counter.
 *
 * @param[in] self_p Command to initialize.
 * @param[in] value Increment value.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_increment(struct fs_counter_t *self_p,
                         uint64_t value);

/**
 * Register given counter.
 *
 * @param[in] self_p Counter to register.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_register(struct fs_counter_t *counter_p);

/**
 * Deregister given counter.
 *
 * @param[in] self_p Counter to deregister.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_deregister(struct fs_counter_t *counter_p);

/**
 * Initialize given parameter.
 *
 * @param[in] self_p Parameter to initialize.
 * @param[in] path_p Path to register.
 * @param[in] callback Command callback function.
 * @param[in] value_p Value.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_init(struct fs_parameter_t *self_p,
                      const FAR char *path_p,
                      fs_callback_t callback,
                      void *value_p);

/**
 * Register given parameter.
 *
 * @param[in] self_p Parameter to register.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_register(struct fs_parameter_t *parameter_p);

/**
 * Deregister given parameter.
 *
 * @param[in] self_p Parameter to deregister.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_deregister(struct fs_parameter_t *parameter_p);

/**
 * Command callback for an int parameter.
 */
int fs_cmd_parameter_int(int argc,
                         const char *argv[],
                         chan_t *chout_p,
                         chan_t *chin_p,
                         void *arg_p,
                         void *call_arg_p);

#endif
