/**
 * @file main.c
 * @version 2.0.0
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

#define LOG_PAGE_SIZE 64
#define PHY_SIZE 1024

uint8_t fs_storage[PHY_SIZE];

int32_t my_read(struct spiffs_t *fs_p,
                uint32_t addr,
                uint32_t size,
                uint8_t *dst_p)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size < sizeof(fs_storage));

    memcpy(dst_p, &fs_storage[addr], size);

    return (0);
}

int32_t my_write(struct spiffs_t *fs_p,
                 uint32_t addr,
                 uint32_t size,
                 uint8_t *src_p)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size < sizeof(fs_storage));

    memcpy(&fs_storage[addr], src_p, size);

    return (0);
}

int32_t my_erase(struct spiffs_t *fs_p,
                 uint32_t addr,
                 uint32_t size)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size <= sizeof(fs_storage));

    memset(&fs_storage[addr], -1, size);

    return (0);
}

int test_read_write(struct harness_t *harness_p)
{
    int res;
    char buf[12];
    spiffs_file fd;
    struct spiffs_t fs;
    struct spiffs_config_t config;
    uint8_t workspace[2 * LOG_PAGE_SIZE];
    uint8_t fdworkspace[128];
    uint8_t cache[256];

    /* Initiate the config struct. */
    config.hal_read_f = my_read;
    config.hal_write_f = my_write;
    config.hal_erase_f = my_erase;
    config.phys_size = PHY_SIZE;
    config.phys_addr = 0;
    config.phys_erase_block = 128;
    config.log_block_size = 128;
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

    /* Create a file, delete previous if it already exists, and open
       it for reading and writing. */
    fd = spiffs_open(&fs,
                     "my_file.txt",
                     SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR,
                     0);
    BTASSERT(fd >= 0);

    /* Write to it. */
    BTASSERT(spiffs_write(&fs, fd, "Hello world", 12) == 12);

    /* Close it. */
    BTASSERT(spiffs_close(&fs, fd) == 0);

    /* Open it. */
    fd = spiffs_open(&fs, "my_file.txt", SPIFFS_RDWR, 0);
    BTASSERT(fd >= 0);

    /* Read it. */
    BTASSERT(spiffs_read(&fs, fd, buf, 12) == 12);
    BTASSERT(strcmp(buf, "Hello world") == 0);

    /* Close it. */
    BTASSERT(spiffs_close(&fs, fd) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
