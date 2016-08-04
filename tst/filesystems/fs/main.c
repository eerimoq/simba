/**
 * @file main.c
 * @version 4.1.0
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

#define OUR_PARAMETER_DEFAULT 5
#define BUFFER_SIZE 512

static struct fs_command_t foo_bar;
static struct fs_command_t bar;

static struct fs_counter_t my_counter;
static struct fs_counter_t your_counter;

static int our_parameter_value = OUR_PARAMETER_DEFAULT;
static struct fs_parameter_t our_parameter;

#if defined(ARCH_LINUX)

#define BLOCK_SIZE 512

static uint8_t fat16_buffer[65536] = { 0 };

/* SPIFFS configuration. */

#define PHY_SIZE         2048
#define PHY_ADDR            0
#define PHYS_ERASE_BLOCK  128

#define LOG_BLOCK_SIZE    128
#define LOG_PAGE_SIZE      64

#define FILE_SIZE_MAX     256
#define CHUNK_SIZE_MAX    128

static uint8_t spiffs_buffer[PHY_SIZE];

struct fat16_t fat16_fs;
struct fs_filesystem_t fat16fs;

struct spiffs_t spiffs_fs;
struct fs_filesystem_t spiffsfs;
struct spiffs_config_t config;
static uint8_t workspace[2 * LOG_PAGE_SIZE];
static uint8_t fdworkspace[128];
static uint8_t cache[256];

/**
 * FAT16 read block callback.
 */
static ssize_t filesystem_fat16_read_block(void *arg_p,
                                            void *dst_p,
                                            uint32_t src_block)
{
    ASSERT(src_block < sizeof(fat16_buffer) / 512);

    memcpy(dst_p, &fat16_buffer[BLOCK_SIZE * src_block], BLOCK_SIZE);

    return (BLOCK_SIZE);
}

/**
 * FAT16 write block callback.
 */
static ssize_t filesystem_fat16_write_block(void *arg_p,
                                             uint32_t dst_block,
                                             const void *src_p)
{
    ASSERT(dst_block < sizeof(fat16_buffer) / 512);

    memcpy(&fat16_buffer[BLOCK_SIZE * dst_block], src_p, BLOCK_SIZE);

    return (BLOCK_SIZE);
}

/**
 * SPIFFS read callback.
 */
static int32_t filesystem_spiffs_read(struct spiffs_t *fs_p,
                                       uint32_t addr,
                                       uint32_t size,
                                       uint8_t *dst_p)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size < sizeof(spiffs_buffer));
    
    memcpy(dst_p, &spiffs_buffer[addr], size);
    
    return (0);
}

/**
 * SPIFFS write callback.
 */
static int32_t filesystem_spiffs_write(struct spiffs_t *fs_p,
                                        uint32_t addr,
                                        uint32_t size,
                                        uint8_t *src_p)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size < sizeof(spiffs_buffer));

    memcpy(&spiffs_buffer[addr], src_p, size);

    return (0);
}

/**
 * SPIFFS erase callback.
 */
static int32_t filesystem_spiffs_erase(struct spiffs_t *fs_p,
                                        uint32_t addr,
                                        uint32_t size)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size <= sizeof(spiffs_buffer));

    memset(&spiffs_buffer[addr], -1, size);

    return (0);
}

#endif

static int tmp_foo_bar(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    UNUSED(out_p);
    UNUSED(in_p);

    BTASSERT(argc == 3);
    std_fprintf(out_p,
                FSTR("argc = %d, argv[0] = %s, argv[1] = %s, argv[2] = %s\n"),
                argc,
                argv[0],
                argv[1],
                argv[2]);

    return (0);
}

static int tmp_bar(int argc,
                   const char *argv[],
                   chan_t *out_p,
                   chan_t *in_p,
                   void *arg_p,
                   void *call_arg_p)
{
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(out_p);
    UNUSED(in_p);

    return (0);
}

static char qoutbuf[BUFFER_SIZE];
static QUEUE_INIT_DECL(qout, qoutbuf, sizeof(qoutbuf));

static int read_until(char *buf_p, const char *pattern)
{
    char c;
    size_t length = 0;
    size_t pattern_length = strlen(pattern);

    while (length < BUFFER_SIZE - 1) {
        chan_read(&qout, &c, sizeof(c));
#if defined(ARCH_LINUX)
        std_printf(FSTR("%c"), c);
#endif
        *buf_p++ = c;
        length++;
        *buf_p = '\0';

        /* Compare to pattern. */
        if (length >= pattern_length) {
            if (!strcmp(&buf_p[-pattern_length], pattern)) {
                return (1);
            }
        }
    }

    return (0);
}

static int test_command(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    strcpy(buf, "  /tmp/foo/bar     foo1 foo2  ");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "\n");

    strcpy(buf, "/tmp/bar 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -E2BIG);

    strcpy(buf, "/tmp/bar/missing");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -ENOENT);
    read_until(buf, "\n");

    strcpy(buf, "");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -ENOENT);
    read_until(buf, "\n");

    strcpy(buf, "/tmp/q");
    BTASSERT(fs_auto_complete(buf) == -ENOENT);

    strcpy(buf, "/tm");
    BTASSERT(fs_auto_complete(buf) == 2);
    BTASSERT(strcmp(buf, "/tmp/") == 0);

    strcpy(buf, "/tmp/f");
    BTASSERT(fs_auto_complete(buf) == 3);
    BTASSERT(strcmp(buf, "/tmp/foo/") == 0);

    strcpy(buf, "/tmp/foo/");
    BTASSERT(fs_auto_complete(buf) == 4);
    BTASSERT(strcmp(buf, "/tmp/foo/bar ") == 0);

    strcpy(buf, "");
    BTASSERT(fs_auto_complete(buf) == 0);
    BTASSERT(strcmp(buf, "") == 0);

    return (0);
}

static int test_counter(struct harness_t *harness_p)
{
    char buf[384];

    strcpy(buf, "filesystems/fs/counters/list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "/your/counter                                        0000000000000000\r\n");

    fs_counter_increment(&my_counter, 3);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "0000000000000003\r\n");

    strcpy(buf, "filesystems/fs/counters/reset");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "0000000000000000\r\n");

    return (0);
}

static int test_parameter(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "filesystems/fs/parameters/list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf,
               "NAME                                                 VALUE\r\n"
               "/our/parameter                                       5\r\n");

    BTASSERT(our_parameter_value == OUR_PARAMETER_DEFAULT);
    our_parameter_value = 1;
    BTASSERT(our_parameter_value == 1);

    strcpy(buf, "/our/parameter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "1\r\n");

    strcpy(buf, "/our/parameter 3");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "/our/parameter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf, "3\r\n");

    return (0);
}

static int test_list(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "kernel");
    BTASSERT(fs_list(buf, NULL, &qout) == 0);
    read_until(buf,
               "log/\r\n"
               "sys/\r\n"
               "thrd/\r\n");

    strcpy(buf, "tmp/foo");
    BTASSERT(fs_list(buf, NULL, &qout) == 0);
    read_until(buf,
               "bar\r\n");

    strcpy(buf, "");
    BTASSERT(fs_list(buf, NULL, &qout) == 0);
    read_until(buf,
               "kernel/\r\n"
               "logout\r\n"
               "my/\r\n"
               "oam/\r\n"
               "our/\r\n"
               "tmp/\r\n"
               "your/\r\n");

    return (0);
}

static int test_split_merge(struct harness_t *harness_p)
{
    char buf[256];
    char *path_p;
    char * command_p;

    /* Split "/foo/fie/bar" into "/foo/fie" and "bar". */
    strcpy(buf, "/foo/fie/bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(path_p == &buf[0]);
    BTASSERT(strcmp(path_p, "/foo/fie") == 0);
    BTASSERT(command_p == &buf[9]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "/foo/fie" and "bar" to "/foo/fie/bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "/foo/fie/bar") == 0);

    /* Split "bar" into "" and "bar". */
    strcpy(buf, "bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(strcmp(path_p, "") == 0);
    BTASSERT(command_p == &buf[0]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "" and "bar" to "bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "bar") == 0);

    /* Split "/bar" into "" and "bar". */
    strcpy(buf, "/bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(strcmp(path_p, "") == 0);
    BTASSERT(command_p == &buf[1]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "" and "bar" to "/bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "/bar") == 0);

    return (0);
}

static int test_filesystem_fat16(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    char buf[32];
    struct fs_file_t file;

    /* Initialize a FAT16 file system in RAM, format and mount it. */
    BTASSERT(fat16_init(&fat16_fs,
                        filesystem_fat16_read_block,
                        filesystem_fat16_write_block,
                        NULL,
                        0) == 0);
    BTASSERT(fat16_format(&fat16_fs) == 0);
    BTASSERT(fat16_mount(&fat16_fs) == 0);

    /* Register the FAT16 file system in the fs module. */
    BTASSERT(fs_filesystem_init(&fat16fs,
                                 FSTR("/fat16fs"),
                                 fs_type_fat16_t,
                                 &fat16_fs) == 0);
    BTASSERT(fs_filesystem_register(&fat16fs) == 0);

    /* Perform file operations. */
    BTASSERT(fs_open(&file, "/fat16fs/foo.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_write(&file, "hello!", 6) == 6);
    BTASSERT(fs_seek(&file, 0, FS_SEEK_SET) == 0);
    BTASSERT(fs_read(&file, buf, 6) == 6);
    BTASSERT(memcmp(buf, "hello!", 6) == 0);
    BTASSERT(fs_tell(&file) == 6);
    BTASSERT(fs_close(&file) == 0);

    /* Try to open a file outside the file system. */
    BTASSERT(fs_open(&file, "/foo.txt", FS_CREAT | FS_RDWR | FS_SYNC) == -1);

    return (0);

#else

    return (1);

#endif
}

static int test_filesystem_spiffs(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    char buf[32];
    struct fs_file_t file;

    /* Initiate the config struct. */
    config.hal_read_f = filesystem_spiffs_read;
    config.hal_write_f = filesystem_spiffs_write;
    config.hal_erase_f = filesystem_spiffs_erase;
    config.phys_size = PHY_SIZE;
    config.phys_addr = PHY_ADDR;
    config.phys_erase_block = PHYS_ERASE_BLOCK;
    config.log_block_size = LOG_BLOCK_SIZE;
    config.log_page_size = LOG_PAGE_SIZE;

    /* Mount the file system to initialize the runtime variables. */
    BTASSERT(spiffs_mount(&spiffs_fs,
                          &config,
                          workspace,
                          fdworkspace,
                          sizeof(fdworkspace),
                          cache,
                          sizeof(cache),
                          NULL) == 0);
    spiffs_unmount(&spiffs_fs);

    /* Format and mount the file system again. */
    BTASSERT(spiffs_format(&spiffs_fs) == 0);
    BTASSERT(spiffs_mount(&spiffs_fs,
                          &config,
                          workspace,
                          fdworkspace,
                          sizeof(fdworkspace),
                          cache,
                          sizeof(cache),
                          NULL) == 0);

    /* Register the SPIFFS file system in the fs module. */
    BTASSERT(fs_filesystem_init(&spiffsfs,
                                 FSTR("/spiffsfs"),
                                 fs_type_spiffs_t,
                                 &spiffs_fs) == 0);
    BTASSERT(fs_filesystem_register(&spiffsfs) == 0);

    /* Perform file operations. */
    BTASSERT(fs_open(&file, "/spiffsfs/foo.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_write(&file, "hello!", 6) == 6);
    BTASSERT(fs_seek(&file, 0, FS_SEEK_SET) == 0);
    BTASSERT(fs_read(&file, buf, 6) == 6);
    BTASSERT(memcmp(buf, "hello!", 6) == 0);
    BTASSERT(fs_tell(&file) == 6);
    BTASSERT(fs_close(&file) == 0);

    return (0);

#else

    return (1);

#endif
}

static int test_filesystem_list(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    char buf[256];

    strcpy(buf, "/filesystems/fs/filesystems/list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    read_until(buf,
               "MOUNT-POINT                    MEDIUM   TYPE     AVAILABLE  SIZE  USAGE\r\n"
               "/spiffsfs                      -        spiffs           -     -     -%\r\n"
               "/fat16fs                       -        fat16            -     -     -%\r\n");

    return (0);

#else

    return (1);

#endif
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_command, "test_command" },
        { test_counter, "test_counter" },
        { test_parameter, "test_parameter" },
        { test_list, "test_list" },
        { test_split_merge, "test_split_merge" },
        { test_filesystem_fat16, "test_filesystem_fat16" },
        { test_filesystem_spiffs, "test_filesystem_spiffs" },
        { test_filesystem_list, "test_filesystem_list" },
        { NULL, NULL }
    };

    sys_start();

    /* Setup the commands. */
    fs_command_init(&foo_bar, FSTR("/tmp/foo/bar"), tmp_foo_bar, NULL);
    fs_command_register(&foo_bar);
    fs_command_init(&bar, FSTR("/tmp/bar"), tmp_bar, NULL);
    fs_command_register(&bar);

    /* Setup the counters. */
    fs_counter_init(&my_counter, FSTR("/my/counter"), 0);
    fs_counter_register(&my_counter);
    fs_counter_init(&your_counter, FSTR("/your/counter"), 0);
    fs_counter_register(&your_counter);

    /* Setup the parameter. */
    fs_parameter_init(&our_parameter,
                      FSTR("/our/parameter"),
                      fs_parameter_int_set,
                      fs_parameter_int_print,
                      &our_parameter_value);
    fs_parameter_register(&our_parameter);

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
