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

FS_COMMAND_DEFINE("/file/list", file_list);
FS_COMMAND_DEFINE("/file/read", file_read);
FS_COMMAND_DEFINE("/file/write", file_write);

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_args_t shell_args;

static struct spi_driver_t spi;
static struct sd_driver_t sd;
static struct fat16_t fs;

static int print_dir_name(chan_t *chan_p,
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

static int format_entry(chan_t *chan_p,
                        const struct fat16_dir_entry_t *entry_p)
{

    /* Print file name with possible blank fill */
    print_dir_name(chan_p, entry_p, 14);

    /* Print modify date/time if requested */
    std_fprintf(chan_p,
                FSTR("%u-%u-%u %u:%u:%u"),
                entry_p->latest_mod_date.year,
                entry_p->latest_mod_date.month,
                entry_p->latest_mod_date.day,
                entry_p->latest_mod_date.hour,
                entry_p->latest_mod_date.minute,
                entry_p->latest_mod_date.second);

    /* Print size if requested */
    if (entry_p->is_dir != 1) {
        std_fprintf(chan_p, FSTR(" %lu"), (unsigned long)entry_p->size);
    }

    std_fprintf(chan_p, FSTR("\r\n"));

    return (0);
}

int file_list(int argc,
              const char *argv[],
              void *out_p,
              void *in_p)
{
    UNUSED(in_p);

    struct fat16_dir_t dir;
    struct fat16_dir_entry_t entry;
    const char *path_p;

    if (argc != 2) {
        std_fprintf(out_p, FSTR("Usage: %s <path>\r\n"), argv[0]);
        return (1);
    }

    path_p = argv[1];

    /* Re-open the directory with read option set. */
    fat16_dir_open(&fs, &dir, path_p, O_READ);

    while (fat16_dir_read(&dir, &entry) == 1) {
        format_entry(out_p, &entry);
    }

    fat16_dir_close(&dir);

    return (0);
}

int file_read(int argc,
              const char *argv[],
              void *out_p,
              void *in_p)
{
    UNUSED(in_p);

    struct fat16_file_t file;
    const char *path_p;
    size_t size;
    char buf[64];

    if (argc != 2) {
        std_fprintf(out_p, FSTR("Usage: %s <path>\r\n"), argv[0]);
        return (1);
    }

    path_p = argv[1];

    /* Re-open the directory with read option set. */
    if (fat16_file_open(&fs, &file, path_p, O_READ) != 0) {
        std_fprintf(out_p, FSTR("%s: file not found\r\n"), path_p);
        return (1);
    }

    while ((size = fat16_file_read(&file, buf, sizeof(buf))) > 0) {
        chan_write(out_p, buf, size);
    }

    fat16_file_close(&file);

    std_fprintf(out_p, FSTR("\r\n"));

    return (0);
}

int file_write(int argc,
               const char *argv[],
               void *out_p,
               void *in_p)
{
    UNUSED(in_p);

    struct fat16_file_t file;
    const char *path_p;

    if (argc != 3) {
        std_fprintf(out_p, FSTR("Usage: %s <path> <data to write>\r\n"),
                    argv[0]);
        return (1);
    }

    path_p = argv[1];

    /* Re-open the directory with read option set. */
    if (fat16_file_open(&fs, &file, path_p, (O_CREAT | O_WRITE | O_SYNC)) != 0) {
        return (1);
    }

    fat16_file_write(&file, argv[2], strlen(argv[2]));
    fat16_file_close(&file);

    return (0);
}

static void init(void)
{
    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    sys_set_stdout(&uart.chout);

    std_printf(sys_get_appinfo());

    spi_init(&spi,
             &spi_device[0],
             &pin_d53_dev,
             SPI_MODE_MASTER,
             SPI_SPEED_2MBPS,
             0,
             1);

    sd_init(&sd, &spi);
    sd_start(&sd);
    fat16_init(&fs, &sd, 0);
    fat16_start(&fs);
}

int main()
{
    init();

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_entry(&shell_args);

    return (0);
}
