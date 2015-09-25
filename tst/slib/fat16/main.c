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
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 0) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

    /* Print various information about the file system. */
    BTASSERT(fat16_print(&fs, &harness_p->uart.chout) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_file_operations(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t foo;
    char buf[16];

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 0) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

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

int test_create_multiple_files(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t file;
    char filename[32];
    int i;

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 0) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

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

int test_reopen(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t bar;
    char buf[16];

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 0) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

    /* Create an empty file and write to it. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&bar, "First open.\n", 12) == 12);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Re-open the file and read it's contents. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&bar, buf, 12) == 12);
    BTASSERT(memcmp(buf, "First open.\n", 12) == 0);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_directory(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_dir_t dir;
    struct fat16_file_t foo;
    struct fat16_dir_entry_t entry;
    char buf[32];

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 1) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

    /* Create an empty directory called HOME and read it's contents. */
    BTASSERT(fat16_dir_open(&fs, &dir, "HOME", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_dir_close(&dir) == 0);

    /* Create a directory called ERIK in the home directory. */
    BTASSERT(fat16_dir_open(&fs, &dir, "HOME/ERIK", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_dir_close(&dir) == 0);

    /* Create a file in HOME/ERIK. */
    BTASSERT(fat16_file_open(&fs, &foo, "HOME/ERIK/BAR.TXT", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Create a file in HOME and write to it. */
    BTASSERT(fat16_file_open(&fs, &foo, "HOME/FOO.TXT", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&foo, "Write in subfolder.\n", 20) == 20);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Try to create a file in a directory that does not exist. */
    BTASSERT(fat16_file_open(&fs, &foo, "HOME2/FOO.TXT", O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Re-open the directory with read option set. */
    BTASSERT(fat16_dir_open(&fs, &dir, "HOME", O_READ) == 0);

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, ".          ") == 0);
    BTASSERT(entry.attributes == DIR_ATTR_DIRECTORY);
    BTASSERT(entry.size == 0);

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "..         ") == 0);
    BTASSERT(entry.attributes == DIR_ATTR_DIRECTORY);
    BTASSERT(entry.size == 0);

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "ERIK       ") == 0);
    BTASSERT(entry.attributes == DIR_ATTR_DIRECTORY);
    BTASSERT(entry.size == 3 * sizeof(struct dir_t));

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "FOO     TXT") == 0);
    BTASSERT(entry.attributes == 0);
    BTASSERT(entry.size == 20);

    BTASSERT(fat16_dir_read(&dir, &entry) == 0);

    BTASSERT(fat16_dir_close(&dir) == 0);

    /* Read from FOO.TXT in the directory HOME. */
    BTASSERT(fat16_file_open(&fs, &foo, "HOME/FOO.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&foo, buf, 20) == 20);
    BTASSERT(memcmp(buf, "Write in subfolder.\n", 20) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_bad_file(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t foo;

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 1) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

    /* Directory APA does not exist. */
    BTASSERT(fat16_file_open(&fs, &foo, "APA/BAR.TXT", O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Invalid 8.3 file name. */
    BTASSERT(fat16_file_open(&fs, &foo, "toolongfilename.txt", O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Invalid 8.3 file name. */
    BTASSERT(fat16_file_open(&fs, &foo, "|", O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_truncate(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t foo;
    char buf[32];

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 1) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

    /* Truncate file on open. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_CREAT | O_WRITE | O_SYNC | O_TRUNC) == 0);
    BTASSERT(fat16_file_write(&foo, "To be truncated.\n", 17) == 17);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Read the file. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&foo, buf, 17) == 17);
    BTASSERT(memcmp(buf, "To be truncated.\n", 17) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Truncate the file. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_WRITE | O_TRUNC) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Read the truncated file. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&foo, buf, 17) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Stop the file system. */
    BTASSERT(fat16_stop(&fs) == 0);

    return (0);
}

int test_append(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    struct fat16_t fs;
    struct fat16_file_t foo;
    char buf[32];

    /* Initialize and start the SD card. */
    BTASSERT(sd_init(&sd, NULL) == 0);

    /* Initialize and start the file system. */
    BTASSERT(fat16_init(&fs, &sd, 1) == 0);
    BTASSERT(fat16_start(&fs) == 0);

    /* Format the file system. */
    BTASSERT(fat16_format(&fs) == 0);

    /* Open a file and write 0123 to it. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&foo, "0123", 4) == 4);
    BTASSERT(fat16_file_close(&foo) == 0);
    BTASSERT(fat16_file_size(&foo) == 4);

    /* Overwrite 0123 with 4567. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_WRITE) == 0);
    BTASSERT(fat16_file_write(&foo, "4567", 4) == 4);
    BTASSERT(fat16_file_close(&foo) == 0);
    BTASSERT(fat16_file_size(&foo) == 4);

    /* Append 89. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_WRITE | O_APPEND | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&foo, "89", 2) == 2);
    BTASSERT(fat16_file_close(&foo) == 0);
    BTASSERT(fat16_file_size(&foo) == 6);

    /* Read 456789. */
    BTASSERT(fat16_file_open(&fs, &foo, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&foo, buf, 6) == 6);
    BTASSERT(memcmp(buf, "456789", 6) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

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
        { test_reopen, "test_reopen" },
        { test_directory, "test_directory" },
        { test_bad_file, "test_bad_file" },
        { test_truncate, "test_truncate" },
        { test_append, "test_append" },
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
