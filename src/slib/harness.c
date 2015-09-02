/**
 * @file harness.c
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

int harness_init(struct harness_t *harness_p)
{
    uart_init(&harness_p->uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&harness_p->uart);
    sys_set_stdout(&harness_p->uart.chout);

    return (0);
}

int harness_run(struct harness_t *harness_p,
                struct harness_testcase_t *testcases_p)
{
    int err;
    struct harness_testcase_t *testcase_p;
    int total, passed, failed;
    char buf[17];

    total = 0;
    passed = 0;
    failed = 0;
    testcase_p = testcases_p;

    while (testcase_p->callback != NULL) {
        if (testcase_p->name_p != NULL) {
            std_printk(STD_LOG_NOTICE, FSTR("enter: %s"), testcase_p->name_p);
        }

        err = testcase_p->callback(harness_p);

        if (err == 0) {
            passed++;
            std_printk(STD_LOG_NOTICE, FSTR("exit: %s: PASSED"), testcase_p->name_p);
        } else {
            failed++;
            std_printk(STD_LOG_NOTICE, FSTR("exit: %s: FAILED"), testcase_p->name_p);
        }

        total++;
        testcase_p++;
    }

    strcpy(buf, "kernel/thrd/list");
    fs_call(buf, NULL, &harness_p->uart.chout);

    std_printk(STD_LOG_NOTICE,
               FSTR("harness report: total(%d), passed(%d), failed(%d)"),
               total, passed, failed);


    return (0);
}
