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

int test_integer(struct harness_t *harness_p)
{
    int8_t int8;
    int16_t int16;
    int32_t int32;
    int64_t int64;

    BTASSERT(settings_read(&int8,
                           SETTINGS_INT8_ADDR,
                           SETTINGS_INT8_SIZE) == SETTINGS_INT8_SIZE);
    BTASSERT(int8 == 8);

    BTASSERT(settings_read(&int16,
                           SETTINGS_INT16_ADDR,
                           SETTINGS_INT16_SIZE) == SETTINGS_INT16_SIZE);
    BTASSERT(int16 == 16);

    BTASSERT(settings_read(&int32,
                           SETTINGS_INT32_ADDR,
                           SETTINGS_INT32_SIZE) == SETTINGS_INT32_SIZE);
    BTASSERT(int32 == 32);

    BTASSERT(settings_read(&int64,
                           SETTINGS_INT64_ADDR,
                           SETTINGS_INT64_SIZE) == SETTINGS_INT64_SIZE);
    BTASSERT(int64 == 64);

    int64 = 46;
    BTASSERT(settings_write(SETTINGS_INT64_ADDR,
                            &int64,
                            SETTINGS_INT64_SIZE) == SETTINGS_INT64_SIZE);

    BTASSERT(settings_read(&int64,
                           SETTINGS_INT64_ADDR,
                           SETTINGS_INT64_SIZE) == SETTINGS_INT64_SIZE);
    BTASSERT(int64 == 46);

    return (0);
}

int test_string(struct harness_t *harness_p)
{
    char string[SETTINGS_STRING_SIZE];

    BTASSERT(settings_read(string,
                           SETTINGS_STRING_ADDR,
                           SETTINGS_STRING_SIZE) == SETTINGS_STRING_SIZE);

    BTASSERT(strcmp(string, "y") == 0);

    string[0] = 'x';
    BTASSERT(settings_write(SETTINGS_STRING_ADDR,
                            string,
                            SETTINGS_STRING_SIZE) == SETTINGS_STRING_SIZE);

    BTASSERT(settings_read(string,
                           SETTINGS_STRING_ADDR,
                           SETTINGS_STRING_SIZE) == SETTINGS_STRING_SIZE);

    BTASSERT(strcmp(string, "x") == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_integer, "test_integer" },
        { test_string, "test_string" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
