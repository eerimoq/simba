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

#if !defined(ARCH_LINUX)
static struct spi_driver_t spi;
static struct sd_driver_t sd;
#endif

static struct fat16_t fs;

#if defined(ARCH_LINUX)
static FILE *file_p = NULL;

static ssize_t linux_read_block(void *arg_p,
                                void *dst_p,
                                uint32_t src_block)
{
    size_t block_start;

    /* Find given block. */
    block_start = (SD_BLOCK_SIZE * src_block);

    if (fseek(arg_p, block_start, SEEK_SET) != 0) {
        return (1);
    }

    return (fread(dst_p, 1, SD_BLOCK_SIZE, arg_p));
}

static ssize_t linux_write_block(void *arg_p,
                                 uint32_t dst_block,
                                 const void *src_p)
{
    size_t block_start;

    /* Find given block. */
    block_start = (SD_BLOCK_SIZE * dst_block);

    if (fseek(arg_p, block_start, SEEK_SET) != 0) {
        return (1);
    }

    if (fwrite(src_p, 1, SD_BLOCK_SIZE, arg_p) != SD_BLOCK_SIZE) {
        return (-1);
    }

    fflush(file_p);

    return (SD_BLOCK_SIZE);
}
#endif

int test_init(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)
    /* Create an empty sd card file. */
    system("../../create_sdcard_linux.sh");
    file_p = fopen("sdcard", "r+b");
    BTASSERT(fat16_init(&fs,
                        linux_read_block,
                        linux_write_block,
                        file_p,
                        0) == 0);
#else
    BTASSERT(spi_init(&spi,
                      &spi_device[0],
                      &pin_d6_dev,
                      SPI_MODE_MASTER,
                      SPI_SPEED_500KBPS,
                      0,
                      0) == 0);
    BTASSERT(sd_init(&sd, &spi) == 0);
    BTASSERT(sd_start(&sd) == 0);
    BTASSERT(fat16_init(&fs,
                        (fat16_read_t)sd_read_block,
                        (fat16_write_t)sd_write_block,
                        &sd,
                        0) == 0);
#endif

    return (0);
}

static int test_format(struct harness_t *harness_p)
{
    BTASSERT(fat16_format(&fs) == 0);

    return (0);
}

static int test_mount(struct harness_t *harness_p)
{
    BTASSERT(fat16_mount(&fs) == 0);

    return (0);
}

static int test_print(struct harness_t *harness_p)
{
    /* Print various information about the file system. */
    BTASSERT(fat16_print(&fs, sys_get_stdout()) == 0);

    return (0);
}

static int test_file_operations(struct harness_t *harness_p)
{
    struct fat16_file_t foo;
    char buf[16];

    /* Create an empty file and operate on it. */
    BTASSERT(fat16_file_open(&fs, &foo, "FOO.TXT",
                             O_CREAT | O_RDWR | O_SYNC) == 0);
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

    return (0);
}

static int test_create_multiple_files(struct harness_t *harness_p)
{
    struct fat16_file_t file;
    char filename[32];
    int i;

    for (i = 0; i < 32; i++) {
        std_sprintf(filename, FSTR("FILE%d.TXT"), i);
        std_printf(FSTR("Creating file with name '%s'.\r\n"), filename);
        BTASSERT(fat16_file_open(&fs, &file, filename, O_CREAT | O_WRITE) == 0);
        BTASSERT(fat16_file_close(&file) == 0);
    }

    return (0);
}

static int test_reopen(struct harness_t *harness_p)
{
    struct fat16_file_t bar;
    char buf[16];

    /* Create an empty file and write to it. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_CREAT | O_WRITE) == 0);
    BTASSERT(fat16_file_write(&bar, "First open.\n", 12) == 12);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Re-open the file and read it's contents. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&bar, buf, 12) == 12);
    BTASSERT(memcmp(buf, "First open.\n", 12) == 0);
    BTASSERT(fat16_file_close(&bar) == 0);

    return (0);
}

static int test_directory(struct harness_t *harness_p)
{
    struct fat16_dir_t dir;
    struct fat16_file_t foo;
    struct fat16_dir_entry_t entry;
    char buf[32];
    struct fat16_stat_t stat;
    
    /* Create an empty directory called HOME and read it's contents. */
    BTASSERT(fat16_dir_open(&fs, &dir, "HOME", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_dir_close(&dir) == 0);

    /* Create a directory called ERIK in the home directory. */
    BTASSERT(fat16_dir_open(&fs,
                            &dir,
                            "HOME/ERIK",
                            O_CREAT | O_WRITE) == 0);
    BTASSERT(fat16_dir_close(&dir) == 0);

    /* Create a file in HOME/ERIK. */
    BTASSERT(fat16_file_open(&fs,
                             &foo,
                             "HOME/ERIK/BAR.TXT",
                             O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Create a file in HOME and write to it. */
    BTASSERT(fat16_file_open(&fs,
                             &foo,
                             "HOME/FOO.TXT",
                             O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&foo, "Write in subfolder.\n", 20) == 20);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Try to create a file in a directory that does not exist. */
    BTASSERT(fat16_file_open(&fs,
                             &foo,
                             "HOME2/FOO.TXT",
                             O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Re-open the directory with read option set. */
    BTASSERT(fat16_dir_open(&fs, &dir, "HOME", O_READ) == 0);

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, ".") == 0);
    BTASSERT(entry.is_dir == 1);
    BTASSERT(entry.size == 0);

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "..") == 0);
    BTASSERT(entry.is_dir == 1);
    BTASSERT(entry.size == 0);

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "ERIK") == 0);
    BTASSERT(entry.is_dir == 1);
    BTASSERT(entry.size == 3 * sizeof(struct dir_t));

    BTASSERT(fat16_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "FOO.TXT") == 0);
    BTASSERT(entry.is_dir == 0);
    BTASSERT(entry.size == 20);

    BTASSERT(fat16_dir_read(&dir, &entry) == 0);

    BTASSERT(fat16_dir_close(&dir) == 0);

    /* Read from FOO.TXT in the directory HOME. */
    BTASSERT(fat16_file_open(&fs, &foo, "HOME/FOO.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&foo, buf, 20) == 20);
    BTASSERT(memcmp(buf, "Write in subfolder.\n", 20) == 0);
    BTASSERT(fat16_file_close(&foo) == 0);

    /* Stat the HOME folder. */
    BTASSERT(fat16_stat(&fs, "HOME", &stat) == 0);
    BTASSERT(stat.is_dir == 1);
    BTASSERT(stat.size == 128);
    
    /* Stat the file HOME/FOO.TXT. */
    BTASSERT(fat16_stat(&fs, "HOME/FOO.TXT", &stat) == 0);
    BTASSERT(stat.is_dir == 0);
    BTASSERT(stat.size == 20);
    
    /* Stat non-existing file. */
    BTASSERT(fat16_stat(&fs, "HOME/FOO2.TXT", &stat) == -1);
    
    return (0);
}

static int test_bad_file(struct harness_t *harness_p)
{
    struct fat16_file_t foo;

    /* Directory APA does not exist. */
    BTASSERT(fat16_file_open(&fs,
                             &foo,
                             "APA/BAR.TXT",
                             O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Trying to open a directory as a file. */
    BTASSERT(fat16_file_open(&fs,
                             &foo,
                             "HOME",
                             O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Invalid 8.3 file name. */
    BTASSERT(fat16_file_open(&fs,
                             &foo,
                             "toolongfilename.txt",
                             O_CREAT | O_WRITE | O_SYNC) == -1);

    /* Invalid 8.3 file name. */
    BTASSERT(fat16_file_open(&fs,
                             &foo,
                             "|",
                             O_CREAT | O_WRITE | O_SYNC) == -1);

    return (0);
}

static int test_truncate(struct harness_t *harness_p)
{
    struct fat16_file_t bar;
    char buf[32];

    /* Truncate file on open. */
    BTASSERT(fat16_file_open(&fs,
                             &bar,
                             "BAR.TXT",
                             O_CREAT | O_WRITE | O_SYNC | O_TRUNC) == 0);
    BTASSERT(fat16_file_write(&bar, "1234567890", 10) == 10);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Truncate to a shorter size. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_truncate(&bar, 7) == 0);
    BTASSERT(fat16_file_size(&bar) == 7);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Truncate to a longer size. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_truncate(&bar, 10) == 0);
    BTASSERT(fat16_file_size(&bar) == 10);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Read the file. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&bar, buf, 10) == 10);
    BTASSERT(memcmp(buf, "1234567\0\0\0", 10) == 0);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Truncate the file. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_WRITE | O_TRUNC) == 0);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Read the truncated file. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&bar, buf, 17) == 0);
    BTASSERT(fat16_file_close(&bar) == 0);

    return (0);
}

static int test_append(struct harness_t *harness_p)
{
    struct fat16_file_t bar;
    char buf[32];

    /* Open a file and write 0123 to it. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_CREAT | O_WRITE | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&bar, "0123", 4) == 4);
    BTASSERT(fat16_file_close(&bar) == 0);
    BTASSERT(fat16_file_size(&bar) == 4);

    /* Overwrite 0123 with 4567. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_WRITE) == 0);
    BTASSERT(fat16_file_write(&bar, "4567", 4) == 4);
    BTASSERT(fat16_file_close(&bar) == 0);
    BTASSERT(fat16_file_size(&bar) == 4);

    /* Append 89. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_WRITE | O_APPEND | O_SYNC) == 0);
    BTASSERT(fat16_file_write(&bar, "89", 2) == 2);
    BTASSERT(fat16_file_close(&bar) == 0);
    BTASSERT(fat16_file_size(&bar) == 6);

    /* Read 456789. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_READ) == 0);
    BTASSERT(fat16_file_read(&bar, buf, 6) == 6);
    BTASSERT(memcmp(buf, "456789", 6) == 0);
    BTASSERT(fat16_file_close(&bar) == 0);

    return (0);
}

static int test_seek(struct harness_t *harness_p)
{
    struct fat16_file_t bar;
    char buf[128];
    int i;

    /* Append 2048 bytes to make the seek function get a new
       cluster. */
    BTASSERT(fat16_file_open(&fs,
                             &bar,
                             "BAR.TXT",
                             O_WRITE | O_APPEND | O_SYNC) == 0);

    for (i = 0; i < 2048; i += sizeof(buf)) {
        memset(&buf[0], (char)i, sizeof(buf));
        BTASSERT(fat16_file_write(&bar,
                                  &buf[0],
                                  sizeof(buf)) == sizeof(buf));
    }

    BTASSERT(fat16_file_close(&bar) == 0);
    BTASSERT(fat16_file_size(&bar) == 6 + 2048);
    BTASSERT(fat16_file_close(&bar) == 0);

    /* Open the file for reading. */
    BTASSERT(fat16_file_open(&fs, &bar, "BAR.TXT", O_READ) == 0);

    /* Seek one from beginning. */
    BTASSERT(fat16_file_seek(&bar, 1, FAT16_SEEK_CUR) == 0);
    BTASSERT(fat16_file_read(&bar, &buf[0], 1) == 1);
    BTASSERT(memcmp(&buf[0], "5", 1) == 0);

    /* Seek the end of the file. */
    BTASSERT(fat16_file_seek(&bar, -1, FAT16_SEEK_END) == 0);
    BTASSERT(fat16_file_tell(&bar) == fat16_file_size(&bar) - 1);

    /* Seek the beginning of the file. */
    BTASSERT(fat16_file_seek(&bar, 1, FAT16_SEEK_SET) == 0);
    BTASSERT(fat16_file_tell(&bar) == 1);

    /* Seek beyond the end of the file. */
    BTASSERT(fat16_file_seek(&bar,
                             fat16_file_size(&bar) + 1,
                             FAT16_SEEK_CUR) == -1);
    BTASSERT(fat16_file_tell(&bar) == 1);

    /* Seek before beginning of file. */
    BTASSERT(fat16_file_seek(&bar, -1, FAT16_SEEK_SET) == -1);
    BTASSERT(fat16_file_tell(&bar) == 1);

    BTASSERT(fat16_file_close(&bar) == 0);

    return (0);
}

static int test_unmount(struct harness_t *harness_p)
{
    BTASSERT(fat16_unmount(&fs) == 0);

#if !defined(ARCH_LINUX)
    BTASSERT(sd_stop(&sd) == 0);
#endif

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_format, "test_format" },
        { test_mount, "test_mount" },
        { test_print, "test_print" },
        { test_file_operations, "test_file_operations" },
        { test_create_multiple_files, "test_create_multiple_files" },
        { test_reopen, "test_reopen" },
        { test_directory, "test_directory" },
        { test_bad_file, "test_bad_file" },
        { test_truncate, "test_truncate" },
        { test_append, "test_append" },
        { test_seek, "test_seek" },
        { test_unmount, "test_unmount" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
