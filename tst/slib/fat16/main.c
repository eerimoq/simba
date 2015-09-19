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

int test_print(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 0) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    BTASSERT(fat16_print(&fs, &harness_p->uart.chout) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_file_operations(struct harness_t *harness)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t foo;
    char buf[16];

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 0) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Create an empty file and operate on it. */
    BTASSERT(fat16_file_open(&fs, &foo, "FOO.TXT", O_CREAT | O_RDWR | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&foo, "foobar\n", 7) == 7);
    BTASSERT(fat16_file_seek(&foo, 0, SEEK_SET) == 0);
    BTASSERT(fat16_file_read(&foo, buf, sizeof(buf)) == 7);
    BTASSERT(memcmp(buf, "foobar\n", 7) == 0);
    BTASSERT(fat16_file_seek(&foo, 3, SEEK_SET) == 0);
    BTASSERT(fat16_file_tell(&foo) == 3);
    BTASSERT(fat16_file_read(&foo, buf, sizeof(buf)) == 4);
    BTASSERT(memcmp(buf, "bar\n", 4) == 0);
    BTASSERT(fat16_file_tell(&foo) == 7);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_create_multiple_files(struct harness_t *harness)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t file;
    char filename[32];
    int i;

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 0) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    for (i = 0; i < 32; i++) {
        std_sprintf(filename, FSTR("FILE%d.TXT"), i);
        std_printf(FSTR("Creating file with name '%s'.\r\n"), filename);
        BTASSERT(fat16_file_open(&fs, &file, filename, O_CREAT | O_WRITE) == 0);
        BTASSERT(fat16_file_close(&file) == 0);
    }

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_ls(struct harness_t *harness)
{
    struct sd_driver_t sd;
    struct fat16_t fs;

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 1) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* List all files in the file system. */
    BTASSERT(fat16_ls(&fs, &harness->uart.chout, 0) == 0);

    /* List all files in the file system. Show date and size columns. */
    BTASSERT(fat16_ls(&fs,
                      &harness->uart.chout,
                      (FAT16_LS_DATE | FAT16_LS_SIZE)) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_print, "test_print" },
        { test_file_operations, "test_file_operations" },
        { test_create_multiple_files, "test_create_multiple_files" },
        { test_ls, "test_ls" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

#if defined(ARCH_LINUX)
    /* Create an empty sd card file. */
    system("./create_sdcard_linux.sh");
#endif

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
