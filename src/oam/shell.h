/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#ifndef __OAM_SHELL_H__
#define __OAM_SHELL_H__

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
    void *chin_p;
    void *chout_p;
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
 * Initialize the shell module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
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
               void *chin_p,
               void *chout_p,
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
 *                NAME        STATE  PRIO   CPU  LOGMASK
 *                main      current     0    0%     0x0f
 *                idle        ready   127    0%     0x0f
 *             monitor        ready   -80    0%     0x0f
 *    $
 * @endrst
 *
 * @param[in] arg_p Pointer to the shell arguemnt struct `struct
 *                  shell_t`. See the struct definition for a
 *                  description of it's content.
 *
 * @return Never returns.
 */
void *shell_main(void *arg_p);

#endif
