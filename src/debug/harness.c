/**
 * @file harness.c
 * @version 2.0.0
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

int harness_init(struct harness_t *self_p)
{
    uart_init(&self_p->uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&self_p->uart);
    sys_set_stdout(&self_p->uart.chout);
    log_set_default_handler_output_channel(sys_get_stdout());

    return (0);
}

int harness_run(struct harness_t *self_p,
                struct harness_testcase_t *testcases_p)
{
    int err;
    struct harness_testcase_t *testcase_p;
    int total, passed, failed, skipped;
    char buf[17];

    total = 0;
    passed = 0;
    failed = 0;
    skipped = 0;
    testcase_p = testcases_p;

    /* Print a header. */
    std_printf(sys_get_info());
    std_printf(sys_get_config());

    while (testcase_p->callback != NULL) {
        if (testcase_p->name_p != NULL) {
            std_printf(FSTR("enter: %s\r\n"), testcase_p->name_p);
        }

        err = testcase_p->callback(self_p);

        if (err == 0) {
            passed++;
            std_printf(FSTR("exit: %s: PASSED\r\n\r\n"),
                       testcase_p->name_p);
        } else if (err < 0) {
            failed++;
            std_printf(FSTR("exit: %s: FAILED\r\n\r\n"),
                       testcase_p->name_p);
        } else {
            skipped++;
            std_printf(FSTR("exit: %s: SKIPPED\r\n\r\n"),
                       testcase_p->name_p);
        }

        total++;
        testcase_p++;
    }

    strcpy(buf, "kernel/thrd/list");
    fs_call(buf, NULL, &self_p->uart.chout, NULL);

    std_printf(FSTR("harness report: total(%d), passed(%d), "
                    "failed(%d), skipped(%d)\r\n"),
               total, passed, failed, skipped);

    sys_stop(failed);

    return (0);
}
