/**
 * @file main.c
 * @version 0.2.0
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

#define OUR_PARAMETER_DEFAULT 5
#define BUFFER_SIZE 512

FS_COMMAND_DEFINE("/tmp/foo", tmp_foo);
FS_COMMAND_DEFINE("/tmp/bar", tmp_bar);

COUNTER_DEFINE("/my/counter", my_counter);
COUNTER_DEFINE("/your/counter", your_counter);

PARAMETER_DEFINE("/our/parameter", our_parameter, int, OUR_PARAMETER_DEFAULT);

int tmp_foo(int argc,
            const char *argv[],
            struct chan_t *out_p,
            struct chan_t *in_p)
{
    UNUSED(out_p);
    UNUSED(in_p);

    BTASSERT(argc == 3);
    std_fprintf(out_p,
                FSTR("argc = %d, argv[0] = %s, argv[1] = %s, argv[2] = %s\n"),
                argc,
                argv[0],
                argv[1],
                argv[2]);

    return (0);
}

int tmp_bar(int argc,
            const char *argv[],
            struct chan_t *out_p,
            struct chan_t *in_p)
{
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(out_p);
    UNUSED(in_p);

    return (0);
}

static char qoutbuf[BUFFER_SIZE];
static QUEUE_INIT_DECL(qout, qoutbuf, sizeof(qoutbuf));

static int read_until(char *buf_p, const char *pattern)
{
    char c;
    size_t length = 0;
    size_t pattern_length = strlen(pattern);

    while (length < BUFFER_SIZE - 1) {
        chan_read(&qout, &c, sizeof(c));
#if defined(ARCH_LINUX)
        printf("%c", c);
#endif
        *buf_p++ = c;
        length++;
        *buf_p = '\0';

        /* Compare to pattern. */
        if (length >= pattern_length) {
            if (!strcmp(&buf_p[-pattern_length], pattern)) {
                return (1);
            }
        }
    }

    return (0);
}

static int test_command(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    strcpy(buf, "  /tmp/foo     foo1 foo2  ");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);
    read_until(buf, "\n");

    strcpy(buf, "/tmp/bar 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
    BTASSERT(fs_call(buf, NULL, &qout) == -E2BIG);
    read_until(buf, "\n");

    strcpy(buf, "/tmp/bar/missing");
    BTASSERT(fs_call(buf, NULL, &qout) == -ENOENT);
    read_until(buf, "\n");

    strcpy(buf, "");
    BTASSERT(fs_call(buf, NULL, &qout) == -ENOENT);
    read_until(buf, "\n");

    strcpy(buf, "/tm");
    BTASSERT(fs_auto_complete(buf, &qout) >= 1);
    read_until(buf, "p/");

    strcpy(buf, "/tmp/b");
    BTASSERT(fs_auto_complete(buf, &qout) >= 1);
    read_until(buf, "ar ");

    strcpy(buf, "/tmp/q");
    BTASSERT(fs_auto_complete(buf, &qout) == -ENOENT);

    return (0);
}

static int test_counter(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "kernel/fs/counters_list");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);
    read_until(buf, "/your/counter                                        0000000000000000\r\n");

    COUNTER_INC(my_counter, 3);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);
    read_until(buf, "0000000000000003\r\n");

    strcpy(buf, "kernel/fs/counters_reset");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);
    read_until(buf, "0000000000000000\r\n");

    return (0);
}

static int test_parameter(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "kernel/fs/parameters_list");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);
    read_until(buf, "/our/parameter 5\r\n");

    BTASSERT(PARAMETER(our_parameter) == OUR_PARAMETER_DEFAULT);
    PARAMETER(our_parameter) = 1;
    BTASSERT(PARAMETER(our_parameter) == 1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_command, "test_command" },
        { test_counter, "test_counter" },
        { test_parameter, "test_parameter" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
