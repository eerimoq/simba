/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#if defined(ARCH_LINUX)

#define BLOCK_SIZE 512

static uint8_t fat16_buffer[CONFIG_START_FILESYSTEM_SIZE] = { 0 };
static struct fat16_t fat16_fs;
static struct fs_filesystem_t fat16fs;

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
 * Start the file system.
 */
static int start_filesystem(void)
{
    /* Initialize a FAT16 file system in RAM, format and mount it. */
    if (fat16_init(&fat16_fs,
                   filesystem_fat16_read_block,
                   filesystem_fat16_write_block,
                   NULL,
                   0) != 0) {
        return (-1);
    }

    if (fat16_format(&fat16_fs) != 0) {
        return (-1);
    }

    if (fat16_mount(&fat16_fs) != 0) {
        return (-1);
    }

    /* Register the FAT16 file system in the fs module. */
    if (fs_filesystem_init_fat16(&fat16fs,
                                 "/fs",
                                 &fat16_fs) != 0) {
        return (-1);
    }

    if (fs_filesystem_register(&fat16fs) != 0) {
        return (-1);
    }

    /* Set current working directory to the mount point of the file
       system. */
    thrd_set_global_env("CWD", "/fs");

    return (0);
}

#else

#if defined(ARCH_ESP) || defined(ARDUINO_DUE) || defined(ARCH_ESP32)

#    define PHYS_ERASE_BLOCK    4096
#    define LOG_BLOCK_SIZE      4096
#    define LOG_PAGE_SIZE        256

struct filesystem_t {
    struct flash_driver_t flash;
    struct {
        struct spiffs_t fs;
        struct spiffs_config_t config;
        uint8_t workspace[2 * LOG_PAGE_SIZE];
        uint8_t fdworkspace[192];
        uint8_t cache[1400];
    } spiffs;
    struct {
        struct fs_filesystem_t fs;
        struct fs_filesystem_spiffs_config_t config;
    } fs;
};

#else

#    define PHYS_ERASE_BLOCK       0x100
#    define LOG_BLOCK_SIZE           256
#    define LOG_PAGE_SIZE            128

struct filesystem_t {
    struct flash_driver_t flash;
    struct {
        struct spiffs_t fs;
        struct spiffs_config_t config;
        uint8_t workspace[2 * LOG_PAGE_SIZE];
        uint8_t fdworkspace[160];
        uint8_t cache[728];
    } spiffs;
    struct {
        struct fs_filesystem_t fs;
        struct fs_filesystem_spiffs_config_t config;
    } fs;
};

#endif

static struct filesystem_t fs;

static int32_t hal_read(struct spiffs_t *fs_p,
                        uint32_t addr,
                        uint32_t size,
                        uint8_t *dst_p)
{
    if (flash_read(&fs.flash, dst_p, addr, size) != size) {
        return (-1);
    }

    return (0);
}

static int32_t hal_write(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size,
                         uint8_t *src_p)
{
    if (flash_write(&fs.flash, addr, src_p, size) != size) {
        return (-1);
    }

    return (0);
}

static int32_t hal_erase(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size)
{
    return (flash_erase(&fs.flash, addr, size));
}

/**
 * Start the file system.
 */
static int start_filesystem(void)
{
    int res;

    if (flash_module_init() != 0) {
        return (-1);
    }

    if (flash_init(&fs.flash, &flash_0_dev) != 0) {
        return (-1);
    }

    LOG_OBJECT_PRINT(NULL,
                     LOG_INFO,
                     OSTR("Mounting the file system...\r\n"));

    /* Initiate the config struct. */
    fs.spiffs.config.hal_read_f = hal_read;
    fs.spiffs.config.hal_write_f = hal_write;
    fs.spiffs.config.hal_erase_f = hal_erase;
    fs.spiffs.config.phys_size = CONFIG_START_FILESYSTEM_SIZE;
    fs.spiffs.config.phys_addr = CONFIG_START_FILESYSTEM_ADDRESS;
    fs.spiffs.config.phys_erase_block = PHYS_ERASE_BLOCK;
    fs.spiffs.config.log_block_size = LOG_BLOCK_SIZE;
    fs.spiffs.config.log_page_size = LOG_PAGE_SIZE;

    /* Mount the file system to initialize the runtime variables. */
    res = spiffs_mount(&fs.spiffs.fs,
                       &fs.spiffs.config,
                       fs.spiffs.workspace,
                       fs.spiffs.fdworkspace,
                       sizeof(fs.spiffs.fdworkspace),
                       fs.spiffs.cache,
                       sizeof(fs.spiffs.cache),
                       NULL);

    if (res != 0) {
        LOG_OBJECT_PRINT(NULL, LOG_INFO, OSTR("failed.\r\n"));
        LOG_OBJECT_PRINT(NULL,
                         LOG_INFO,
                         OSTR("Formatting the file system...\r\n"));

        res = spiffs_format(&fs.spiffs.fs);

        if (res != 0) {
            LOG_OBJECT_PRINT(NULL, LOG_INFO, OSTR("failed.\r\n"));
            return (-1);
        }

        LOG_OBJECT_PRINT(NULL, LOG_INFO, OSTR("done.\r\n"));
        LOG_OBJECT_PRINT(NULL,
                         LOG_INFO,
                         OSTR("Mounting the file system...\r\n"));

        res = spiffs_mount(&fs.spiffs.fs,
                           &fs.spiffs.config,
                           fs.spiffs.workspace,
                           fs.spiffs.fdworkspace,
                           sizeof(fs.spiffs.fdworkspace),
                           fs.spiffs.cache,
                           sizeof(fs.spiffs.cache),
                           NULL);

        if (res != 0) {
            LOG_OBJECT_PRINT(NULL, LOG_INFO, OSTR("failed.\r\n"));
            return (-1);
        }
    }

    LOG_OBJECT_PRINT(NULL, LOG_INFO, OSTR("done.\r\n"));

    /* Register the SPIFFS file system in the fs module. */
    fs.fs.config.config_p = &fs.spiffs.config;
    fs.fs.config.workspace_p = fs.spiffs.workspace;
    fs.fs.config.fdworkspace.buf_p = fs.spiffs.fdworkspace;
    fs.fs.config.fdworkspace.size = sizeof(fs.spiffs.fdworkspace);
    fs.fs.config.cache.buf_p = fs.spiffs.cache;
    fs.fs.config.cache.size = sizeof(fs.spiffs.cache);

    fs_filesystem_init_spiffs(&fs.fs.fs,
                              "/fs",
                              &fs.spiffs.fs,
                              &fs.fs.config);

    if (fs_filesystem_register(&fs.fs.fs) != 0) {
        return (-1);
    }

    /* Set current working directory to the mount point of the file
       system. */
    thrd_set_global_env("CWD", "/fs");

    return (0);
}

#endif
