/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#ifndef __SPIFFS_MOCK_H__
#define __SPIFFS_MOCK_H__

#include "simba.h"

int mock_write_spiffs_probe_fs(struct spiffs_config_t *config_p,
                               int32_t res);

int mock_write_spiffs_mount(struct spiffs_config_t *config_p,
                            uint8_t *work_p,
                            uint8_t *fd_space_p,
                            uint32_t fd_space_size,
                            void *cache_p,
                            uint32_t cache_size,
                            spiffs_check_callback_t check_cb,
                            int32_t res);

int mock_write_spiffs_unmount();

int mock_write_spiffs_creat(const char *path_p,
                            spiffs_mode_t mode,
                            int32_t res);

int mock_write_spiffs_open(const char *path_p,
                           spiffs_flags_t flags,
                           spiffs_mode_t mode,
                           spiffs_file_t res);

int mock_write_spiffs_open_by_dirent(struct spiffs_dirent_t *ent_p,
                                     spiffs_flags_t flags,
                                     spiffs_mode_t mode,
                                     spiffs_file_t res);

int mock_write_spiffs_open_by_page(spiffs_page_ix_t page_ix,
                                   spiffs_flags_t flags,
                                   spiffs_mode_t mode,
                                   spiffs_file_t res);

int mock_write_spiffs_read(spiffs_file_t fh,
                           void *buf_p,
                           int32_t len,
                           int32_t res);

int mock_write_spiffs_write(spiffs_file_t fh,
                            void *buf_p,
                            int32_t len,
                            int32_t res);

int mock_write_spiffs_lseek(spiffs_file_t fh,
                            int32_t offs,
                            int whence,
                            int32_t res);

int mock_write_spiffs_remove(const char *path_p,
                             int32_t res);

int mock_write_spiffs_fremove(spiffs_file_t fh,
                              int32_t res);

int mock_write_spiffs_stat(const char *path_p,
                           struct spiffs_stat_t *stat_p,
                           int32_t res);

int mock_write_spiffs_fstat(spiffs_file_t fh,
                            struct spiffs_stat_t *stat_p,
                            int32_t res);

int mock_write_spiffs_fflush(spiffs_file_t fh,
                             int32_t res);

int mock_write_spiffs_close(spiffs_file_t fh,
                            int32_t res);

int mock_write_spiffs_rename(const char *old_path_p,
                             const char *new_path_p,
                             int32_t res);

int mock_write_spiffs_errno(int32_t res);

int mock_write_spiffs_clearerr();

int mock_write_spiffs_opendir(const char *name_p,
                              struct spiffs_dir_t *dir_p,
                              struct spiffs_dir_t *res);

int mock_write_spiffs_closedir(struct spiffs_dir_t *dir_p,
                               int32_t res);

int mock_write_spiffs_readdir(struct spiffs_dir_t *dir_p,
                              struct spiffs_dirent_t *ent_p,
                              struct spiffs_dirent_t *res);

int mock_write_spiffs_check(int32_t res);

int mock_write_spiffs_info(uint32_t *total_p,
                           uint32_t *used_p,
                           int32_t res);

int mock_write_spiffs_format(int32_t res);

int mock_write_spiffs_mounted(uint8_t res);

int mock_write_spiffs_gc_quick(uint16_t max_free_pages,
                               int32_t res);

int mock_write_spiffs_gc(uint32_t size,
                         int32_t res);

int mock_write_spiffs_eof(spiffs_file_t fh,
                          int32_t res);

int mock_write_spiffs_tell(spiffs_file_t fh,
                           int32_t res);

int mock_write_spiffs_set_file_callback_func(spiffs_file_callback_t cb_func,
                                             int32_t res);

int mock_write_spiffs_vis(int32_t res);

int mock_write_spiffs_buffer_bytes_for_filedescs(uint32_t num_descs,
                                                 uint32_t res);

int mock_write_spiffs_buffer_bytes_for_cache(uint32_t num_pages,
                                             uint32_t res);

#endif
