/**
 * @section License
 *
 * Copyright (C) 2009, William Greiman (Arduino Fat16 Library)
 * Copyright (C) 2013-2015, Mikael Patel (Cosa, Refactoring)
 * Copyright (C) 2015-2018, Erik Moqvist
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

#ifndef __FAT16_MOCK_H__
#define __FAT16_MOCK_H__

#include "simba.h"

int mock_write_fat16_init(fat16_read_t read,
                          fat16_write_t write,
                          void *arg_p,
                          unsigned int partition,
                          int res);

int mock_write_fat16_mount(int res);

int mock_write_fat16_unmount(int res);

int mock_write_fat16_format(int res);

int mock_write_fat16_print(void *chan_p,
                           int res);

int mock_write_fat16_file_open(struct fat16_file_t *file_p,
                               const char *path_p,
                               int oflag,
                               int res);

int mock_write_fat16_file_close(struct fat16_file_t *file_p,
                                int res);

int mock_write_fat16_file_read(struct fat16_file_t *file_p,
                               void *buf_p,
                               size_t size,
                               ssize_t res);

int mock_write_fat16_file_write(struct fat16_file_t *file_p,
                                const void *buf_p,
                                size_t size,
                                ssize_t res);

int mock_write_fat16_file_seek(struct fat16_file_t *file_p,
                               int pos,
                               int whence,
                               int res);

int mock_write_fat16_file_tell(struct fat16_file_t *file_p,
                               ssize_t res);

int mock_write_fat16_file_truncate(struct fat16_file_t *file_p,
                                   size_t size,
                                   int res);

int mock_write_fat16_file_size(struct fat16_file_t *file_p,
                               ssize_t res);

int mock_write_fat16_file_sync(struct fat16_file_t *file_p,
                               int res);

int mock_write_fat16_dir_open(struct fat16_dir_t *dir_p,
                              const char *path_p,
                              int oflag,
                              int res);

int mock_write_fat16_dir_close(struct fat16_dir_t *dir_p,
                               int res);

int mock_write_fat16_dir_read(struct fat16_dir_t *dir_p,
                              struct fat16_dir_entry_t *entry_p,
                              int res);

int mock_write_fat16_stat(const char *path_p,
                          struct fat16_stat_t *stat_p,
                          int res);

#endif
