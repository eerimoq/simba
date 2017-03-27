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

#include "simba.h"

#if defined(BOARD_ARDUINO_DUE)

#define PHY_SIZE                                    32768
#define PHY_ADDR          CONFIG_START_FILESYSTEM_ADDRESS
#define PHYS_ERASE_BLOCK                            0x100

#define LOG_BLOCK_SIZE                                256
#define LOG_PAGE_SIZE                                 128

#define FILE_SIZE_MAX                                8192
#define CHUNK_SIZE_MAX                               1024

static struct flash_driver_t flash;
static uint8_t fdworkspace[192];
static uint8_t cache[1400];

static int hal_init(void)
{
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

#elif defined(BOARD_ESP12E) || defined(BOARD_ESP01) || defined(BOARD_NANO32) || defined(BOARD_NODEMCU)

#define PHY_SIZE                                             0x10000
#define PHY_ADDR                     CONFIG_START_FILESYSTEM_ADDRESS
#define PHYS_ERASE_BLOCK                                        4096

#define LOG_BLOCK_SIZE                                          4096
#define LOG_PAGE_SIZE                                            256

#define FILE_SIZE_MAX                                           4096
#define CHUNK_SIZE_MAX                                           512

static struct flash_driver_t flash;
static uint8_t fdworkspace[192];
static uint8_t cache[1400];

static int hal_init(void)
{
    BTASSERT(flash_init(&flash, &flash_0_dev) == 0);
    BTASSERT(flash_erase(&flash, PHY_ADDR, PHY_SIZE) == 0);

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
    return (flash_erase(&flash, addr, size));
}

#else

#define PHY_SIZE        0x10000
#define PHY_ADDR              0
#define PHYS_ERASE_BLOCK   4096

#define LOG_BLOCK_SIZE     4096
#define LOG_PAGE_SIZE       256

#define FILE_SIZE_MAX       256
#define CHUNK_SIZE_MAX      128

static uint8_t fs_storage[PHY_SIZE];
static uint8_t fdworkspace[240];
static uint8_t cache[1408];

static int hal_init(void)
{
    memset(&fs_storage[0], -1, sizeof(fs_storage));
    return (0);
}

static int32_t hal_read(struct spiffs_t *fs_p,
                        uint32_t addr,
                        uint32_t size,
                        uint8_t *dst_p)
{
    BTASSERT(addr + size < sizeof(fs_storage));

    memcpy(dst_p, &fs_storage[addr], size);

    return (0);
}

static int32_t hal_write(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size,
                         uint8_t *src_p)
{
    BTASSERT(addr + size < sizeof(fs_storage));

    memcpy(&fs_storage[addr], src_p, size);

    return (0);
}

static int32_t hal_erase(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size)
{
    BTASSERT(addr + size <= sizeof(fs_storage));

    memset(&fs_storage[addr], -1, size);

    return (0);
}

#endif

static struct spiffs_t fs;
static struct spiffs_config_t config;
static uint8_t workspace[2 * LOG_PAGE_SIZE];

static int test_init(struct harness_t *harness_p)
{
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
    BTASSERT(spiffs_mount(&fs,
                          &config,
                          workspace,
                          fdworkspace,
                          sizeof(fdworkspace),
                          cache,
                          sizeof(cache),
                          NULL) != 0);

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

    std_printf(FSTR("buffer_bytes_for_filedescs = %d\r\n"),
               spiffs_buffer_bytes_for_filedescs(&fs, 5));

    std_printf(FSTR("buffer_bytes_for_cache = %d\r\n"),
               spiffs_buffer_bytes_for_cache(&fs, 5));

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
            time_subtract(&diff, &done, &start);
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
