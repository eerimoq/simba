/**
 * @file main.c
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

#include "simba.h"

#define PARAMETER_FIE_DEFAULT 5

FS_COMMAND_DEFINE("/tmp/foo", tmp_foo);
FS_COMMAND_DEFINE("/tmp/bar", tmp_bar);

FS_COUNTER_DEFINE(counter1);

FS_PARAMETER_DEFINE("/tmp/fie", tmp_fie, int, PARAMETER_FIE_DEFAULT);

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

static char qoutbuf[512];
static QUEUE_INIT_DECL(qout, qoutbuf, sizeof(qoutbuf));

static int test_all(struct harness_t *harness_p)
{
    char buf[64];

    strcpy(buf, "  /tmp/foo     foo1 foo2  ");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);
    strcpy(buf, "/tmp/bar 1 2 3 4 5 6 7 8 9 10");
    BTASSERT(fs_call(buf, NULL, &qout) == -E2BIG);
    strcpy(buf, "/tmp/bar/missing");
    BTASSERT(fs_call(buf, NULL, &qout) == -ENOENT);
    strcpy(buf, "");
    BTASSERT(fs_call(buf, NULL, &qout) == -ENOENT);
    strcpy(buf, "kernel/fs/counter_reset");
    BTASSERT(fs_call(buf, NULL, &qout) == 0);

    return (0);
}

static int test_parameter(struct harness_t *harness_p)
{
    BTASSERT(FS_PARAMETER(tmp_fie) == PARAMETER_FIE_DEFAULT);
    FS_PARAMETER(tmp_fie) = 1;
    BTASSERT(FS_PARAMETER(tmp_fie) == 1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_all, "test_all" },
        { test_parameter, "test_parameter" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
