/**
 * @section License
 *
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

#include "simba.h"
#include "spiffs_mock.h"

int mock_write_spiffs_probe_fs(struct spiffs_config_t *config_p,
                               int32_t res)
{
    harness_mock_write("spiffs_probe_fs(config_p)",
                       config_p,
                       sizeof(*config_p));

    harness_mock_write("spiffs_probe_fs(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_probe_fs)(struct spiffs_config_t *config_p)
{
    int32_t res;

    harness_mock_assert("spiffs_probe_fs(config_p)",
                        config_p,
                        sizeof(*config_p));

    harness_mock_read("spiffs_probe_fs(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_mount(struct spiffs_config_t *config_p,
                            uint8_t *work_p,
                            uint8_t *fd_space_p,
                            uint32_t fd_space_size,
                            void *cache_p,
                            uint32_t cache_size,
                            spiffs_check_callback_t check_cb,
                            int32_t res)
{
    harness_mock_write("spiffs_mount(config_p)",
                       config_p,
                       sizeof(*config_p));

    harness_mock_write("spiffs_mount(work_p)",
                       work_p,
                       sizeof(*work_p));

    harness_mock_write("spiffs_mount(fd_space_p)",
                       fd_space_p,
                       sizeof(*fd_space_p));

    harness_mock_write("spiffs_mount(fd_space_size)",
                       &fd_space_size,
                       sizeof(fd_space_size));

    harness_mock_write("spiffs_mount(cache_p)",
                       cache_p,
                       sizeof(cache_p));

    harness_mock_write("spiffs_mount(cache_size)",
                       &cache_size,
                       sizeof(cache_size));

    harness_mock_write("spiffs_mount(check_cb)",
                       &check_cb,
                       sizeof(check_cb));

    harness_mock_write("spiffs_mount(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_mount)(struct spiffs_t *self_p,
                                                  struct spiffs_config_t *config_p,
                                                  uint8_t *work_p,
                                                  uint8_t *fd_space_p,
                                                  uint32_t fd_space_size,
                                                  void *cache_p,
                                                  uint32_t cache_size,
                                                  spiffs_check_callback_t check_cb)
{
    int32_t res;

    harness_mock_assert("spiffs_mount(config_p)",
                        config_p,
                        sizeof(*config_p));

    harness_mock_assert("spiffs_mount(work_p)",
                        work_p,
                        sizeof(*work_p));

    harness_mock_assert("spiffs_mount(fd_space_p)",
                        fd_space_p,
                        sizeof(*fd_space_p));

    harness_mock_assert("spiffs_mount(fd_space_size)",
                        &fd_space_size,
                        sizeof(fd_space_size));

    harness_mock_assert("spiffs_mount(cache_p)",
                        cache_p,
                        sizeof(*cache_p));

    harness_mock_assert("spiffs_mount(cache_size)",
                        &cache_size,
                        sizeof(cache_size));

    harness_mock_assert("spiffs_mount(check_cb)",
                        &check_cb,
                        sizeof(check_cb));

    harness_mock_read("spiffs_mount(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_unmount()
{
    return (0);
}

void __attribute__ ((weak)) STUB(spiffs_unmount)(struct spiffs_t *self_p)
{
}

int mock_write_spiffs_creat(const char *path_p,
                            spiffs_mode_t mode,
                            int32_t res)
{
    harness_mock_write("spiffs_creat(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("spiffs_creat(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("spiffs_creat(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_creat)(struct spiffs_t *self_p,
                                                  const char *path_p,
                                                  spiffs_mode_t mode)
{
    int32_t res;

    harness_mock_assert("spiffs_creat(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("spiffs_creat(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_read("spiffs_creat(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_open(const char *path_p,
                           spiffs_flags_t flags,
                           spiffs_mode_t mode,
                           spiffs_file_t res)
{
    harness_mock_write("spiffs_open(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("spiffs_open(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("spiffs_open(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("spiffs_open(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

spiffs_file_t __attribute__ ((weak)) STUB(spiffs_open)(struct spiffs_t *self_p,
                                                       const char *path_p,
                                                       spiffs_flags_t flags,
                                                       spiffs_mode_t mode)
{
    spiffs_file_t res;

    harness_mock_assert("spiffs_open(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("spiffs_open(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_assert("spiffs_open(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_read("spiffs_open(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_open_by_dirent(struct spiffs_dirent_t *ent_p,
                                     spiffs_flags_t flags,
                                     spiffs_mode_t mode,
                                     spiffs_file_t res)
{
    harness_mock_write("spiffs_open_by_dirent(): return (ent_p)",
                       ent_p,
                       sizeof(*ent_p));

    harness_mock_write("spiffs_open_by_dirent(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("spiffs_open_by_dirent(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("spiffs_open_by_dirent(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

spiffs_file_t __attribute__ ((weak)) STUB(spiffs_open_by_dirent)(struct spiffs_t *self_p,
                                                                 struct spiffs_dirent_t *ent_p,
                                                                 spiffs_flags_t flags,
                                                                 spiffs_mode_t mode)
{
    spiffs_file_t res;

    harness_mock_read("spiffs_open_by_dirent(): return (ent_p)",
                      ent_p,
                      sizeof(*ent_p));

    harness_mock_assert("spiffs_open_by_dirent(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_assert("spiffs_open_by_dirent(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_read("spiffs_open_by_dirent(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_open_by_page(spiffs_page_ix_t page_ix,
                                   spiffs_flags_t flags,
                                   spiffs_mode_t mode,
                                   spiffs_file_t res)
{
    harness_mock_write("spiffs_open_by_page(page_ix)",
                       &page_ix,
                       sizeof(page_ix));

    harness_mock_write("spiffs_open_by_page(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("spiffs_open_by_page(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("spiffs_open_by_page(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

spiffs_file_t __attribute__ ((weak)) STUB(spiffs_open_by_page)(struct spiffs_t *self_p,
                                                               spiffs_page_ix_t page_ix,
                                                               spiffs_flags_t flags,
                                                               spiffs_mode_t mode)
{
    spiffs_file_t res;

    harness_mock_assert("spiffs_open_by_page(page_ix)",
                        &page_ix,
                        sizeof(page_ix));

    harness_mock_assert("spiffs_open_by_page(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_assert("spiffs_open_by_page(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_read("spiffs_open_by_page(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_read(spiffs_file_t fh,
                           void *buf_p,
                           int32_t len,
                           int32_t res)
{
    harness_mock_write("spiffs_read(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_read(buf_p)",
                       buf_p,
                       sizeof(buf_p));

    harness_mock_write("spiffs_read(len)",
                       &len,
                       sizeof(len));

    harness_mock_write("spiffs_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_read)(struct spiffs_t *self_p,
                                                 spiffs_file_t fh,
                                                 void *buf_p,
                                                 int32_t len)
{
    int32_t res;

    harness_mock_assert("spiffs_read(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_assert("spiffs_read(buf_p)",
                        buf_p,
                        sizeof(*buf_p));

    harness_mock_assert("spiffs_read(len)",
                        &len,
                        sizeof(len));

    harness_mock_read("spiffs_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_write(spiffs_file_t fh,
                            void *buf_p,
                            int32_t len,
                            int32_t res)
{
    harness_mock_write("spiffs_write(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_write(buf_p)",
                       buf_p,
                       sizeof(buf_p));

    harness_mock_write("spiffs_write(len)",
                       &len,
                       sizeof(len));

    harness_mock_write("spiffs_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_write)(struct spiffs_t *self_p,
                                                  spiffs_file_t fh,
                                                  void *buf_p,
                                                  int32_t len)
{
    int32_t res;

    harness_mock_assert("spiffs_write(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_assert("spiffs_write(buf_p)",
                        buf_p,
                        sizeof(*buf_p));

    harness_mock_assert("spiffs_write(len)",
                        &len,
                        sizeof(len));

    harness_mock_read("spiffs_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_lseek(spiffs_file_t fh,
                            int32_t offs,
                            int whence,
                            int32_t res)
{
    harness_mock_write("spiffs_lseek(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_lseek(offs)",
                       &offs,
                       sizeof(offs));

    harness_mock_write("spiffs_lseek(whence)",
                       &whence,
                       sizeof(whence));

    harness_mock_write("spiffs_lseek(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_lseek)(struct spiffs_t *self_p,
                                                  spiffs_file_t fh,
                                                  int32_t offs,
                                                  int whence)
{
    int32_t res;

    harness_mock_assert("spiffs_lseek(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_assert("spiffs_lseek(offs)",
                        &offs,
                        sizeof(offs));

    harness_mock_assert("spiffs_lseek(whence)",
                        &whence,
                        sizeof(whence));

    harness_mock_read("spiffs_lseek(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_remove(const char *path_p,
                             int32_t res)
{
    harness_mock_write("spiffs_remove(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("spiffs_remove(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_remove)(struct spiffs_t *self_p,
                                                   const char *path_p)
{
    int32_t res;

    harness_mock_assert("spiffs_remove(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_read("spiffs_remove(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_fremove(spiffs_file_t fh,
                              int32_t res)
{
    harness_mock_write("spiffs_fremove(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_fremove(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_fremove)(struct spiffs_t *self_p,
                                                    spiffs_file_t fh)
{
    int32_t res;

    harness_mock_assert("spiffs_fremove(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_read("spiffs_fremove(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_stat(const char *path_p,
                           struct spiffs_stat_t *stat_p,
                           int32_t res)
{
    harness_mock_write("spiffs_stat(path_p)",
                       path_p,
                       strlen(path_p) + 1);

    harness_mock_write("spiffs_stat(stat_p)",
                       stat_p,
                       sizeof(*stat_p));

    harness_mock_write("spiffs_stat(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_stat)(struct spiffs_t *self_p,
                                                 const char *path_p,
                                                 struct spiffs_stat_t *stat_p)
{
    int32_t res;

    harness_mock_assert("spiffs_stat(path_p)",
                        path_p,
                        sizeof(*path_p));

    harness_mock_assert("spiffs_stat(stat_p)",
                        stat_p,
                        sizeof(*stat_p));

    harness_mock_read("spiffs_stat(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_fstat(spiffs_file_t fh,
                            struct spiffs_stat_t *stat_p,
                            int32_t res)
{
    harness_mock_write("spiffs_fstat(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_fstat(stat_p)",
                       stat_p,
                       sizeof(*stat_p));

    harness_mock_write("spiffs_fstat(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_fstat)(struct spiffs_t *self_p,
                                                  spiffs_file_t fh,
                                                  struct spiffs_stat_t *stat_p)
{
    int32_t res;

    harness_mock_assert("spiffs_fstat(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_assert("spiffs_fstat(stat_p)",
                        stat_p,
                        sizeof(*stat_p));

    harness_mock_read("spiffs_fstat(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_fflush(spiffs_file_t fh,
                             int32_t res)
{
    harness_mock_write("spiffs_fflush(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_fflush(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_fflush)(struct spiffs_t *self_p,
                                                   spiffs_file_t fh)
{
    int32_t res;

    harness_mock_assert("spiffs_fflush(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_read("spiffs_fflush(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_close(spiffs_file_t fh,
                            int32_t res)
{
    harness_mock_write("spiffs_close(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_close(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_close)(struct spiffs_t *self_p,
                                                  spiffs_file_t fh)
{
    int32_t res;

    harness_mock_assert("spiffs_close(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_read("spiffs_close(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_rename(const char *old_path_p,
                             const char *new_path_p,
                             int32_t res)
{
    harness_mock_write("spiffs_rename(old_path_p)",
                       old_path_p,
                       strlen(old_path_p) + 1);

    harness_mock_write("spiffs_rename(new_path_p)",
                       new_path_p,
                       strlen(new_path_p) + 1);

    harness_mock_write("spiffs_rename(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_rename)(struct spiffs_t *self_p,
                                                   const char *old_path_p,
                                                   const char *new_path_p)
{
    int32_t res;

    harness_mock_assert("spiffs_rename(old_path_p)",
                        old_path_p,
                        sizeof(*old_path_p));

    harness_mock_assert("spiffs_rename(new_path_p)",
                        new_path_p,
                        sizeof(*new_path_p));

    harness_mock_read("spiffs_rename(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_errno(int32_t res)
{
    harness_mock_write("spiffs_errno(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_errno)(struct spiffs_t *self_p)
{
    int32_t res;

    harness_mock_read("spiffs_errno(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_clearerr()
{
    return (0);
}

void __attribute__ ((weak)) STUB(spiffs_clearerr)(struct spiffs_t *self_p)
{
}

int mock_write_spiffs_opendir(const char *name_p,
                              struct spiffs_dir_t *dir_p,
                              struct spiffs_dir_t *res)
{
    harness_mock_write("spiffs_opendir(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("spiffs_opendir(dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("spiffs_opendir(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct spiffs_dir_t *__attribute__ ((weak)) STUB(spiffs_opendir)(struct spiffs_t *self_p,
                                                                 const char *name_p,
                                                                 struct spiffs_dir_t *dir_p)
{
    struct spiffs_dir_t *res;

    harness_mock_assert("spiffs_opendir(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("spiffs_opendir(dir_p)",
                        dir_p,
                        sizeof(*dir_p));

    harness_mock_read("spiffs_opendir(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_closedir(struct spiffs_dir_t *dir_p,
                               int32_t res)
{
    harness_mock_write("spiffs_closedir(dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("spiffs_closedir(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_closedir)(struct spiffs_dir_t *dir_p)
{
    int32_t res;

    harness_mock_assert("spiffs_closedir(dir_p)",
                        dir_p,
                        sizeof(*dir_p));

    harness_mock_read("spiffs_closedir(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_readdir(struct spiffs_dir_t *dir_p,
                              struct spiffs_dirent_t *ent_p,
                              struct spiffs_dirent_t *res)
{
    harness_mock_write("spiffs_readdir(dir_p)",
                       dir_p,
                       sizeof(*dir_p));

    harness_mock_write("spiffs_readdir(ent_p)",
                       ent_p,
                       sizeof(*ent_p));

    harness_mock_write("spiffs_readdir(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct spiffs_dirent_t *__attribute__ ((weak)) STUB(spiffs_readdir)(struct spiffs_dir_t *dir_p,
                                                                    struct spiffs_dirent_t *ent_p)
{
    struct spiffs_dirent_t *res;

    harness_mock_assert("spiffs_readdir(dir_p)",
                        dir_p,
                        sizeof(*dir_p));

    harness_mock_assert("spiffs_readdir(ent_p)",
                        ent_p,
                        sizeof(*ent_p));

    harness_mock_read("spiffs_readdir(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_check(int32_t res)
{
    harness_mock_write("spiffs_check(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_check)(struct spiffs_t *self_p)
{
    int32_t res;

    harness_mock_read("spiffs_check(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_info(uint32_t *total_p,
                           uint32_t *used_p,
                           int32_t res)
{
    harness_mock_write("spiffs_info(total_p)",
                       total_p,
                       sizeof(*total_p));

    harness_mock_write("spiffs_info(used_p)",
                       used_p,
                       sizeof(*used_p));

    harness_mock_write("spiffs_info(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_info)(struct spiffs_t *self_p,
                                                 uint32_t *total_p,
                                                 uint32_t *used_p)
{
    int32_t res;

    harness_mock_assert("spiffs_info(total_p)",
                        total_p,
                        sizeof(*total_p));

    harness_mock_assert("spiffs_info(used_p)",
                        used_p,
                        sizeof(*used_p));

    harness_mock_read("spiffs_info(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_format(int32_t res)
{
    harness_mock_write("spiffs_format(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_format)(struct spiffs_t *self_p)
{
    int32_t res;

    harness_mock_read("spiffs_format(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_mounted(uint8_t res)
{
    harness_mock_write("spiffs_mounted(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

uint8_t __attribute__ ((weak)) STUB(spiffs_mounted)(struct spiffs_t *self_p)
{
    uint8_t res;

    harness_mock_read("spiffs_mounted(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_gc_quick(uint16_t max_free_pages,
                               int32_t res)
{
    harness_mock_write("spiffs_gc_quick(max_free_pages)",
                       &max_free_pages,
                       sizeof(max_free_pages));

    harness_mock_write("spiffs_gc_quick(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_gc_quick)(struct spiffs_t *self_p,
                                                     uint16_t max_free_pages)
{
    int32_t res;

    harness_mock_assert("spiffs_gc_quick(max_free_pages)",
                        &max_free_pages,
                        sizeof(max_free_pages));

    harness_mock_read("spiffs_gc_quick(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_gc(uint32_t size,
                         int32_t res)
{
    harness_mock_write("spiffs_gc(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("spiffs_gc(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_gc)(struct spiffs_t *self_p,
                                               uint32_t size)
{
    int32_t res;

    harness_mock_assert("spiffs_gc(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("spiffs_gc(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_eof(spiffs_file_t fh,
                          int32_t res)
{
    harness_mock_write("spiffs_eof(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_eof(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_eof)(struct spiffs_t *self_p,
                                                spiffs_file_t fh)
{
    int32_t res;

    harness_mock_assert("spiffs_eof(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_read("spiffs_eof(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_tell(spiffs_file_t fh,
                           int32_t res)
{
    harness_mock_write("spiffs_tell(fh)",
                       &fh,
                       sizeof(fh));

    harness_mock_write("spiffs_tell(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_tell)(struct spiffs_t *self_p,
                                                 spiffs_file_t fh)
{
    int32_t res;

    harness_mock_assert("spiffs_tell(fh)",
                        &fh,
                        sizeof(fh));

    harness_mock_read("spiffs_tell(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_set_file_callback_func(spiffs_file_callback_t cb_func,
                                             int32_t res)
{
    harness_mock_write("spiffs_set_file_callback_func(cb_func)",
                       &cb_func,
                       sizeof(cb_func));

    harness_mock_write("spiffs_set_file_callback_func(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_set_file_callback_func)(struct spiffs_t *self_p,
                                                                   spiffs_file_callback_t cb_func)
{
    int32_t res;

    harness_mock_assert("spiffs_set_file_callback_func(cb_func)",
                        &cb_func,
                        sizeof(cb_func));

    harness_mock_read("spiffs_set_file_callback_func(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_vis(int32_t res)
{
    harness_mock_write("spiffs_vis(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int32_t __attribute__ ((weak)) STUB(spiffs_vis)(struct spiffs_t *self_p)
{
    int32_t res;

    harness_mock_read("spiffs_vis(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_buffer_bytes_for_filedescs(uint32_t num_descs,
                                                 uint32_t res)
{
    harness_mock_write("spiffs_buffer_bytes_for_filedescs(num_descs)",
                       &num_descs,
                       sizeof(num_descs));

    harness_mock_write("spiffs_buffer_bytes_for_filedescs(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

uint32_t __attribute__ ((weak)) STUB(spiffs_buffer_bytes_for_filedescs)(struct spiffs_t *self_p,
                                                                        uint32_t num_descs)
{
    uint32_t res;

    harness_mock_assert("spiffs_buffer_bytes_for_filedescs(num_descs)",
                        &num_descs,
                        sizeof(num_descs));

    harness_mock_read("spiffs_buffer_bytes_for_filedescs(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_spiffs_buffer_bytes_for_cache(uint32_t num_pages,
                                             uint32_t res)
{
    harness_mock_write("spiffs_buffer_bytes_for_cache(num_pages)",
                       &num_pages,
                       sizeof(num_pages));

    harness_mock_write("spiffs_buffer_bytes_for_cache(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

uint32_t __attribute__ ((weak)) STUB(spiffs_buffer_bytes_for_cache)(struct spiffs_t *self_p,
                                                                    uint32_t num_pages)
{
    uint32_t res;

    harness_mock_assert("spiffs_buffer_bytes_for_cache(num_pages)",
                        &num_pages,
                        sizeof(num_pages));

    harness_mock_read("spiffs_buffer_bytes_for_cache(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
