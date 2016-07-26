/**
 * @file main.c
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

#include "simba.h"

#define OUR_PARAMETER_DEFAULT 5
#define BUFFER_SIZE 512

static struct fs_command_t foo_bar;
static struct fs_command_t bar;

static struct fs_counter_t my_counter;
static struct fs_counter_t your_counter;

static int our_parameter_value = OUR_PARAMETER_DEFAULT;
static struct fs_parameter_t our_parameter;

static int tmp_foo_bar(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
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

static int tmp_bar(int argc,
                   const char *argv[],
                   chan_t *out_p,
                   chan_t *in_p,
                   void *arg_p,
                   void *call_arg_p)
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
        std_printf(FSTR("%c"), c);
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

    strcpy(buf, "  /tmp/foo/bar     foo1 foo2  ");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "\n");

    strcpy(buf, "/tmp/bar 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -E2BIG);

    strcpy(buf, "/tmp/bar/missing");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -ENOENT);
    read_until(buf, "\n");

    strcpy(buf, "");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -ENOENT);
    read_until(buf, "\n");

    strcpy(buf, "/tmp/q");
    BTASSERT(fs_auto_complete(buf) == -ENOENT);

    strcpy(buf, "/tm");
    BTASSERT(fs_auto_complete(buf) == 2);
    BTASSERT(strcmp(buf, "/tmp/") == 0);

    strcpy(buf, "/tmp/f");
    BTASSERT(fs_auto_complete(buf) == 3);
    BTASSERT(strcmp(buf, "/tmp/foo/") == 0);

    strcpy(buf, "/tmp/foo/");
    BTASSERT(fs_auto_complete(buf) == 4);
    BTASSERT(strcmp(buf, "/tmp/foo/bar ") == 0);

    strcpy(buf, "");
    BTASSERT(fs_auto_complete(buf) == 0);
    BTASSERT(strcmp(buf, "") == 0);

    return (0);
}

static int test_counter(struct harness_t *harness_p)
{
    char buf[384];

    strcpy(buf, "kernel/fs/counters_list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "/your/counter                                        0000000000000000\r\n");

    fs_counter_increment(&my_counter, 3);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "0000000000000003\r\n");

    strcpy(buf, "kernel/fs/counters_reset");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "0000000000000000\r\n");

    return (0);
}

static int test_parameter(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "kernel/fs/parameters_list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "/our/parameter 5\r\n");

    BTASSERT(our_parameter_value == OUR_PARAMETER_DEFAULT);
    our_parameter_value = 1;
    BTASSERT(our_parameter_value == 1);

    strcpy(buf, "/our/parameter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "1\r\n");

    strcpy(buf, "/our/parameter 3");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "/our/parameter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "3\r\n");

    return (0);
}

static int test_list(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "kernel");
    BTASSERT(fs_list(buf, NULL, &qout) == 0);
    read_until(buf,
               "fs/\r\n"
               "log/\r\n"
               "sys/\r\n"
               "thrd/\r\n");

    strcpy(buf, "tmp/foo");
    BTASSERT(fs_list(buf, NULL, &qout) == 0);
    read_until(buf,
               "bar\r\n");

    strcpy(buf, "");
    BTASSERT(fs_list(buf, NULL, &qout) == 0);
    read_until(buf,
               "kernel/\r\n"
               "logout\r\n"
               "my/\r\n"
               "our/\r\n"
               "tmp/\r\n"
               "your/\r\n");

    return (0);
}

static int test_split_merge(struct harness_t *harness_p)
{
    char buf[256];
    char *path_p;
    char * command_p;

    /* Split "/foo/fie/bar" into "/foo/fie" and "bar". */
    strcpy(buf, "/foo/fie/bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(path_p == &buf[0]);
    BTASSERT(strcmp(path_p, "/foo/fie") == 0);
    BTASSERT(command_p == &buf[9]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "/foo/fie" and "bar" to "/foo/fie/bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "/foo/fie/bar") == 0);

    /* Split "bar" into "" and "bar". */
    strcpy(buf, "bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(strcmp(path_p, "") == 0);
    BTASSERT(command_p == &buf[0]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "" and "bar" to "bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "bar") == 0);

    /* Split "/bar" into "" and "bar". */
    strcpy(buf, "/bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(strcmp(path_p, "") == 0);
    BTASSERT(command_p == &buf[1]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "" and "bar" to "/bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "/bar") == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_command, "test_command" },
        { test_counter, "test_counter" },
        { test_parameter, "test_parameter" },
        { test_list, "test_list" },
        { test_split_merge, "test_split_merge" },
        { NULL, NULL }
    };

    sys_start();

    /* Setup the commands. */
    fs_command_init(&foo_bar, FSTR("/tmp/foo/bar"), tmp_foo_bar, NULL);
    fs_command_register(&foo_bar);
    fs_command_init(&bar, FSTR("/tmp/bar"), tmp_bar, NULL);
    fs_command_register(&bar);

    /* Setup the counters. */
    fs_counter_init(&my_counter, FSTR("/my/counter"), 0);
    fs_counter_register(&my_counter);
    fs_counter_init(&your_counter, FSTR("/your/counter"), 0);
    fs_counter_register(&your_counter);

    /* Setup the parameter. */
    fs_parameter_init(&our_parameter,
                      FSTR("/our/parameter"),
                      fs_cmd_parameter_int,
                      &our_parameter_value);
    fs_parameter_register(&our_parameter);

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
