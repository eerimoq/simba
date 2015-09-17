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

int test_file_operations(struct harness_t *harness)
{
    struct sd_t sd;
    struct fat16_t fs;
    struct fat16_file_t foo;
    char buf[16];

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Create an empty file and operate on it. */
    BTASSERT(fat16_file_open(&fs, &foo, "FOO.TXT", O_RDWR) == 0);
    BTASSERT(fat16_file_write(&foo, "foobar", 6) == 6);
    BTASSERT(fat16_file_read(&foo, buf, sizeof(buf)) == 6);
    BTASSERT(memcmp(buf, "foobar", 6) == 0);
    BTASSERT(fat16_file_seek(&foo, 3, SEEK_SET) == 0);
    BTASSERT(fat16_file_tell(&foo) == 3);
    BTASSERT(fat16_file_read(&foo, buf, sizeof(buf)) == 3);
    BTASSERT(memcmp(buf, "bar", 3) == 0);
    BTASSERT(fat16_file_tell(&foo) == 6);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_ls(struct harness_t *harness)
{
    struct sd_t sd;
    struct fat16_t fs;

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* List all files in the file system. */
    BTASSERT(fat16_ls(&uart, &fs) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_file_operations, "test_file_operations" },
        { test_ls, "test_ls" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
