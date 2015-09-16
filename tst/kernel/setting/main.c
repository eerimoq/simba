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

    BTASSERT(setting_read(&int8,
                           SETTINGS_INT8_ADDR,
                           SETTINGS_INT8_SIZE) == SETTINGS_INT8_SIZE);
    BTASSERT(int8 == SETTINGS_INT8_VALUE);

    BTASSERT(setting_read(&int16,
                           SETTINGS_INT16_ADDR,
                           SETTINGS_INT16_SIZE) == SETTINGS_INT16_SIZE);
    BTASSERT(int16 == SETTINGS_INT16_VALUE);

    BTASSERT(setting_read(&int32,
                           SETTINGS_INT32_ADDR,
                           SETTINGS_INT32_SIZE) == SETTINGS_INT32_SIZE);
    BTASSERT(int32 == SETTINGS_INT32_VALUE);

    int64 = 46;
    BTASSERT(setting_write(SETTINGS_INT64_ADDR,
                            &int64,
                            SETTINGS_INT64_SIZE) == SETTINGS_INT64_SIZE);

    BTASSERT(setting_read(&int64,
                           SETTINGS_INT64_ADDR,
                           SETTINGS_INT64_SIZE) == SETTINGS_INT64_SIZE);
    BTASSERT(int64 == 46);

    return (0);
}

int test_unsigned_integer(struct harness_t *harness_p)
{
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    uint64_t uint64;

    BTASSERT(setting_read(&uint8,
                           SETTINGS_UINT8_ADDR,
                           SETTINGS_UINT8_SIZE) == SETTINGS_UINT8_SIZE);
    BTASSERT(uint8 == SETTINGS_UINT8_VALUE);

    BTASSERT(setting_read(&uint16,
                           SETTINGS_UINT16_ADDR,
                           SETTINGS_UINT16_SIZE) == SETTINGS_UINT16_SIZE);
    BTASSERT(uint16 == SETTINGS_UINT16_VALUE);

    BTASSERT(setting_read(&uint32,
                           SETTINGS_UINT32_ADDR,
                           SETTINGS_UINT32_SIZE) == SETTINGS_UINT32_SIZE);
    BTASSERT(uint32 == SETTINGS_UINT32_VALUE);

    uint64 = 46;
    BTASSERT(setting_write(SETTINGS_UINT64_ADDR,
                            &uint64,
                            SETTINGS_UINT64_SIZE) == SETTINGS_UINT64_SIZE);

    BTASSERT(setting_read(&uint64,
                           SETTINGS_UINT64_ADDR,
                           SETTINGS_UINT64_SIZE) == SETTINGS_UINT64_SIZE);
    BTASSERT(uint64 == 46);

    return (0);
}

int test_string(struct harness_t *harness_p)
{
    char string[SETTINGS_STRING_SIZE];

    string[0] = 'x';
    string[1] = '\0';
    BTASSERT(setting_write(SETTINGS_STRING_ADDR,
                            string,
                            SETTINGS_STRING_SIZE) == SETTINGS_STRING_SIZE);

    string[0] = 'X';
    BTASSERT(setting_read(string,
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
        { test_unsigned_integer, "test_unsigned_integer" },
        { test_string, "test_string" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
