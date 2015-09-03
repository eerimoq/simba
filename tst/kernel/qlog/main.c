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

QLOG_DEFINE(ERR,     qlog0, "qlog0");
QLOG_DEFINE(WARNING, qlog1, "qlog1 %ld");
QLOG_DEFINE(NOTICE,  qlog2, "qlog2 %ld %ld");
QLOG_DEFINE(NOTICE,  qlog3, "qlog3 %ld %ld %ld");
QLOG_DEFINE(NOTICE,  qlog4, "qlog4 %ld %ld %ld %ld");
QLOG_DEFINE(DEBUG,   qlog5, "qlog5");

QLOG_DEFINE(NOTICE,  qlog_trigger, "qlog_trigger %ld");

int test_circular(struct harness_t *harness_p)
{
    int i;

    QLOG0(qlog0);

    /* Fill the log and write some more. */
    for (i = 0; i < QLOG_ENTRIES_MAX + 3; i++) {
        QLOG1(qlog1, i);
    }

    QLOG2(qlog2, 3, 4);

    /* Log must be turned off before formatting. */
    BTASSERT(qlog_format(sys_get_stdout()) == -1);

    /* Format entries and write to standard output. */
    BTASSERT(qlog_set_mode(QLOG_MODE_OFF) == QLOG_MODE_CIRCULAR);
    BTASSERT(qlog_format(sys_get_stdout()) == 0);

    return (0);
}

int test_trigger(struct harness_t *harness_p)
{
    int i;

    qlog_set_trigger(QLOG_ID(qlog_trigger), 0x1, 4, 0, 0 ,0);
    qlog_set_mode(QLOG_MODE_TRIGGER);

    QLOG0(qlog0);
    QLOG1(qlog_trigger, 3);
    QLOG1(qlog_trigger, 4);

    /* Fill the log and write some more. */
    for (i = 0; i < QLOG_ENTRIES_MAX + 3; i++) {
        QLOG4(qlog4, i, 2*i, 3*i, 4*i);
    }

    /* Format entries and write to standard output. */
    BTASSERT(qlog_set_mode(QLOG_MODE_OFF) == QLOG_MODE_OFF);
    BTASSERT(qlog_format(sys_get_stdout()) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_circular, "test_circular" },
        { test_trigger, "test_trigger" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();
    qlog_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
