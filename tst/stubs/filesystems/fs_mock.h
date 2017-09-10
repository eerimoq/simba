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

#ifndef __FS_MOCK_H__
#define __FS_MOCK_H__

#include "simba.h"

int mock_write_fs_module_init(int res);

int mock_write_fs_call(char *command_p,
                       void *chin_p,
                       void *chout_p,
                       void *arg_p,
                       int res);

int mock_write_fs_open(const char *path_p,
                       int flags,
                       int res);

int mock_write_fs_close(int res);

int mock_write_fs_read(void *dst_p,
                       size_t size,
                       ssize_t res);

int mock_write_fs_read_line(void *dst_p,
                            size_t size,
                            ssize_t res);

int mock_write_fs_write(const void *src_p,
                        size_t size,
                        ssize_t res);

int mock_write_fs_seek(int offset,
                       int whence,
                       int res);

int mock_write_fs_tell(ssize_t res);

int mock_write_fs_dir_open(struct fs_dir_t *dir_p,
                           const char *path_p,
                           int oflag,
                           int res);

int mock_write_fs_dir_close(struct fs_dir_t *dir_p,
                            int res);

int mock_write_fs_dir_read(struct fs_dir_t *dir_p,
                           struct fs_dir_entry_t *entry_p,
                           int res);

int mock_write_fs_remove(const char *path_p,
                         int res);

int mock_write_fs_stat(const char *path_p,
                       struct fs_stat_t *stat_p,
                       int res);

int mock_write_fs_mkdir(const char *path_p,
                        int res);

int mock_write_fs_format(const char *path_p,
                         int res);

int mock_write_fs_ls(const char *path_p,
                     const char *filter_p,
                     void *chout_p,
                     int res);

int mock_write_fs_list(const char *path_p,
                       const char *filter_p,
                       void *chout_p,
                       int res);

int mock_write_fs_auto_complete(char *path_p,
                                int res);

int mock_write_fs_merge(char *path_p,
                        char *cmd_p);

int mock_write_fs_filesystem_init_generic(const char *name_p,
                                          struct fs_filesystem_operations_t *ops_p,
                                          int res);

int mock_write_fs_filesystem_init_fat16(const char *name_p,
                                        struct fat16_t *fat16_p,
                                        int res);

int mock_write_fs_filesystem_init_spiffs(const char *name_p,
                                         struct spiffs_t *spiffs_p,
                                         struct fs_filesystem_spiffs_config_t *config_p,
                                         int res);

int mock_write_fs_filesystem_register(int res);

int mock_write_fs_filesystem_deregister(int res);

int mock_write_fs_command_init(far_string_t path_p,
                               fs_callback_t callback,
                               void *arg_p,
                               int res);

int mock_write_fs_command_register(struct fs_command_t *command_p,
                                   int res);

int mock_write_fs_command_deregister(struct fs_command_t *command_p,
                                     int res);

int mock_write_fs_counter_init(far_string_t path_p,
                               uint64_t value,
                               int res);

int mock_write_fs_counter_increment(uint64_t value,
                                    int res);

int mock_write_fs_counter_register(struct fs_counter_t *counter_p,
                                   int res);

int mock_write_fs_counter_deregister(struct fs_counter_t *counter_p,
                                     int res);

int mock_write_fs_parameter_init(far_string_t path_p,
                                 fs_parameter_set_callback_t set_cb,
                                 fs_parameter_print_callback_t print_cb,
                                 void *value_p,
                                 int res);

int mock_write_fs_parameter_register(struct fs_parameter_t *parameter_p,
                                     int res);

int mock_write_fs_parameter_deregister(struct fs_parameter_t *parameter_p,
                                       int res);

int mock_write_fs_parameter_int_set(void *value_p,
                                    const char *src_p,
                                    int res);

int mock_write_fs_parameter_int_print(void *chout_p,
                                      void *value_p,
                                      int res);

#endif
