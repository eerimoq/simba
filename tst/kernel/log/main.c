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
    int i;
    int number_of_entries;

    log_reset();

    log_set_mode(LOG_MODE_OFF);
    BTASSERT(log_format(sys_get_stdout()) == 0);

    std_printf(FSTR("formatted\r\n"));

    BTASSERT(log_set_mode(LOG_MODE_CIRCULAR) == LOG_MODE_OFF);

    LOG(NOTICE, "trace point1 %f %c %d %ld %u %lu", 1.0f, 'a', 1, -2L, 3, 4);
    LOG(NOTICE, "trace point2");
    LOG(CRIT, "trace point3");
    LOG(DEBUG, "trace point4");
    LOG(DEBUG, "trace point5");
    LOG(NOTICE, "trace point6 %f %c %d %ld %u %lu", 1.0, 'a', 1, -2L, 3, 4);
    LOG(ERR, "trace point7");

    for (i = 0; i < 10; i++) {
        LOG(WARNING, "trace point %d", i);
    }

    std_printf(FSTR("written\r\n"));

    BTASSERT(log_set_mode(LOG_MODE_OFF) == LOG_MODE_CIRCULAR);
    number_of_entries = log_format(sys_get_stdout());
#if defined(ARCH_LINUX)
    BTASSERT(number_of_entries == 4);
#elif defined(ARCH_ARM)
    BTASSERT(number_of_entries == 9);
#else
    BTASSERT(number_of_entries == 13);
#endif

    std_printf(FSTR("formatted\r\n"));

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

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
