/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

struct command_t {
    char *command_p;
    int res;
    char *output_p;
};

int test_init(void)
{
    /* Call init two times. */
    BTASSERT(log_module_init() == 0);
    BTASSERT(log_module_init() == 0);

    return (0);
}

int test_print(void)
{
    struct log_object_t foo;
    struct log_object_t bar;

    /* Initialize the log objects. */
    BTASSERT(log_object_init(&foo,
                             "foo",
                             LOG_UPTO(INFO)) == 0);
    BTASSERT(log_object_init(&bar,
                             "bar",
                             LOG_UPTO(DEBUG)) == 0);

    /* Write on INFO level. */
    BTASSERT(log_object_print(&foo, LOG_INFO, FSTR("x = %d\r\n"), 1) == 1);
    BTASSERT(log_object_print(&bar, LOG_INFO, FSTR("y = %d\r\n"), 2) == 1);

    /* Write on DEBUG level. */
    BTASSERT(log_object_print(&foo, LOG_DEBUG, FSTR("m = %d\r\n"), 3) == 0);
    BTASSERT(log_object_print(&bar, LOG_DEBUG, FSTR("n = %d\r\n"), 4) == 1);

    /* Write using the thread log mask instead of the log object
       mask. */
    BTASSERT(log_object_print(NULL, LOG_DEBUG, FSTR("k = %d\r\n"), 5) == 0);
    BTASSERT(log_object_print(NULL, LOG_ERROR, FSTR("l = %d\r\n"), 6) == 1);

    return (0);
}

int test_object(void)
{
    struct log_object_t foo;

    /* Initialize the log objects. */
    BTASSERT(log_object_init(&foo,
                             "foo",
                             LOG_UPTO(INFO)) == 0);

    /* Add our object, and the remove it. */
    BTASSERT(log_add_object(&foo) == 0);
    BTASSERT(log_remove_object(&foo) == 0);
    BTASSERT(log_remove_object(&foo) == 1);

    return (0);
}

int test_handler(void)
{
    struct log_object_t foo;
    struct log_handler_t handler;

    /* Initialize the log objects. */
    BTASSERT(log_object_init(&foo,
                             "foo",
                             LOG_UPTO(INFO)) == 0);
    BTASSERT(log_handler_init(&handler, sys_get_stdout()) == 0);

    /* This should be printed once for the default handler. */
    BTASSERT(log_object_print(&foo,
                              LOG_INFO,
                              FSTR("one handler\r\n")) == 1);

    /* Add our handler. */
    BTASSERT(log_add_handler(&handler) == 0);

    /* This should be printed twice, one for the default handler and
       once for our handler. */
    BTASSERT(log_object_print(&foo,
                              LOG_INFO,
                              FSTR("two handlers\r\n")) == 2);

    /* Remove our handler. */
    BTASSERT(log_remove_handler(&handler) == 0);
    BTASSERT(log_remove_handler(&handler) == 1);

    /* This should be printed once for the default handler. */
    BTASSERT(log_object_print(&foo,
                              LOG_INFO,
                              FSTR("one handler again\r\n")) == 1);

    return (0);
}

int test_log_mask(void)
{
    struct log_object_t foo;

    /* Initialize the log objects. */
    BTASSERT(log_object_init(&foo,
                             "foo",
                             LOG_UPTO(INFO)) == 0);
    BTASSERT(log_object_get_log_mask(&foo) == 0x0f);

    /* Log all. */
    BTASSERT(log_object_set_log_mask(&foo, LOG_ALL) == 0);
    BTASSERT(log_object_get_log_mask(&foo) == 0x1f);

    /* Log none. */
    BTASSERT(log_object_set_log_mask(&foo, LOG_NONE) == 0);
    BTASSERT(log_object_get_log_mask(&foo) == 0x00);

    /* Log error. */
    BTASSERT(log_object_set_log_mask(&foo, LOG_MASK(ERROR)) == 0);
    BTASSERT(log_object_get_log_mask(&foo) == 0x02);

    /* Invalid levels are discarded. */
    BTASSERT(log_object_set_log_mask(&foo, 0xf0) == 0);
    BTASSERT(log_object_get_log_mask(&foo) == 0x10);

    /* Is enabled for. */
    BTASSERT(log_object_set_log_mask(&foo, LOG_MASK(ERROR)) == 0);
    BTASSERT(log_object_is_enabled_for(&foo, LOG_INFO) == 0);
    BTASSERT(log_object_is_enabled_for(&foo, LOG_ERROR) == 1);
    thrd_set_log_mask(thrd_self(), 0x00);
    BTASSERT(log_object_is_enabled_for(NULL, LOG_ERROR) == 0);

    return (0);
}

int test_fs(void)
{
    char command[64];
    struct queue_t queue;
    uint8_t buf[256];
    struct command_t *command_p;
    struct command_t commands[] = {
        { "/debug/log/list",
          0,
          "OBJECT-NAME       MASK\r\n"
          "log               0x0f\r\n"
        },
        { "/debug/log/print foo", 0, NULL },
        { "/debug/log/set_log_mask log 0xff", 0, NULL },

        {
            "/debug/log/list d",
            -EINVAL,
            "Usage: list\r\n"
        },
        {
            "/debug/log/print d d",
            -EINVAL,
            "Usage: print <string>\r\n"
        },
        {
            "/debug/log/set_log_mask invalid_object 0xff",
            -EINVAL,
            "No log object with name 'invalid_object'.\r\n" },
        {
            "/debug/log/set_log_mask missing_mask",
            -EINVAL,
            "Usage: set_log_mask <object> <mask>\r\n"
        },
        {
            "/debug/log/set_log_mask bad_mask ds",
            -EINVAL,
            "Bad mask 'ds'.\r\n"
        },
        { NULL, 0, NULL }
    };

    BTASSERT(queue_init(&queue, &buf[0], sizeof(buf)) == 0);

    command_p = &commands[0];

    while (command_p->command_p != NULL) {
        strcpy(command, command_p->command_p);
        BTASSERT(fs_call(command,
                         NULL,
                         &queue,
                         NULL) == command_p->res);

        if (command_p->output_p != NULL) {
            BTASSERTI(harness_expect(&queue,
                                     command_p->output_p,
                                     NULL), ==, strlen(command_p->output_p));
        }

        command_p++;
    }

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_print, "test_print" },
        { test_object, "test_object" },
        { test_handler, "test_handler" },
        { test_log_mask, "test_log_mask" },
        { test_fs, "test_fs" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
