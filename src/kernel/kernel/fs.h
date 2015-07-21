/**
 * @file kernel/fs.h
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

#ifndef __KERNEL_FS_H__
#define __KERNEL_FS_H__

#include "simba.h"

/* Add a command to the file system with given callback. */
#if defined(__FS__)
#    define FS_COMMAND_DEFINE(command, callback) ..fs_command.. command #callback
#    define FS_COUNTER_DEFINE(name) ..fs_counter.. #name
#    define FS_PARAMETER_DEFINE(command, name, type, default)   \
    ..fs_parameter.. command #name #type
#else
#    define FS_COMMAND_DEFINE(command, callback)
#    define FS_COUNTER_DEFINE(name) extern struct fs_counter_t fs_counter_ ## name;
#    define FS_PARAMETER_DEFINE(command, name, type, default)   \
    type fs_parameter_ ## name = default;                       \
    FS_PARAMETER_CMD(name, type)
#endif

#define FS_PARAMETER_CMD(name, handler)                                 \
    int fs_parameter_cmd_ ## name (int argc,                            \
                                   const char *argv[],                  \
                                   chan_t *chout_p,                     \
                                   chan_t *chin_p)                      \
    {                                                                   \
        if (argc == 1) {                                                \
            return (fs_parameter_handler_ ## handler ## _get(&FS_PARAMETER(name), \
                                                             argc,      \
                                                             argv,      \
                                                             chout_p,   \
                                                             chin_p));  \
        } else {                                                        \
            return (fs_parameter_handler_ ## handler ## _set(&FS_PARAMETER(name), \
                                                             argc,      \
                                                             argv,      \
                                                             chout_p,   \
                                                             chin_p));  \
        }                                                               \
    }

/* Increment given counter. */
#define FS_COUNTER_INC(name, _value) (fs_counter_ ## name).value += (_value)

/* Get and set parameter value. */
#define FS_PARAMETER(name) fs_parameter_ ## name

/* Counter node. */
struct fs_counter_t {
    struct fs_counter_t *next_p;
    long long unsigned int value;
    FAR const char *name_p;
};

/* File system node. */
struct fs_node_t {
    int next;
    FAR const char *name_p;
    struct {
        int begin;
        int end;
        int len;
    } children;
    int (*callback)(int argc,
                    const char *argv[],
                    void *out_p,
                    void *in_p);
};

/**
 * Call given file system command.
 * @param[in] command_p Command string to call. May be
 *                      modified by this function.
 * @param[in] chin Input channel.
 * @param[in] chout Output channel.
 * @return zero(0) or negative error code.
 */
int fs_call(char *command_p,
            chan_t *chin_p,
            chan_t *chout_p);

/**
 * List files and directories in 'path'.
 * @param[in] path Directory to list.
 * @param[in] filter Filter files and folders.
 * @param[in] chout Output chan.
 * @return zero(0) or negative error code.
 */
int fs_list(const char *path_p,
            const char *filter_p,
            chan_t *chout_p);

/**
 * Auto-complete given 'path'.
 * @param[in,out] path Path to auto-complete.
 * @param[in] chout Output chan.
 * @return >=1 if completion happened. Match length.
 *         0 if no completion happend,
 *         or negative error code.
 */
int fs_auto_complete(char *path_p, chan_t *chout_p);

/**
 * Split buffer into path and command.
 * @param[in] buf Buffer to split.
 * @param[out] path Path part.
 * @param[out] cmd Command part.
 * @return zero(0) or negative error code.
 */
void fs_split(char *buf_p, char **path_pp, char **cmd_pp);

/**
 * Merge path and command previously split using fs_split().
 * @param[in] path Path from spilt.
 * @param[in] cmd Command from split.
 * @return zero(0) or negative error code.
 */
void fs_merge(char *path_p, char *cmd_p);

int fs_parameter_handler_int_set(int *parameter_p,
                                 int argc,
                                 const char *argv[],
                                 chan_t *chout_p,
                                 chan_t *chin_p);

int fs_parameter_handler_int_get(int *parameter_p,
                                 int argc,
                                 const char *argv[],
                                 chan_t *chout_p,
                                 chan_t *chin_p);

#endif
