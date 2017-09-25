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
#include "fs_mock.h"

int mock_write_fs_module_init(int res)
{
    harness_mock_write("fs_module_init()",
                       NULL,
                       0);

    harness_mock_write("fs_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_module_init)()
{
    int res;

    harness_mock_assert("fs_module_init()",
                        NULL,
                        0);

    harness_mock_read("fs_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_call(char *command_p,
                       void *chin_p,
                       void *chout_p,
                       void *arg_p,
                       int res)
{
    harness_mock_write("fs_call(): return (command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("fs_call(chin_p)",
                       chin_p,
                       sizeof(chin_p));

    harness_mock_write("fs_call(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("fs_call(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("fs_call(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_call)(char *command_p,
                                         void *chin_p,
                                         void *chout_p,
                                         void *arg_p)
{
    int res;

    harness_mock_read("fs_call(): return (command_p)",
                      command_p,
                      sizeof(*command_p));

    harness_mock_assert("fs_call(chin_p)",
                        chin_p,
                        sizeof(*chin_p));

    harness_mock_assert("fs_call(chout_p)",
                        chout_p,
                        sizeof(*chout_p));

    harness_mock_assert("fs_call(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_read("fs_call(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_open(const char *path_p,
                       int flags,
                       int res)
{
    harness_mock_write("fs_open(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_open(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("fs_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_open)(struct fs_file_t *self_p,
                                         const char *path_p,
                                         int flags)
{
    int res;

    harness_mock_assert("fs_open(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fs_open(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_read("fs_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_close(int res)
{
    harness_mock_write("fs_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_close)(struct fs_file_t *self_p)
{
    int res;

    harness_mock_read("fs_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_read(void *dst_p,
                       size_t size,
                       ssize_t res)
{
    harness_mock_write("fs_read(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("fs_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("fs_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fs_read)(struct fs_file_t *self_p,
                                             void *dst_p,
                                             size_t size)
{
    ssize_t res;

    harness_mock_read("fs_read(): return (dst_p)",
                      dst_p,
                      size);

    harness_mock_assert("fs_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("fs_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_read_line(void *dst_p,
                            size_t size,
                            ssize_t res)
{
    harness_mock_write("fs_read_line(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("fs_read_line(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("fs_read_line(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fs_read_line)(struct fs_file_t *self_p,
                                                  void *dst_p,
                                                  size_t size)
{
    ssize_t res;

    harness_mock_read("fs_read_line(): return (dst_p)",
                      dst_p,
                      size);

    harness_mock_assert("fs_read_line(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("fs_read_line(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_write(const void *src_p,
                        size_t size,
                        ssize_t res)
{
    harness_mock_write("fs_write(src_p)",
                       src_p,
                       size);

    harness_mock_write("fs_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("fs_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fs_write)(struct fs_file_t *self_p,
                                              const void *src_p,
                                              size_t size)
{
    ssize_t res;

    harness_mock_assert("fs_write(src_p)",
                        src_p,
                        size);

    harness_mock_assert("fs_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("fs_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_seek(int offset,
                       int whence,
                       int res)
{
    harness_mock_write("fs_seek(offset)",
                       &offset,
                       sizeof(offset));

    harness_mock_write("fs_seek(whence)",
                       &whence,
                       sizeof(whence));

    harness_mock_write("fs_seek(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_seek)(struct fs_file_t *self_p,
                                         int offset,
                                         int whence)
{
    int res;

    harness_mock_assert("fs_seek(offset)",
                        &offset,
                        sizeof(offset));

    harness_mock_assert("fs_seek(whence)",
                        &whence,
                        sizeof(whence));

    harness_mock_read("fs_seek(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_tell(ssize_t res)
{
    harness_mock_write("fs_tell(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(fs_tell)(struct fs_file_t *self_p)
{
    ssize_t res;

    harness_mock_read("fs_tell(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_dir_open(struct fs_dir_t *dir_p,
                           const char *path_p,
                           int oflag,
                           int res)
{
    harness_mock_write("fs_dir_open(): return (dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("fs_dir_open(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_dir_open(oflag)",
                       &oflag,
                       sizeof(oflag));

    harness_mock_write("fs_dir_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_dir_open)(struct fs_dir_t *dir_p,
                                             const char *path_p,
                                             int oflag)
{
    int res;

    harness_mock_read("fs_dir_open(): return (dir_p)",
                      dir_p,
                      sizeof(*dir_p));

    harness_mock_assert("fs_dir_open(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fs_dir_open(oflag)",
                        &oflag,
                        sizeof(oflag));

    harness_mock_read("fs_dir_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_dir_close(struct fs_dir_t *dir_p,
                            int res)
{
    harness_mock_write("fs_dir_close(dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("fs_dir_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_dir_close)(struct fs_dir_t *dir_p)
{
    int res;

    harness_mock_assert("fs_dir_close(dir_p)",
                        dir_p,
                        sizeof(*dir_p));

    harness_mock_read("fs_dir_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_dir_read(struct fs_dir_t *dir_p,
                           struct fs_dir_entry_t *entry_p,
                           int res)
{
    harness_mock_write("fs_dir_read(dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("fs_dir_read(): return (entry_p)",
                       entry_p,
                       sizeof(*entry_p));

    harness_mock_write("fs_dir_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_dir_read)(struct fs_dir_t *dir_p,
                                             struct fs_dir_entry_t *entry_p)
{
    int res;

    harness_mock_assert("fs_dir_read(dir_p)",
                        dir_p,
                        sizeof(*dir_p));

    harness_mock_read("fs_dir_read(): return (entry_p)",
                      entry_p,
                      sizeof(*entry_p));

    harness_mock_read("fs_dir_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_remove(const char *path_p,
                         int res)
{
    harness_mock_write("fs_remove(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_remove(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_remove)(const char *path_p)
{
    int res;

    harness_mock_assert("fs_remove(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_read("fs_remove(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_stat(const char *path_p,
                       struct fs_stat_t *stat_p,
                       int res)
{
    harness_mock_write("fs_stat(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_stat(stat_p)",
                       stat_p,
                       sizeof(*stat_p));

    harness_mock_write("fs_stat(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_stat)(const char *path_p,
                                         struct fs_stat_t *stat_p)
{
    int res;

    harness_mock_assert("fs_stat(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fs_stat(stat_p)",
                        stat_p,
                        sizeof(*stat_p));

    harness_mock_read("fs_stat(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_mkdir(const char *path_p,
                        int res)
{
    harness_mock_write("fs_mkdir(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_mkdir(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_mkdir)(const char *path_p)
{
    int res;

    harness_mock_assert("fs_mkdir(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_read("fs_mkdir(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_format(const char *path_p,
                         int res)
{
    harness_mock_write("fs_format(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_format(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_format)(const char *path_p)
{
    int res;

    harness_mock_assert("fs_format(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_read("fs_format(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_ls(const char *path_p,
                     const char *filter_p,
                     void *chout_p,
                     int res)
{
    harness_mock_write("fs_ls(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_ls(filter_p)",
                       filter_p,
                       strlen(filter_p) + 1);

    harness_mock_write("fs_ls(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("fs_ls(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_ls)(const char *path_p,
                                       const char *filter_p,
                                       void *chout_p)
{
    int res;

    harness_mock_assert("fs_ls(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fs_ls(filter_p)",
                        filter_p,
                        sizeof(*filter_p));

    harness_mock_assert("fs_ls(chout_p)",
                        chout_p,
                        sizeof(*chout_p));

    harness_mock_read("fs_ls(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_list(const char *path_p,
                       const char *filter_p,
                       void *chout_p,
                       int res)
{
    harness_mock_write("fs_list(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_list(filter_p)",
                       filter_p,
                       strlen(filter_p) + 1);

    harness_mock_write("fs_list(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("fs_list(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_list)(const char *path_p,
                                         const char *filter_p,
                                         void *chout_p)
{
    int res;

    harness_mock_assert("fs_list(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fs_list(filter_p)",
                        filter_p,
                        sizeof(*filter_p));

    harness_mock_assert("fs_list(chout_p)",
                        chout_p,
                        sizeof(*chout_p));

    harness_mock_read("fs_list(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_auto_complete(char *path_p,
                                int res)
{
    harness_mock_write("fs_auto_complete(): return (path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_auto_complete(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_auto_complete)(char *path_p)
{
    int res;

    harness_mock_read("fs_auto_complete(): return (path_p)",
                      path_p,
                      sizeof(*path_p));

    harness_mock_read("fs_auto_complete(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_merge(char *path_p,
                        char *cmd_p)
{
    harness_mock_write("fs_merge(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("fs_merge(cmd_p)",
                       cmd_p,
                       strlen(cmd_p) + 1);

    return (0);
}

void __attribute__ ((weak)) STUB(fs_merge)(char *path_p,
                                           char *cmd_p)
{
    harness_mock_assert("fs_merge(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("fs_merge(cmd_p)",
                        cmd_p,
                        sizeof(*cmd_p));
}

int mock_write_fs_filesystem_init_generic(const char *name_p,
                                          struct fs_filesystem_operations_t *ops_p,
                                          int res)
{
    harness_mock_write("fs_filesystem_init_generic(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("fs_filesystem_init_generic(ops_p)",
                       ops_p,
                       sizeof(*ops_p));

    harness_mock_write("fs_filesystem_init_generic(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_filesystem_init_generic)(struct fs_filesystem_t *self_p,
                                                            const char *name_p,
                                                            struct fs_filesystem_operations_t *ops_p)
{
    int res;

    harness_mock_assert("fs_filesystem_init_generic(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("fs_filesystem_init_generic(ops_p)",
                        ops_p,
                        sizeof(*ops_p));

    harness_mock_read("fs_filesystem_init_generic(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_filesystem_init_fat16(const char *name_p,
                                        struct fat16_t *fat16_p,
                                        int res)
{
    harness_mock_write("fs_filesystem_init_fat16(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("fs_filesystem_init_fat16(fat16_p)",
                       fat16_p,
                       sizeof(*fat16_p));

    harness_mock_write("fs_filesystem_init_fat16(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_filesystem_init_fat16)(struct fs_filesystem_t *self_p,
                                                          const char *name_p,
                                                          struct fat16_t *fat16_p)
{
    int res;

    harness_mock_assert("fs_filesystem_init_fat16(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("fs_filesystem_init_fat16(fat16_p)",
                        fat16_p,
                        sizeof(*fat16_p));

    harness_mock_read("fs_filesystem_init_fat16(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_filesystem_init_spiffs(const char *name_p,
                                         struct spiffs_t *spiffs_p,
                                         struct fs_filesystem_spiffs_config_t *config_p,
                                         int res)
{
    harness_mock_write("fs_filesystem_init_spiffs(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("fs_filesystem_init_spiffs(spiffs_p)",
                       spiffs_p,
                       sizeof(*spiffs_p));

    harness_mock_write("fs_filesystem_init_spiffs(config_p)",
                       config_p,
                       sizeof(*config_p));

    harness_mock_write("fs_filesystem_init_spiffs(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_filesystem_init_spiffs)(struct fs_filesystem_t *self_p,
                                                           const char *name_p,
                                                           struct spiffs_t *spiffs_p,
                                                           struct fs_filesystem_spiffs_config_t *config_p)
{
    int res;

    harness_mock_assert("fs_filesystem_init_spiffs(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("fs_filesystem_init_spiffs(spiffs_p)",
                        spiffs_p,
                        sizeof(*spiffs_p));

    harness_mock_assert("fs_filesystem_init_spiffs(config_p)",
                        config_p,
                        sizeof(*config_p));

    harness_mock_read("fs_filesystem_init_spiffs(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_filesystem_register(int res)
{
    harness_mock_write("fs_filesystem_register(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_filesystem_register)(struct fs_filesystem_t *self_p)
{
    int res;

    harness_mock_read("fs_filesystem_register(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_filesystem_deregister(int res)
{
    harness_mock_write("fs_filesystem_deregister(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_filesystem_deregister)(struct fs_filesystem_t *self_p)
{
    int res;

    harness_mock_read("fs_filesystem_deregister(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_command_init(far_string_t path_p,
                               fs_callback_t callback,
                               void *arg_p,
                               int res)
{
    harness_mock_write("fs_command_init(path_p)",
                       &path_p,
                       sizeof(path_p));

    harness_mock_write("fs_command_init(callback)",
                       &callback,
                       sizeof(callback));

    harness_mock_write("fs_command_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("fs_command_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_command_init)(struct fs_command_t *self_p,
                                                 far_string_t path_p,
                                                 fs_callback_t callback,
                                                 void *arg_p)
{
    int res;

    harness_mock_assert("fs_command_init(path_p)",
                        &path_p,
                        sizeof(path_p));

    harness_mock_assert("fs_command_init(callback)",
                        &callback,
                        sizeof(callback));

    harness_mock_assert("fs_command_init(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_read("fs_command_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_command_register(struct fs_command_t *command_p,
                                   int res)
{
    harness_mock_write("fs_command_register(command_p)",
                       command_p,
                       sizeof(*command_p));

    harness_mock_write("fs_command_register(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_command_register)(struct fs_command_t *command_p)
{
    int res;

    harness_mock_assert("fs_command_register(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_read("fs_command_register(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_command_deregister(struct fs_command_t *command_p,
                                     int res)
{
    harness_mock_write("fs_command_deregister(command_p)",
                       command_p,
                       sizeof(*command_p));

    harness_mock_write("fs_command_deregister(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_command_deregister)(struct fs_command_t *command_p)
{
    int res;

    harness_mock_assert("fs_command_deregister(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_read("fs_command_deregister(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_counter_init(far_string_t path_p,
                               uint64_t value,
                               int res)
{
    harness_mock_write("fs_counter_init(path_p)",
                       &path_p,
                       sizeof(path_p));

    harness_mock_write("fs_counter_init(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("fs_counter_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_counter_init)(struct fs_counter_t *self_p,
                                                 far_string_t path_p,
                                                 uint64_t value)
{
    int res;

    harness_mock_assert("fs_counter_init(path_p)",
                        &path_p,
                        sizeof(path_p));

    harness_mock_assert("fs_counter_init(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("fs_counter_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_counter_increment(uint64_t value,
                                    int res)
{
    harness_mock_write("fs_counter_increment(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("fs_counter_increment(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_counter_increment)(struct fs_counter_t *self_p,
                                                      uint64_t value)
{
    int res;

    harness_mock_assert("fs_counter_increment(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("fs_counter_increment(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_counter_register(struct fs_counter_t *counter_p,
                                   int res)
{
    harness_mock_write("fs_counter_register(counter_p)",
                       counter_p,
                       sizeof(*counter_p));

    harness_mock_write("fs_counter_register(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_counter_register)(struct fs_counter_t *counter_p)
{
    int res;

    harness_mock_assert("fs_counter_register(counter_p)",
                        counter_p,
                        sizeof(*counter_p));

    harness_mock_read("fs_counter_register(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_counter_deregister(struct fs_counter_t *counter_p,
                                     int res)
{
    harness_mock_write("fs_counter_deregister(counter_p)",
                       counter_p,
                       sizeof(*counter_p));

    harness_mock_write("fs_counter_deregister(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_counter_deregister)(struct fs_counter_t *counter_p)
{
    int res;

    harness_mock_assert("fs_counter_deregister(counter_p)",
                        counter_p,
                        sizeof(*counter_p));

    harness_mock_read("fs_counter_deregister(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_parameter_init(far_string_t path_p,
                                 fs_parameter_set_callback_t set_cb,
                                 fs_parameter_print_callback_t print_cb,
                                 void *value_p,
                                 int res)
{
    harness_mock_write("fs_parameter_init(path_p)",
                       &path_p,
                       sizeof(path_p));

    harness_mock_write("fs_parameter_init(set_cb)",
                       &set_cb,
                       sizeof(set_cb));

    harness_mock_write("fs_parameter_init(print_cb)",
                       &print_cb,
                       sizeof(print_cb));

    harness_mock_write("fs_parameter_init(value_p)",
                       value_p,
                       sizeof(value_p));

    harness_mock_write("fs_parameter_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_parameter_init)(struct fs_parameter_t *self_p,
                                                   far_string_t path_p,
                                                   fs_parameter_set_callback_t set_cb,
                                                   fs_parameter_print_callback_t print_cb,
                                                   void *value_p)
{
    int res;

    harness_mock_assert("fs_parameter_init(path_p)",
                        &path_p,
                        sizeof(path_p));

    harness_mock_assert("fs_parameter_init(set_cb)",
                        &set_cb,
                        sizeof(set_cb));

    harness_mock_assert("fs_parameter_init(print_cb)",
                        &print_cb,
                        sizeof(print_cb));

    harness_mock_assert("fs_parameter_init(value_p)",
                        value_p,
                        sizeof(*value_p));

    harness_mock_read("fs_parameter_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_parameter_register(struct fs_parameter_t *parameter_p,
                                     int res)
{
    harness_mock_write("fs_parameter_register(parameter_p)",
                       parameter_p,
                       sizeof(*parameter_p));

    harness_mock_write("fs_parameter_register(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_parameter_register)(struct fs_parameter_t *parameter_p)
{
    int res;

    harness_mock_assert("fs_parameter_register(parameter_p)",
                        parameter_p,
                        sizeof(*parameter_p));

    harness_mock_read("fs_parameter_register(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_parameter_deregister(struct fs_parameter_t *parameter_p,
                                       int res)
{
    harness_mock_write("fs_parameter_deregister(parameter_p)",
                       parameter_p,
                       sizeof(*parameter_p));

    harness_mock_write("fs_parameter_deregister(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_parameter_deregister)(struct fs_parameter_t *parameter_p)
{
    int res;

    harness_mock_assert("fs_parameter_deregister(parameter_p)",
                        parameter_p,
                        sizeof(*parameter_p));

    harness_mock_read("fs_parameter_deregister(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_parameter_int_set(void *value_p,
                                    const char *src_p,
                                    int res)
{
    harness_mock_write("fs_parameter_int_set(): return (value_p)",
                       value_p,
                       sizeof(value_p));

    harness_mock_write("fs_parameter_int_set(src_p)",
                       src_p,
                       strlen(src_p) + 1);

    harness_mock_write("fs_parameter_int_set(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_parameter_int_set)(void *value_p,
                                                      const char *src_p)
{
    int res;

    harness_mock_read("fs_parameter_int_set(): return (value_p)",
                      value_p,
                      sizeof(*value_p));

    harness_mock_assert("fs_parameter_int_set(src_p)",
                        src_p,
                        sizeof(*src_p));

    harness_mock_read("fs_parameter_int_set(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_fs_parameter_int_print(void *chout_p,
                                      void *value_p,
                                      int res)
{
    harness_mock_write("fs_parameter_int_print(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("fs_parameter_int_print(value_p)",
                       value_p,
                       sizeof(value_p));

    harness_mock_write("fs_parameter_int_print(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(fs_parameter_int_print)(void *chout_p,
                                                        void *value_p)
{
    int res;

    harness_mock_assert("fs_parameter_int_print(chout_p)",
                        chout_p,
                        sizeof(*chout_p));

    harness_mock_assert("fs_parameter_int_print(value_p)",
                        value_p,
                        sizeof(*value_p));

    harness_mock_read("fs_parameter_int_print(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
