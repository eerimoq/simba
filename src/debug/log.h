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

#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#include "simba.h"

/* Severity levels' log masks. */

/** An unhandleable error that results in a program crash. */
#define LOG_FATAL     0
/** A handable error conditions. */
#define LOG_ERROR     1
/** A warning. */
#define LOG_WARNING   2
/** Generic (useful) information about system operation. */
#define LOG_INFO      3
/** Developer debugging messages. */
#define LOG_DEBUG     4

/** Craete a log mask with given level set. */
#define LOG_MASK(level) (1 << (LOG_ ## level))

/** Set all levels up to and including given level. */
#define LOG_UPTO(level) ((1 << (LOG_ ## level + 1)) - 1)

/** Set all levels. */
#define LOG_ALL    LOG_UPTO(DEBUG)

/** Clear all levels. */
#define LOG_NONE   0x00

struct log_handler_t {
    void *chout_p;
    struct log_handler_t *next_p;
};

struct log_object_t {
    const char *name_p;
    char mask;
    struct log_object_t *next_p;
};

/**
 * Initialize the logging module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int log_module_init(void);

/**
 * Initialize given log object with given name and mask.
 *
 * @param[in] self_p Log object to initialize.
 * @param[in] name_p Log object name.
 * @param[in] mask Log object mask.
 *
 * @return zero(0) or negative error code.
 */
int log_object_init(struct log_object_t *self_p,
                    const char *name_p,
                    char mask);

/**
 * Set given log mask for given log object.
 *
 * @param[in] self_p Log object.
 * @param[in] mask Log object mask.
 *
 * @return zero(0) or negative error code.
 */
int log_object_set_log_mask(struct log_object_t *self_p,
                            char mask);

/**
 * Get the log mask of given log object.
 *
 * @param[in] self_p Log object.
 *
 * @return Log mask.
 */
char log_object_get_log_mask(struct log_object_t *self_p);

/**
 * Check if given log level is enabled in given log object.
 *
 * @param[in] self_p Log object, or NULL to check the level in the
 *                   thread log mask.
 * @param[in] level Log level to check.
 *
 * @return true(1) if given log level is enabled, false(0) if given
 *         log level is disabled, otherwise negative error code.
 */
int log_object_is_enabled_for(struct log_object_t *self_p,
                              int level);

/**
 * Check if given log level is set in the log object mask. If so,
 * format a log entry and write it to all log handlers.
 *
 * ``self_p`` may be NULL, and in that case the current thread's log
 * mask is used instead of the log object mask.
 *
 * @param[in] self_p Log object, or NULL to use the thread's log mask.
 * @param[in] level Log level.
 * @param[in] fmt_p Log format string.
 * @param[in] ... Variable argument list.
 *
 * @return zero(0) or negative error code.
 */
int log_object_print(struct log_object_t *self_p,
                     int level,
                     const char *fmt_p,
                     ...);

/**
 * Initialize given log handler with given output channel.
 *
 * @param[in] self_p Log handler to initialize.
 * @param[in] chout_p Output handler.
 *
 * @return zero(0) or negative error code.
 */
int log_handler_init(struct log_handler_t *self_p,
                     void *chout_p);

/**
 * Add given log handler to the list of log handlers. Log entries will
 * be written to all log handlers in the list.
 *
 * @param[in] handler_p Log handler to add.
 *
 * @return zero(0) or negative error code.
 */
int log_add_handler(struct log_handler_t *handler_p);

/**
 * Remove given log handler from the list of log handlers.
 *
 * @param[in] handler_p Log handler to remove.
 *
 * @return zero(0) or negative error code.
 */
int log_remove_handler(struct log_handler_t *handler_p);

/**
 * Add given log object to the list of log objects. There are file
 * system commands to list all log objects in the list and also modify
 * their log mask.
 *
 * @param[in] object_p Log object to add.
 *
 * @return zero(0) or negative error code.
 */
int log_add_object(struct log_object_t *object_p);

/**
 * Remove given log object from the list of log objects.
 *
 * @param[in] object_p Object to remove.
 *
 * @return zero(0) or negative error code.
 */
int log_remove_object(struct log_object_t *object_p);

/**
 * Set the output channel of the default log handler.
 *
 * @param[in] chout_p Channel to set as the default output
 *                    channel. May be NULL if no output should be
 *                    written.
 *
 * @return zero(0) or negative error code.
 */
int log_set_default_handler_output_channel(void *chout_p);

#endif
