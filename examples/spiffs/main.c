/**
 * @file main.c
 * @version 5.0.0
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

#if !defined(BOARD_ARDUINO_DUE)
#    error "This example can only be built for Arduino Due."
#endif

#define PHY_SIZE               32768
#define PHY_ADDR          0x000c0000
#define PHYS_ERASE_BLOCK       0x100

#define LOG_BLOCK_SIZE           256
#define LOG_PAGE_SIZE            128

#define FILE_SIZE_MAX           8192
#define CHUNK_SIZE_MAX          1024

static struct flash_driver_t flash;

static struct spiffs_t fs;
static struct spiffs_config_t config;
static uint8_t workspace[2 * LOG_PAGE_SIZE];
static uint8_t fdworkspace[128];
static uint8_t cache[256];

struct fs_filesystem_t spiffsfs;

static struct shell_t shell;

static int hal_init(void)
{
    if (flash_module_init() != 0) {
        std_printf(FSTR("Failed to initialize the flash module.\r\n"));
        return (-1);
    }
    
    if (flash_init(&flash, &flash_0_dev) != 0) {
        std_printf(FSTR("Failed to initialize the flash driver.\r\n"));
        return (-1);
    }

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

static void init_filesystem()
{
    int res;

    if (hal_init() != 0) {
        sys_stop(1);
    }
    
    std_printf(FSTR("Trying to mount the file system.\r\n"));

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

    if (res != 0) {
        std_printf(FSTR("Failed to mount the file system. Formatting it.\r\n"));

        res = spiffs_format(&fs);

        if (res != 0) {
            std_printf(FSTR("Failed to mount the file system. Formatting it.\r\n"));
            sys_stop(1);
        }
    
        std_printf(FSTR("Trying to mount the file system after formatting it.\r\n"));
        
        res = spiffs_mount(&fs,
                           &config,
                           workspace,
                           fdworkspace,
                           sizeof(fdworkspace),
                           cache,
                           sizeof(cache),
                           NULL);
        
        if (res != 0) {
            std_printf(FSTR("Failed to mount the file system.\r\n"));
            sys_stop(1);
        }
    }

    /* Register the SPIFFS file system in the fs module. */
    fs_filesystem_init(&spiffsfs,
                       FSTR("/fs"),
                       fs_type_spiffs_t,
                       &fs);

    if (fs_filesystem_register(&spiffsfs) != 0) {
        std_printf(FSTR("Failed to register the file system into the debug file system.\r\n"));
        sys_stop(1);
    }
}

static int write_story(void)
{
    struct fs_file_t file;

    std_printf(FSTR("Writing 'The big bad wolf.' to 'fs/story.txt'.\r\n"));
        
    /* Create a file, delete previous if it already exists, and open
       it for reading and writing. */
    if (fs_open(&file, "fs/story.txt", FS_CREAT | FS_TRUNC | FS_RDWR) != 0) {
        std_printf(FSTR("Failed to open file.\r\n"));
        return (-1);
    }

    /* Write to it. */
    if (fs_write(&file, "The big bad wolf.", 16) != 16) {
        std_printf(FSTR("Failed to write to the file.\r\n"));
        return (-1);
    }

    /* Close it. */
    if (fs_close(&file) != 0) {
        std_printf(FSTR("Failed to close the file.\r\n"));
        return (-1);
    }
    
    return (0);
}

static int increment_counter(void)
{
    char buf[32];
    struct fs_file_t file;
    long counter;
    size_t size;
    
    std_printf(FSTR("Incrementing the counter in 'fs/counter.txt'.\r\n"));
        
    if (fs_open(&file, "fs/counter.txt", FS_RDWR) != 0) {
        if (fs_open(&file,
                    "fs/counter.txt",
                    FS_CREAT | FS_TRUNC | FS_RDWR) != 0) {
            std_printf(FSTR("Failed to open file.\r\n"));
            return (-1);
        }

        /* Initialize the file by writing 0 to it. */
        if (fs_write(&file, "0\0", 2) != 2) {
            std_printf(FSTR("Failed to write to the file.\r\n"));
            return (-1);
        }

        /* Rewind the file position. */
        if (fs_seek(&file, 0, FS_SEEK_SET) != 0) {
            std_printf(FSTR("Failed to seek to beginning of the file.\r\n"));
            return (-1);
        }
    }

    /* Read the value from it. */
    if (fs_read(&file, buf, 16) <= 0) {
        std_printf(FSTR("Failed to write to the file.\r\n"));
        return (-1);
    }

    if (std_strtol(buf, &counter) == NULL) {
        std_printf(FSTR("Failed to parse the counter value.\r\n"));
        return (-1);
    }

    counter++;
    std_sprintf(buf, FSTR("%lu"), counter);
    size = strlen(buf) + 1;

    /* Rewind the file position. */
    if (fs_seek(&file, 0, FS_SEEK_SET) != 0) {
        std_printf(FSTR("Failed to seek to beginning of the file.\r\n"));
        return (-1);
    }

    /* Write the incremented value. */
    if (fs_write(&file, buf, size) != size) {
        std_printf(FSTR("Failed to write to the file.\r\n"));
        return (-1);
    }

    /* Close it. */
    if (fs_close(&file) != 0) {
        std_printf(FSTR("Failed to close the file.\r\n"));
        return (-1);
    }

    std_printf(FSTR("Counter incremented to %lu\r\n"), counter);
    
    return (0);
}

int main()
{
    /* Start the system. */
    sys_start();

    /* Print the system information. */
    std_printf(sys_get_info());

    /* Try to mount the file system. Format it on failure and
       remount. */
    init_filesystem();

    /* Perform a few file operations. */
    if (write_story() != 0) {
        sys_stop(1);
    }

    /* Increment a counter in a file. */
    if (increment_counter() != 0) {
        sys_stop(1);
    }

    /* Call the shell main function. */
    shell_init(&shell,
               sys_get_stdin(),
               sys_get_stdout(),
               NULL,
               NULL,
               NULL,
               NULL);
    shell_main(&shell);

    return (0);
}
