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

int test_circular(struct harness_t *harness_p)
{
    log_reset();
    log_set_mode(LOG_MODE_CIRCULAR);

    LOG(NOTICE, "trace point1 %f %c %d %ld %u %lu", 1.0, 'a', 1, -2L, 3, 4);
    LOG(NOTICE, "trace point2");
    LOG(CRIT, "trace point3");
    LOG(DEBUG, "trace point4");
    LOG(DEBUG, "trace point5");
    LOG(NOTICE, "trace point6 %f %c %d %ld %u %lu", 1.0, 'a', 1, -2L, 3, 4);
    LOG(ERR, "trace point7");
    LOG(WARNING, "trace point8");

    BTASSERT(log_set_mode(LOG_MODE_OFF) == LOG_MODE_CIRCULAR);
    BTASSERT(log_format(sys_get_stdout()) == 4);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_circular, "test_circular" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();
    log_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
