/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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
#include <limits.h>

static struct fs_command_t cmd_ls;
static struct fs_command_t cmd_open;
static struct fs_command_t cmd_close;
static struct fs_command_t cmd_read;
static struct fs_command_t cmd_write;

static struct shell_t shell;

static struct spi_driver_t spi;
static struct sd_driver_t sd;
static struct fat16_t fs;

static int is_file_open = 0;
static struct fat16_file_t file;

static int print_dir_name(void *chan_p,
                          const struct fat16_dir_entry_t *entry_p,
                          int width)
{
    size_t size;
    char b;

    size = strlen(entry_p->name);
    chan_write(chan_p, entry_p->name, size);

    if (entry_p->is_dir == 1) {
        b = '/';
        chan_write(chan_p, &b, sizeof(b));
        size++;
    }

    while (size < width) {
        b = ' ';
        chan_write(chan_p, &b, sizeof(b));
        size++;
    }

    return (0);
}

static int format_entry(void *chan_p,
                        const struct fat16_dir_entry_t *entry_p)
{

    /* Print file name with possible blank fill */
    print_dir_name(chan_p, entry_p, 14);

    /* Print modify date/time if requested */
    std_fprintf(chan_p,
                OSTR("%04u-%02u-%02u %02u:%02u:%02u"),
                entry_p->latest_mod_date.year,
                entry_p->latest_mod_date.month,
                entry_p->latest_mod_date.day,
                entry_p->latest_mod_date.hour,
                entry_p->latest_mod_date.minute,
                entry_p->latest_mod_date.second);

    /* Print size if requested */
    if (entry_p->is_dir != 1) {
        std_fprintf(chan_p, OSTR(" %lu"), (unsigned long)entry_p->size);
    }

    std_fprintf(chan_p, OSTR("\r\n"));

    return (0);
}

static int cmd_ls_cb(int argc,
                     const char *argv[],
                     void *out_p,
                     void *in_p,
                     void *arg_p,
                     void *call_arg_p)
{
    UNUSED(in_p);

    struct fat16_dir_t dir;
    struct fat16_dir_entry_t entry;
    const char *path_p;

    if (argc != 2) {
        std_fprintf(out_p, OSTR("Usage: %s <path>\r\n"), argv[0]);
        return (1);
    }

    path_p = argv[1];

    /* Re-open the directory with read option set. */
    if (fat16_dir_open(&fs, &dir, path_p, O_READ) != 0) {
        std_fprintf(out_p, OSTR("%s: directory not found\r\n"), path_p);
        return (-1);
    }

    while (fat16_dir_read(&dir, &entry) == 1) {
        format_entry(out_p, &entry);
    }

    fat16_dir_close(&dir);

    return (0);
}

static int cmd_open_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    UNUSED(in_p);

    const char *path_p;
    int flags;

    if (argc != 3) {
        std_fprintf(out_p, OSTR("Usage: %s <path> <r | w>\r\n"), argv[0]);
        return (1);
    }

    if (is_file_open == 1) {
        std_fprintf(out_p,
                    OSTR("Cannot have more than one file open. Please close "
                         "current file before opening a new one.\r\n"));
        return (1);
    }

    path_p = argv[1];

    if (argv[2][0] == 'w') {
        flags = (O_CREAT | O_WRITE | O_SYNC);
    } else {
        flags = O_READ;
    }

    /* Re-open the directory with read option set. */
    if (fat16_file_open(&fs, &file, path_p, flags) != 0) {
        std_fprintf(out_p, OSTR("%s: file not found\r\n"), path_p);
        return (1);
    }

    is_file_open = 1;

    return (0);
}

static int cmd_close_cb(int argc,
                        const char *argv[],
                        void *out_p,
                        void *in_p,
                        void *arg_p,
                        void *call_arg_p)
{
    UNUSED(in_p);

    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: %s\r\n"), argv[0]);
        return (1);
    }

    if (is_file_open == 0) {
        std_fprintf(out_p, OSTR("No file is open.\r\n"));
        return (1);
    }

    fat16_file_close(&file);

    is_file_open = 0;

    return (0);
}

static int cmd_read_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    UNUSED(in_p);

    ssize_t read, n;
    long size;
    char buf[64];

    if (argc == 1) {
        size = INT_MAX;
    } else if (argc == 2) {
        if (std_strtol(argv[1], &size) == NULL) {
            std_fprintf(out_p, OSTR("%s: expected posotove integer\r\n"), argv[1]);
            return (1);
        }
    } else {
        std_fprintf(out_p, OSTR("Usage: %s [<size to read>]\r\n"), argv[0]);

        return (1);
    }

    /* A file must be open. */
    if (is_file_open == 0) {
        std_fprintf(out_p, OSTR("The file must be opened before it can be read from.\r\n"));

        return (1);
    }

    /* Read from file. */
    read = 0;

    while (read < size) {
        if ((size - read) < sizeof(buf)) {
            n = (size - read);
        } else {
            n = sizeof(buf);
        }

        if ((n = fat16_file_read(&file, buf, n)) <= 0) {
            break;
        }

        chan_write(out_p, buf, n);
        read += n;
    }

    return (0);
}

static int cmd_write_cb(int argc,
                        const char *argv[],
                        void *out_p,
                        void *in_p,
                        void *arg_p,
                        void *call_arg_p)
{
    UNUSED(in_p);

    if (argc != 2) {
        std_fprintf(out_p, OSTR("Usage: %s <data to write>\r\n"), argv[0]);
        return (1);
    }

    /* A file must be open. */
    if (is_file_open == 0) {
        std_fprintf(out_p, OSTR("The file must be opened before it can be written to.\r\n"));

        return (1);
    }

    fat16_file_write(&file, argv[1], strlen(argv[1]));

    return (0);
}

static int init(void)
{
    sys_start();

    fs_command_init(&cmd_ls,
                    CSTR("/ls"),
                    cmd_ls_cb,
                    NULL);
    fs_command_register(&cmd_ls);

    fs_command_init(&cmd_open,
                    CSTR("/open"),
                    cmd_open_cb,
                    NULL);
    fs_command_register(&cmd_open);

    fs_command_init(&cmd_close,
                    CSTR("/close"),
                    cmd_close_cb,
                    NULL);
    fs_command_register(&cmd_close);

    fs_command_init(&cmd_read,
                    CSTR("/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&cmd_read);

    fs_command_init(&cmd_write,
                    CSTR("/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&cmd_write);

    std_printf(sys_get_info());

    spi_init(&spi,
             &spi_device[0],
             &pin_d6_dev,
             SPI_MODE_MASTER,
             SPI_SPEED_1MBPS,
             0,
             0);

    sd_init(&sd, &spi);

    if (sd_start(&sd) != 0) {
        std_printf(FSTR("Failed to start the SD card.\r\n"));
        return (-1);
    }

    std_printf(FSTR("SD card started.\r\n"));
    fat16_init(&fs,
               (fat16_read_t)sd_read_block,
               (fat16_write_t)sd_write_block,
               &sd,
               0);

    if (fat16_mount(&fs) != 0) {
        std_printf(FSTR("Failed to mount FAT16 file system.\r\n"));
        return (-1);
    }

    std_printf(FSTR("fat16 file system mounted\r\n"));

    return (0);
}

int main()
{
    if (init() != 0) {
        return (-1);
    }

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
