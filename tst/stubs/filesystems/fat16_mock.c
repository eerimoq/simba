/**
 * @section License
 *
 * Copyright (C) 2009, William Greiman (Arduino Fat16 Library)
 * Copyright (C) 2013-2015, Mikael Patel (Cosa, Refactoring)
 * Copyright (C) 2015-2017, Erik Moqvist
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
#include "fat16_mock.h"

int mock_write_fat16_init(fat16_read_t read,
                          fat16_write_t write,
                          void *arg_p,
                          unsigned int partition,
                          int res)
{
    harness_mock_write("fat16_init(read)",
                       &read,
                       sizeof(read));

    harness_mock_write("fat16_init(write)",
                       &write,
                       sizeof(write));

    harness_mock_write("fat16_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("fat16_init(partition)",
                       &partition,
                       sizeof(partition));

    harness_mock_write("fat16_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_init)(struct fat16_t *self_p,
                                            fat16_read_t read,
                                            fat16_write_t write,
                                            void *arg_p,
                                            unsigned int partition)
{
    int res;

    harness_mock_assert("fat16_init(read)",
                        &read,
                        sizeof(read));

    harness_mock_assert("fat16_init(write)",
                        &write,
                        sizeof(write));

    harness_mock_assert("fat16_init(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_assert("fat16_init(partition)",
                        &partition,
                        sizeof(partition));

    harness_mock_read("fat16_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_mount(int res)
{
    harness_mock_write("fat16_mount(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_mount)(struct fat16_t *self_p)
{
    int res;

    harness_mock_read("fat16_mount(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_unmount(int res)
{
    harness_mock_write("fat16_unmount(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_unmount)(struct fat16_t *self_p)
{
    int res;

    harness_mock_read("fat16_unmount(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_format(int res)
{
    harness_mock_write("fat16_format(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_format)(struct fat16_t *self_p)
{
    int res;

    harness_mock_read("fat16_format(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_print(void *chan_p,
                           int res)
{
    harness_mock_write("fat16_print(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("fat16_print(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_print)(struct fat16_t *self_p,
                                             void *chan_p)
{
    int res;

    harness_mock_assert("fat16_print(chan_p)",
                        chan_p,
                        sizeof(*chan_p));

    harness_mock_read("fat16_print(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_open(struct fat16_file_t *file_p,
                               const char *path_p,
                               int oflag,
                               int res)
{
    harness_mock_write("fat16_file_open(): return (file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_open(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fat16_file_open(oflag)",
                       &oflag,
                       sizeof(oflag));

    harness_mock_write("fat16_file_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_file_open)(struct fat16_t *self_p,
                                                 struct fat16_file_t *file_p,
                                                 const char *path_p,
                                                 int oflag)
{
    int res;

    harness_mock_read("fat16_file_open(): return (file_p)",
                      file_p,
                      sizeof(*file_p));

    harness_mock_assert("fat16_file_open(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fat16_file_open(oflag)",
                        &oflag,
                        sizeof(oflag));

    harness_mock_read("fat16_file_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_close(struct fat16_file_t *file_p,
                                int res)
{
    harness_mock_write("fat16_file_close(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_file_close)(struct fat16_file_t *file_p)
{
    int res;

    harness_mock_assert("fat16_file_close(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_read("fat16_file_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_read(struct fat16_file_t *file_p,
                               void *buf_p,
                               size_t size,
                               ssize_t res)
{
    harness_mock_write("fat16_file_read(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("fat16_file_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("fat16_file_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fat16_file_read)(struct fat16_file_t *file_p,
                                                     void *buf_p,
                                                     size_t size)
{
    ssize_t res;

    harness_mock_assert("fat16_file_read(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_assert("fat16_file_read(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("fat16_file_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("fat16_file_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_write(struct fat16_file_t *file_p,
                                const void *buf_p,
                                size_t size,
                                ssize_t res)
{
    harness_mock_write("fat16_file_write(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("fat16_file_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("fat16_file_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fat16_file_write)(struct fat16_file_t *file_p,
                                                      const void *buf_p,
                                                      size_t size)
{
    ssize_t res;

    harness_mock_assert("fat16_file_write(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_assert("fat16_file_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("fat16_file_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("fat16_file_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_seek(struct fat16_file_t *file_p,
                               int pos,
                               int whence,
                               int res)
{
    harness_mock_write("fat16_file_seek(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_seek(pos)",
                       &pos,
                       sizeof(pos));

    harness_mock_write("fat16_file_seek(whence)",
                       &whence,
                       sizeof(whence));

    harness_mock_write("fat16_file_seek(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_file_seek)(struct fat16_file_t *file_p,
                                                 int pos,
                                                 int whence)
{
    int res;

    harness_mock_assert("fat16_file_seek(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_assert("fat16_file_seek(pos)",
                        &pos,
                        sizeof(pos));

    harness_mock_assert("fat16_file_seek(whence)",
                        &whence,
                        sizeof(whence));

    harness_mock_read("fat16_file_seek(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_tell(struct fat16_file_t *file_p,
                               ssize_t res)
{
    harness_mock_write("fat16_file_tell(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_tell(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fat16_file_tell)(struct fat16_file_t *file_p)
{
    ssize_t res;

    harness_mock_assert("fat16_file_tell(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_read("fat16_file_tell(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_truncate(struct fat16_file_t *file_p,
                                   size_t size,
                                   int res)
{
    harness_mock_write("fat16_file_truncate(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_truncate(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("fat16_file_truncate(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_file_truncate)(struct fat16_file_t *file_p,
                                                     size_t size)
{
    int res;

    harness_mock_assert("fat16_file_truncate(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_assert("fat16_file_truncate(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("fat16_file_truncate(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_size(struct fat16_file_t *file_p,
                               ssize_t res)
{
    harness_mock_write("fat16_file_size(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fat16_file_size)(struct fat16_file_t *file_p)
{
    ssize_t res;

    harness_mock_assert("fat16_file_size(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_read("fat16_file_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_file_sync(struct fat16_file_t *file_p,
                               int res)
{
    harness_mock_write("fat16_file_sync(file_p)",
                       file_p,
                       sizeof(*file_p));

    harness_mock_write("fat16_file_sync(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_file_sync)(struct fat16_file_t *file_p)
{
    int res;

    harness_mock_assert("fat16_file_sync(file_p)",
                        file_p,
                        sizeof(*file_p));

    harness_mock_read("fat16_file_sync(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_dir_open(struct fat16_dir_t *dir_p,
                              const char *path_p,
                              int oflag,
                              int res)
{
    harness_mock_write("fat16_dir_open(): return (dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("fat16_dir_open(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fat16_dir_open(oflag)",
                       &oflag,
                       sizeof(oflag));

    harness_mock_write("fat16_dir_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_dir_open)(struct fat16_t *self_p,
                                                struct fat16_dir_t *dir_p,
                                                const char *path_p,
                                                int oflag)
{
    int res;

    harness_mock_read("fat16_dir_open(): return (dir_p)",
                      dir_p,
                      sizeof(*dir_p));

    harness_mock_assert("fat16_dir_open(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fat16_dir_open(oflag)",
                        &oflag,
                        sizeof(oflag));

    harness_mock_read("fat16_dir_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_dir_close(struct fat16_dir_t *dir_p,
                               int res)
{
    harness_mock_write("fat16_dir_close(dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("fat16_dir_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_dir_close)(struct fat16_dir_t *dir_p)
{
    int res;

    harness_mock_assert("fat16_dir_close(dir_p)",
                        dir_p,
                        sizeof(*dir_p));

    harness_mock_read("fat16_dir_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_dir_read(struct fat16_dir_t *dir_p,
                              struct fat16_dir_entry_t *entry_p,
                              int res)
{
    harness_mock_write("fat16_dir_read(dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("fat16_dir_read(): return (entry_p)",
                       entry_p,
                       sizeof(*entry_p));

    harness_mock_write("fat16_dir_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_dir_read)(struct fat16_dir_t *dir_p,
                                                struct fat16_dir_entry_t *entry_p)
{
    int res;

    harness_mock_assert("fat16_dir_read(dir_p)",
                        dir_p,
                        sizeof(*dir_p));

    harness_mock_read("fat16_dir_read(): return (entry_p)",
                      entry_p,
                      sizeof(*entry_p));

    harness_mock_read("fat16_dir_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fat16_stat(const char *path_p,
                          struct fat16_stat_t *stat_p,
                          int res)
{
    harness_mock_write("fat16_stat(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fat16_stat(stat_p)",
                       stat_p,
                       sizeof(*stat_p));

    harness_mock_write("fat16_stat(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fat16_stat)(struct fat16_t *self_p,
                                            const char *path_p,
                                            struct fat16_stat_t *stat_p)
{
    int res;

    harness_mock_assert("fat16_stat(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fat16_stat(stat_p)",
                        stat_p,
                        sizeof(*stat_p));

    harness_mock_read("fat16_stat(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
