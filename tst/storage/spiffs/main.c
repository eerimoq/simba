/**
 * @file main.c
 * @version 4.0.0
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

#if defined(BOARD_ARDUINO_DUE)

#define PHY_SIZE               32768
#define PHY_ADDR          0x000c0000
#define PHYS_ERASE_BLOCK       0x100

#define LOG_BLOCK_SIZE           256
#define LOG_PAGE_SIZE            128

#define FILE_SIZE_MAX           8192
#define CHUNK_SIZE_MAX          1024

static struct flash_driver_t flash;

static int hal_init(void)
{
    BTASSERT(flash_module_init() == 0);
    BTASSERT(flash_init(&flash, &flash_0_dev) == 0);

    return (0);
}


static int32_t hal_read(struct spiffs_t *fs_p,
                        uint32_t addr,
                        uint32_t size,
                        uint8_t *dst_p)
{
    if (flash_read(&flash, dst_p, addr, size) != size) {
        return (-1);
    }

    return (0);
}

static int32_t hal_write(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size,
                         uint8_t *src_p)
{
    if (flash_write(&flash, addr, src_p, size) != size) {
        return (-1);
    }

    return (0);
}

static int32_t hal_erase(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size)
{
    char buf[PHYS_ERASE_BLOCK];

    if (size != PHYS_ERASE_BLOCK) {
        sys_stop(1);
    }

    memset(buf, -1, sizeof(buf));

    if (flash_write(&flash, addr, buf, size) != size) {
        return (-1);
    }

    return (0);
}

#else

#define PHY_SIZE         2048
#define PHY_ADDR            0
#define PHYS_ERASE_BLOCK  128

#define LOG_BLOCK_SIZE    128
#define LOG_PAGE_SIZE      64

#define FILE_SIZE_MAX     256
#define CHUNK_SIZE_MAX    128

static uint8_t fs_storage[PHY_SIZE];

static int hal_init(void)
{
    return (0);
}

static int32_t hal_read(struct spiffs_t *fs_p,
                        uint32_t addr,
                        uint32_t size,
                        uint8_t *dst_p)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size < sizeof(fs_storage));

    memcpy(dst_p, &fs_storage[addr], size);

    return (0);
}

static int32_t hal_write(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size,
                         uint8_t *src_p)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size < sizeof(fs_storage));

    memcpy(&fs_storage[addr], src_p, size);

    return (0);
}

static int32_t hal_erase(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size <= sizeof(fs_storage));

    memset(&fs_storage[addr], -1, size);

    return (0);
}

#endif

static struct spiffs_t fs;
static struct spiffs_config_t config;
static uint8_t workspace[2 * LOG_PAGE_SIZE];
static uint8_t fdworkspace[128];
static uint8_t cache[256];

static int test_init(struct harness_t *harness_p)
{
    int res;

    BTASSERT(hal_init() == 0);

    /* Initiate the config struct. */
    config.hal_read_f = hal_read;
    config.hal_write_f = hal_write;
    config.hal_erase_f = hal_erase;
    config.phys_size = PHY_SIZE;
    config.phys_addr = PHY_ADDR;
    config.phys_erase_block = PHYS_ERASE_BLOCK;
    config.log_block_size = LOG_BLOCK_SIZE;
    config.log_page_size = LOG_PAGE_SIZE;

    /* Mount the file system to initialize the runtime variables. */
    res = spiffs_mount(&fs,
                       &config,
                       workspace,
                       fdworkspace,
                       sizeof(fdworkspace),
                       cache,
                       sizeof(cache),
                       NULL);

    if (res != SPIFFS_ERR_NOT_A_FS) {
        spiffs_unmount(&fs);
    }

    return (0);
}

static int test_format(struct harness_t *harness_p)
{
    /* Format and mount the file system again. */
    BTASSERT(spiffs_format(&fs) == 0);
    BTASSERT(spiffs_mount(&fs,
                          &config,
                          workspace,
                          fdworkspace,
                          sizeof(fdworkspace),
                          cache,
                          sizeof(cache),
                          NULL) == 0);

    return (0);
}

static int test_read_write(struct harness_t *harness_p)
{
    char buf[12];
    spiffs_file fd;

    /* Create a file, delete previous if it already exists, and open
       it for reading and writing. */
    fd = spiffs_open(&fs,
                     "file.txt",
                     SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR,
                     0);
    BTASSERT(fd >= 0);

    /* Write to it. */
    BTASSERT(spiffs_write(&fs, fd, "Hello world", 12) == 12);

    /* Close it. */
    BTASSERT(spiffs_close(&fs, fd) == 0);

    /* Open it. */
    fd = spiffs_open(&fs, "file.txt", SPIFFS_RDWR, 0);
    BTASSERT(fd >= 0);

    /* Read it. */
    BTASSERT(spiffs_read(&fs, fd, buf, 12) == 12);
    BTASSERT(strcmp(buf, "Hello world") == 0);

    /* Close it. */
    BTASSERT(spiffs_close(&fs, fd) == 0);

    return (0);
}

static int test_read_write_performance(struct harness_t *harness_p)
{
    int res;
    spiffs_file fd;
    size_t file_size;
    size_t chunk_size;
    size_t i;
    static char buf[CHUNK_SIZE_MAX];
    struct time_t start, done, diff;
    float seconds;
    size_t written;
    uint32_t total, used;

    /* Fill the buffer. */
    for (i = 0; i < membersof(buf); i++) {
        buf[i] = i;
    }

    /* Print statistics. */
    BTASSERT(spiffs_info(&fs, &total, &used) == 0);
    std_printf(FSTR("used: %lu/%lu\r\n"), used, total);

    for (file_size = 32; file_size <= FILE_SIZE_MAX; file_size *= 4) {
        for (chunk_size = 32;
             (chunk_size <= CHUNK_SIZE_MAX) && (chunk_size <= file_size);
             chunk_size *= 4) {
            std_printf(FSTR("File size: %lu, Chunk size: %lu\r\n"),
                       file_size, chunk_size);

            /* Open a new file. */
            fd = spiffs_open(&fs,
                             "file.txt",
                             SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR,
                             0);
            BTASSERT(fd >= 0);

            /* Force the garbage collector to run. */
            spiffs_gc(&fs, 0);

            written = 0;
            time_get(&start);

            /* Write to it. */
            for (i = 0; i < file_size; i += chunk_size) {
                res = spiffs_write(&fs, fd, buf, chunk_size);
                written += chunk_size;
                BTASSERT(res == chunk_size, "res = %d\r\n", res);
            }

            time_get(&done);
            time_diff(&diff, &done, &start);
            seconds = (diff.seconds + diff.nanoseconds / 1000000000.0);
            std_printf(FSTR("  Wrote %lu bytes in %f seconds "
                            "(%d bytes/s).\r\n"),
                       written,
                       seconds,
                       (int)((float)written / seconds));

            /* Close it. */
            BTASSERT(spiffs_close(&fs, fd) == 0);
        }
    }

    /* Print statistics. */
    BTASSERT(spiffs_info(&fs, &total, &used) == 0);
    std_printf(FSTR("used: %lu/%lu\r\n"), used, total);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_format, "test_format" },
        { test_read_write, "test_read_write" },
        { test_read_write_performance, "test_read_write_performance" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
