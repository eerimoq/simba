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

/*
 * spiffs.h
 *
 *  Created on: May 26, 2013
 *      Author: petera
 */

#ifndef __FILESYSTEMS_SPIFFS_H__
#define __FILESYSTEMS_SPIFFS_H__

#include "simba.h"

#include "spiffs/config.h"

#define SPIFFS_OK                       0
#define SPIFFS_ERR_NOT_MOUNTED          -10000
#define SPIFFS_ERR_FULL                 -10001
#define SPIFFS_ERR_NOT_FOUND            -10002
#define SPIFFS_ERR_END_OF_OBJECT        -10003
#define SPIFFS_ERR_DELETED              -10004
#define SPIFFS_ERR_NOT_FINALIZED        -10005
#define SPIFFS_ERR_NOT_INDEX            -10006
#define SPIFFS_ERR_OUT_OF_FILE_DESCS    -10007
#define SPIFFS_ERR_FILE_CLOSED          -10008
#define SPIFFS_ERR_FILE_DELETED         -10009
#define SPIFFS_ERR_BAD_DESCRIPTOR       -10010
#define SPIFFS_ERR_IS_INDEX             -10011
#define SPIFFS_ERR_IS_FREE              -10012
#define SPIFFS_ERR_INDEX_SPAN_MISMATCH  -10013
#define SPIFFS_ERR_DATA_SPAN_MISMATCH   -10014
#define SPIFFS_ERR_INDEX_REF_FREE       -10015
#define SPIFFS_ERR_INDEX_REF_LU         -10016
#define SPIFFS_ERR_INDEX_REF_INVALID    -10017
#define SPIFFS_ERR_INDEX_FREE           -10018
#define SPIFFS_ERR_INDEX_LU             -10019
#define SPIFFS_ERR_INDEX_INVALID        -10020
#define SPIFFS_ERR_NOT_WRITABLE         -10021
#define SPIFFS_ERR_NOT_READABLE         -10022
#define SPIFFS_ERR_CONFLICTING_NAME     -10023
#define SPIFFS_ERR_NOT_CONFIGURED       -10024

#define SPIFFS_ERR_NOT_A_FS             -10025
#define SPIFFS_ERR_MOUNTED              -10026
#define SPIFFS_ERR_ERASE_FAIL           -10027
#define SPIFFS_ERR_MAGIC_NOT_POSSIBLE   -10028

#define SPIFFS_ERR_NO_DELETED_BLOCKS    -10029

#define SPIFFS_ERR_FILE_EXISTS          -10030

#define SPIFFS_ERR_NOT_A_FILE           -10031
#define SPIFFS_ERR_RO_NOT_IMPL          -10032
#define SPIFFS_ERR_RO_ABORTED_OPERATION -10033
#define SPIFFS_ERR_PROBE_TOO_FEW_BLOCKS -10034
#define SPIFFS_ERR_PROBE_NOT_A_FS       -10035
#define SPIFFS_ERR_NAME_TOO_LONG        -10036

#define SPIFFS_ERR_INTERNAL             -10050

#define SPIFFS_ERR_TEST                 -10100

/** Spiffs file descriptor index type. must be signed. */
typedef int16_t spiffs_file_t;
/** Spiffs file descriptor flags. */
typedef uint16_t spiffs_flags_t;
/** Spiffs file mode. */
typedef uint16_t spiffs_mode_t;
/** Object type. */
typedef uint8_t spiffs_obj_type_t;

struct spiffs_t;

#if SPIFFS_HAL_CALLBACK_EXTRA

/** Spi read call function type. */
typedef int32_t (*spiffs_read_cb_t)(struct spiffs_t *fs_p,
                                    uint32_t addr,
                                    uint32_t size,
                                    uint8_t *dst_p);
/** Spi write call function type. */
typedef int32_t (*spiffs_write_cb_t)(struct spiffs_t *fs_p,
                                     uint32_t addr,
                                     uint32_t size,
                                     uint8_t *src_p);
/** Spi erase call function type. */
typedef int32_t (*spiffs_erase_cb_t)(struct spiffs_t *fs_p,
                                     uint32_t addr,
                                     uint32_t size);

#else

/** Spi read call function type. */
typedef int32_t (*spiffs_read_cb_t)(uint32_t addr,
                                    uint32_t size,
                                    uint8_t *dst_p);
/** Spi write call function type. */
typedef int32_t (*spiffs_write_cb_t)(uint32_t addr,
                                     uint32_t size,
                                     uint8_t *src_p);
/** Spi erase call function type. */
typedef int32_t (*spiffs_erase_cb_t)(uint32_t addr,
                                     uint32_t size);
#endif

/** File system check callback report operation. */
enum spiffs_check_type_t {
    SPIFFS_CHECK_LOOKUP = 0,
    SPIFFS_CHECK_INDEX,
    SPIFFS_CHECK_PAGE
};

/** File system check callback report type. */
enum spiffs_check_report_t {
    SPIFFS_CHECK_PROGRESS = 0,
    SPIFFS_CHECK_ERROR,
    SPIFFS_CHECK_FIX_INDEX,
    SPIFFS_CHECK_FIX_LOOKUP,
    SPIFFS_CHECK_DELETE_ORPHANED_INDEX,
    SPIFFS_CHECK_DELETE_PAGE,
    SPIFFS_CHECK_DELETE_BAD_FILE
};

/** File system check callback function. */
#if SPIFFS_HAL_CALLBACK_EXTRA
typedef void (*spiffs_check_callback_t)(struct spiffs_t *fs_p,
                                        enum spiffs_check_type_t type,
                                        enum spiffs_check_report_t report,
                                        uint32_t arg1,
                                        uint32_t arg2);
#else
typedef void (*spiffs_check_callback_t)(enum spiffs_check_type_t type,
                                        enum spiffs_check_report_t report,
                                        uint32_t arg1,
                                        uint32_t arg2);
#endif

/** File system listener callback operation. */
enum spiffs_fileop_type_t {
    /** The file has been created. */
    SPIFFS_CB_CREATED = 0,
    /** The file has been updated or moved to another page. */
    SPIFFS_CB_UPDATED,
    /** The file has been deleted. */
    SPIFFS_CB_DELETED
};

/** File system listener callback function. */
typedef void (*spiffs_file_callback_t)(struct spiffs_t *fs_p,
                                       enum spiffs_fileop_type_t op,
                                       spiffs_obj_id_t obj_id,
                                       spiffs_page_ix_t pix);

#ifndef SPIFFS_DBG
#define SPIFFS_DBG(...)                         \
    print(__VA_ARGS__)
#endif
#ifndef SPIFFS_GC_DBG
#define SPIFFS_GC_DBG(...) printf(__VA_ARGS__)
#endif
#ifndef SPIFFS_CACHE_DBG
#define SPIFFS_CACHE_DBG(...) printf(__VA_ARGS__)
#endif
#ifndef SPIFFS_CHECK_DBG
#define SPIFFS_CHECK_DBG(...) printf(__VA_ARGS__)
#endif

/** Any write to the filehandle is appended to end of the file. */
#define SPIFFS_APPEND                   0x04 //(1<<0)
#define SPIFFS_O_APPEND                 SPIFFS_APPEND
/** If the opened file exists, it will be truncated to zero length
    before opened. */
#define SPIFFS_TRUNC                    0x40 //(1<<1)
#define SPIFFS_O_TRUNC                  SPIFFS_TRUNC
/** If the opened file does not exist, it will be created before
    opened. */
#define SPIFFS_CREAT                    0x10 //(1<<2)
#define SPIFFS_O_CREAT                  SPIFFS_CREAT
/** The opened file may only be read. */
#define SPIFFS_RDONLY                   0x01 //(1<<3)
#define SPIFFS_O_RDONLY                 SPIFFS_RDONLY
/** The opened file may only be written. */
#define SPIFFS_WRONLY                   0x02 //(1<<4)
#define SPIFFS_O_WRONLY                 SPIFFS_WRONLY
/** The opened file may be both read and written. */
#define SPIFFS_RDWR                     (SPIFFS_RDONLY | SPIFFS_WRONLY)
#define SPIFFS_O_RDWR                   SPIFFS_RDWR
/** Any writes to the filehandle will never be cached but flushed
    directly. */
#define SPIFFS_DIRECT                   0x08 //(1<<5)
#define SPIFFS_O_DIRECT                 SPIFFS_DIRECT
/** If SPIFFS_O_CREAT and SPIFFS_O_EXCL are set, SPIFFS_open() shall
    fail if the file exists. */
#define SPIFFS_EXCL                     0x20 //(1<<6)
#define SPIFFS_O_EXCL                   SPIFFS_EXCL

#define SPIFFS_SEEK_SET                 (0)
#define SPIFFS_SEEK_CUR                 (1)
#define SPIFFS_SEEK_END                 (2)

#define SPIFFS_TYPE_FILE                (1)
#define SPIFFS_TYPE_DIR                 (2)
#define SPIFFS_TYPE_HARD_LINK           (3)
#define SPIFFS_TYPE_SOFT_LINK           (4)

#ifndef SPIFFS_LOCK
#define SPIFFS_LOCK(fs)
#endif

#ifndef SPIFFS_UNLOCK
#define SPIFFS_UNLOCK(fs)
#endif

/** Spiffs spi configuration struct. */
struct spiffs_config_t {
    /** Physical read function. */
    spiffs_read_cb_t hal_read_f;
    /** Physical write function. */
    spiffs_write_cb_t hal_write_f;
    /** Physical erase function. */
    spiffs_erase_cb_t hal_erase_f;
#if SPIFFS_SINGLETON == 0
    /** Physical size of the spi flash. */
    uint32_t phys_size;
    /** Physical offset in spi flash used for spiffs, must be on block
        boundary. */
    uint32_t phys_addr;
    /** Physical size when erasing a block. */
    uint32_t phys_erase_block;

    /** Logical size of a block, must be on physical block size
        boundary and must never be less than a physical block. */
    uint32_t log_block_size;
    /** Logical size of a page, must be at least log_block_size /
        8. */
    uint32_t log_page_size;

#endif
#if SPIFFS_FILEHDL_OFFSET
    /** An integer offset added to each file handle. */
    uint16_t fh_ix_offset;
#endif
};

struct spiffs_t {
    /** File system configuration. */
    struct spiffs_config_t cfg;
    /** Number of logical blocks. */
    uint32_t block_count;

    /** Cursor for free blocks, block index. */
    spiffs_block_ix_t free_cursor_block_ix;
    /** Cursor for free blocks, entry index. */
    int free_cursor_obj_lu_entry;
    /** Cursor when searching, block index. */
    spiffs_block_ix_t cursor_block_ix;
    /** Cursor when searching, entry index. */
    int cursor_obj_lu_entry;

    /** Primary work buffer, size of a logical page. */
    uint8_t *lu_work;
    /** Secondary work buffer, size of a logical page. */
    uint8_t *work;
    /** File descriptor memory area. */
    uint8_t *fd_space;
    /** Available file descriptors. */
    uint32_t fd_count;

    /** Last error. */
    int32_t err_code;

    /** Current number of free blocks. */
    uint32_t free_blocks;
    /** Current number of busy pages. */
    uint32_t stats_p_allocated;
    /** Current number of deleted pages. */
    uint32_t stats_p_deleted;
    /** Flag indicating that garbage collector is cleaning. */
    uint8_t cleaning;
    /** Max erase count amongst all blocks. */
    spiffs_obj_id_t max_erase_count;

#if SPIFFS_GC_STATS
    uint32_t stats_gc_runs;
#endif

#if SPIFFS_CACHE
    // cache memory
    void *cache;
    // cache size
    uint32_t cache_size;
#if SPIFFS_CACHE_STATS
    uint32_t cache_hits;
    uint32_t cache_misses;
#endif
#endif

    /** Check callback function. */
    spiffs_check_callback_t check_cb_f;
    /** File callback function. */
    spiffs_file_callback_t file_cb_f;
    /** Mounted flag. */
    uint8_t mounted;
    /** User data. */
    void *user_data;
    /** Config magic. */
    uint32_t config_magic;
};

/** Spiffs file status struct. */
struct spiffs_stat_t {
    spiffs_obj_id_t obj_id;
    uint32_t size;
    spiffs_obj_type_t type;
    spiffs_page_ix_t pix;
    uint8_t name[SPIFFS_OBJ_NAME_LEN];
};

struct spiffs_dirent_t {
    spiffs_obj_id_t obj_id;
    uint8_t name[SPIFFS_OBJ_NAME_LEN];
    spiffs_obj_type_t type;
    uint32_t size;
    spiffs_page_ix_t pix;
};

struct spiffs_dir_t {
    struct spiffs_t *fs;
    spiffs_block_ix_t block;
    int entry;
};

#if SPIFFS_USE_MAGIC && SPIFFS_USE_MAGIC_LENGTH && SPIFFS_SINGLETON == 0

/**
 * Special function. This takes a spiffs config struct and returns the
 * number of blocks this file system was formatted with. This function
 * relies on that following info is set correctly in given config
 * struct:
 *
 * phys_addr, log_page_size, and log_block_size.
 *
 * Also, hal_read_f must be set in the config struct.
 *
 * One must be sure of the correct page size and that the physical
 * address is correct in the probed file system when calling this
 * function. It is not checked if the phys_addr actually points to the
 * start of the file system, so one might get a false positive if
 * entering a phys_addr somewhere in the middle of the file system at
 * block boundary. In addition, it is not checked if the page size is
 * actually correct. If it is not, weird file system sizes will be
 * returned.
 *
 * If this function detects a file system it returns the assumed file
 * system size, which can be used to set the phys_size.
 *
 * Otherwise, it returns an error indicating why it is not regarded as
 * a file system.
 *
 * Note: This function is not protected with SPIFFS_LOCK and
 *       SPIFFS_UNLOCK macros. It returns the error code directly,
 *       instead of as read by SPIFFS_errno.
 *
 * @param[in] config_p Essential parts of the physical and logical
 *                     configuration of the file system.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_probe_fs(struct spiffs_config_t *config_p);

#endif

/**
 * Initializes the file system dynamic parameters and mounts the
 * filesystem.  If SPIFFS_USE_MAGIC is enabled the mounting may fail
 * with SPIFFS_ERR_NOT_A_FS if the flash does not contain a
 * recognizable file system.  In this case, SPIFFS_format must be
 * called prior to remounting.
 *
 * @param[in] self_p The file system struct.
 * @param[in] config_p The physical and logical configuration of the
 *                     file system.
 * @param[in] work_p A memory work buffer comprising
 *                   2*config->log_page_size bytes used throughout all
 *                   file system operations
 * @param[in] fd_space_p Memory for file descriptors.
 * @param[in] fd_space_size Memory size of file descriptors.
 * @param[in] cache_p Memory for cache, may be NULL.
 * @param[in] cache_size Memory size of cache.
 * @param[in] check_cb Callback function for reporting during
 *                     consistency checks.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_mount(struct spiffs_t *self_p,
                     struct spiffs_config_t *config_p,
                     uint8_t *work_p,
                     uint8_t *fd_space_p,
                     uint32_t fd_space_size,
                     void *cache_p,
                     uint32_t cache_size,
                     spiffs_check_callback_t check_cb);

/**
 * Unmounts the file system. All file handles will be flushed of any
 * cached writes and closed.
 *
 * @param[in] self_p The file system struct.
 *
 * @return void.
 */
void spiffs_unmount(struct spiffs_t *self_p);

/**
 * Creates a new file.
 *
 * @param[in] self_p The file system struct.
 * @param[in] path_p The path of the new file.
 * @param[in] mode Ignored, for posix compliance.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_creat(struct spiffs_t *self_p,
                     const char *path_p,
                     spiffs_mode_t mode);

/**
 * Opens/creates a file.
 *
 * @param[in] self_p The file system struct.
 * @param[in] path_p The path of the new file.
 * @param[in] flags The flags for the open command, can be
 *                  combinations of SPIFFS_O_APPEND, SPIFFS_O_TRUNC,
 *                  SPIFFS_O_CREAT, SPIFFS_O_RDONLY, SPIFFS_O_WRONLY,
 *                  SPIFFS_O_RDWR, SPIFFS_O_DIRECT, SPIFFS_O_EXCL.
 * @param[in] mode Ignored, for posix compliance.
 */
spiffs_file_t spiffs_open(struct spiffs_t *self_p,
                          const char *path_p,
                          spiffs_flags_t flags,
                          spiffs_mode_t mode);

/**
 * Opens a file by given dir entry.
 *
 * Optimization purposes, when traversing a file system with
 * SPIFFS_readdir a normal SPIFFS_open would need to traverse the
 * filesystem again to find the file, whilst SPIFFS_open_by_dirent
 * already knows where the file resides.
 *
 * @param[in] self_p The file system struct.
 * @param[in] e_p The dir entry to the file.
 * @param[in] flags The flags for the open command, can be
 *                  combinations of SPIFFS_APPEND, SPIFFS_TRUNC,
 *                  SPIFFS_CREAT, SPIFFS_RD_ONLY, SPIFFS_WR_ONLY,
 *                  SPIFFS_RDWR, SPIFFS_DIRECT.  SPIFFS_CREAT will
 *                  have no effect in this case.
 * @param[in] mode Ignored, for posix compliance.
 */
spiffs_file_t spiffs_open_by_dirent(struct spiffs_t *self_p,
                                    struct spiffs_dirent_t *ent_p,
                                    spiffs_flags_t flags,
                                    spiffs_mode_t mode);

/**
 * Opens a file by given page index.
 *
 * Optimization purposes, opens a file by directly pointing to the
 * page index in the spi flash.  If the page index does not point to a
 * file header SPIFFS_ERR_NOT_A_FILE is returned.
 *
 * @param[in] self_p The file system struct.
 * @param[in] page_ix The page index.
 * @param[in] flags The flags for the open command, can be
 *                  combinations of SPIFFS_APPEND, SPIFFS_TRUNC,
 *                  SPIFFS_CREAT, SPIFFS_RD_ONLY, SPIFFS_WR_ONLY,
 *                  SPIFFS_RDWR, SPIFFS_DIRECT.  SPIFFS_CREAT will
 *                  have no effect in this case.
 * @param[in] mode Ignored, for posix compliance.
 */
spiffs_file_t spiffs_open_by_page(struct spiffs_t *self_p,
                                  spiffs_page_ix_t page_ix,
                                  spiffs_flags_t flags,
                                  spiffs_mode_t mode);

/**
 * Reads from given filehandle.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle.
 * @param[in] buf_p Where to put read data.
 * @param[in] len How much to read.
 *
 * @return Number of bytes read or negative error code.
 */
int32_t spiffs_read(struct spiffs_t *self_p,
                    spiffs_file_t fh,
                    void *buf_p,
                    int32_t len);

/**
 * Writes to given filehandle.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle.
 * @param[in] buf_p The data to write.
 * @param[in] len How much to write.
 *
 * @returns Number of bytes written, or negative error code.
 */
int32_t spiffs_write(struct spiffs_t *self_p,
                     spiffs_file_t fh,
                     void *buf_p,
                     int32_t len);

/**
 * Moves the read/write file offset. Resulting offset is returned or
 * negative if error.
 *
 * lseek(fs, fd, 0, SPIFFS_SEEK_CUR) will thus return current offset.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle.
 * @param[in] offs How much/where to move the offset.
 * @param[in] whence If SPIFFS_SEEK_SET, the file offset shall be set
 *                   to offset bytes.
 *                   
 *                   If SPIFFS_SEEK_CUR, the file offset shall be set
 *                   to its current location plus offset.
 *
 *                   If SPIFFS_SEEK_END, the file offset shall be set
 *                   to the size of the file plus offse, which should
 *                   be negative.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_lseek(struct spiffs_t *self_p,
                     spiffs_file_t fh,
                     int32_t offs,
                     int whence);

/**
 * Removes a file by path.
 *
 * @param[in] self_p The file system struct.
 * @param[in] path_p The path of the file to remove.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_remove(struct spiffs_t *self_p,
                      const char *path_p);

/**
 * Removes a file by filehandle.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle of the file to remove.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_fremove(struct spiffs_t *self_p,
                       spiffs_file_t fh);

/**
 * Gets file status by path.
 *
 * @param[in] self_p The file system struct.
 * @param[in] path_p The path of the file to stat.
 * @param[in] stat_p The stat struct to populate.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_stat(struct spiffs_t *self_p,
                    const char *path_p,
                    struct spiffs_stat_t *stat_p);

/**
 * Gets file status by filehandle.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle of the file to stat.
 * @param[in] stat_p The stat struct to populate.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_fstat(struct spiffs_t *self_p,
                     spiffs_file_t fh,
                     struct spiffs_stat_t *stat_p);

/**
 * Flushes all pending write operations from cache for given file.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle of the file to flush.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_fflush(struct spiffs_t *self_p,
                      spiffs_file_t fh);

/**
 * Closes a filehandle. If there are pending write operations, these
 * are finalized before closing.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle of the file to close.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_close(struct spiffs_t *self_p,
                     spiffs_file_t fh);

/**
 * Renames a file.
 *
 * @param[in] self_p The file system struct.
 * @param[in] old_path_p Path of file to rename.
 * @param[in] new_path_p New path of file.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_rename(struct spiffs_t *self_p,
                      const char *old_path_p,
                      const char *new_path_p);

/**
 * Returns last error of last file operation.
 *
 * @param[in] self_p The file system struct.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_errno(struct spiffs_t *self_p);

/**
 * Clears last error.
 *
 * @param[in] self_p The file system struct.
 *
 * @return void.
 */
void spiffs_clearerr(struct spiffs_t *self_p);

/**
 * Opens a directory stream corresponding to the given name.  The
 * stream is positioned at the first entry in the directory.  On
 * hydrogen builds the name argument is ignored as hydrogen builds
 * always correspond to a flat file structure - no directories.
 *
 * @param[in] self_p The file system struct.
 * @param[in] name_p The name of the directory.
 * @param[in] dir_p Pointer the directory stream to be populated.
 */
struct spiffs_dir_t *spiffs_opendir(struct spiffs_t *self_p,
                                    const char *name_p,
                                    struct spiffs_dir_t *dir_p);

/**
 * Closes a directory stream
 *
 * @param[in] dir_p The directory stream to close.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_closedir(struct spiffs_dir_t *dir_p);

/**
 * Reads a directory into given spifs_dirent struct.
 *
 * @param[in] dir_p Pointer to the directory stream.
 * @param[in] ent_p The dirent struct to be populated.
 *
 * @return NULL if error or end of stream, else given dirent is
 *         returned.
 */
struct spiffs_dirent_t *spiffs_readdir(struct spiffs_dir_t *dir_p,
                                       struct spiffs_dirent_t *ent_p);

/**
 * Runs a consistency check on given filesystem.
 *
 * @param[in] self_p The file system struct.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_check(struct spiffs_t *self_p);

/**
 * Returns number of total bytes available and number of used bytes.
 * This is an estimation, and depends on if there a many files with
 * little data or few files with much data.
 *
 * NB: If used number of bytes exceeds total bytes, a SPIFFS_check
 *     should run. This indicates a power loss in midst of things. In
 *     worst case (repeated powerlosses in mending or gc) you might
 *     have to delete some files.
 *
 * @param[in] self_p The file system struct.
 * @param[in] total_p Total number of bytes in filesystem.
 * @param[in] used_p Used number of bytes in filesystem.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_info(struct spiffs_t *self_p,
                    uint32_t *total_p,
                    uint32_t *used_p);

/**
 * Formats the entire file system. All data will be lost.
 * The filesystem must not be mounted when calling this.
 *
 * NB: formatting is awkward. Due to backwards compatibility,
 *     SPIFFS_mount MUST be called prior to formatting in order to
 *     configure the filesystem.  If SPIFFS_mount succeeds,
 *     SPIFFS_unmount must be called before calling SPIFFS_format.  If
 *     SPIFFS_mount fails, SPIFFS_format can be called directly
 *     without calling SPIFFS_unmount first.
 *
 * @param[in] self_p The file system struct.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_format(struct spiffs_t *self_p);

/**
 * Returns nonzero if spiffs is mounted, or zero if unmounted.
 *
 * @param[in] self_p The file system struct.
 */
uint8_t spiffs_mounted(struct spiffs_t *self_p);

/**
 * Tries to find a block where most or all pages are deleted, and
 * erase that block if found. Does not care for wear levelling. Will
 * not move pages around.
 *
 * If parameter max_free_pages are set to 0, only blocks with only
 * deleted pages will be selected.
 *
 * NB: the garbage collector is automatically called when spiffs needs
 * free pages. The reason for this function is to give possibility to
 * do background tidying when user knows the system is idle.
 *
 * Use with care.
 *
 * Setting max_free_pages to anything larger than zero will eventually
 * wear flash more as a block containing free pages can be erased.
 *
 * Will set err_no to SPIFFS_OK if a block was found and erased,
 * SPIFFS_ERR_NO_DELETED_BLOCK if no matching block was found, or
 * other error.
 *
 * @param[in] self_p The file system struct.
 * @param[in] max_free_pages maximum number allowed free pages in
 *                           block.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_gc_quick(struct spiffs_t *self_p,
                        uint16_t max_free_pages);

/**
 * Will try to make room for given amount of bytes in the filesystem
 * by moving pages and erasing blocks.  If it is physically
 * impossible, err_no will be set to SPIFFS_ERR_FULL. If there already
 * is this amount (or more) of free space, SPIFFS_gc will silently
 * return. It is recommended to call SPIFFS_info before invoking this
 * method in order to determine what amount of bytes to give.
 *
 * NB: the garbage collector is automatically called when spiffs needs
 * free pages. The reason for this function is to give possibility to
 * do background tidying when user knows the system is idle.
 *
 * Use with care.
 *
 * @param[in] self_p The file system struct.
 * @param[in] size Amount of bytes that should be freed.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_gc(struct spiffs_t *self_p,
                  uint32_t size);

/**
 * Check if EOF reached.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle of the file to check.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_eof(struct spiffs_t *self_p,
                   spiffs_file_t fh);

/**
 * Get position in file.
 *
 * @param[in] self_p The file system struct.
 * @param[in] fh The filehandle of the file to check.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_tell(struct spiffs_t *self_p,
                    spiffs_file_t fh);

/**
 * Registers a callback function that keeps track on operations on
 * file headers. Do note, that this callback is called from within
 * internal spiffs mechanisms. Any operations on the actual file
 * system being callbacked from in this callback will mess things up
 * for sure - do not do this.  This can be used to track where files
 * are and move around during garbage collection, which in turn can be
 * used to build location tables in ram.  Used in conjuction with
 * SPIFFS_open_by_page this may improve performance when opening a lot
 * of files.  Must be invoked after mount.
 *
 * @param[in] self_p The file system struct.
 * @param[in] cb_func The callback on file operations.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_set_file_callback_func(struct spiffs_t *self_p,
                                      spiffs_file_callback_t cb_func);

#if SPIFFS_TEST_VISUALISATION
/**
 * Prints out a visualization of the filesystem.
 *
 * @param[in] self_p The file system struct.
 *
 * @return zero(0) or negative error code.
 */
int32_t spiffs_vis(struct spiffs_t *self_p);
#endif

#if SPIFFS_BUFFER_HELP
/**
 * Returns number of bytes needed for the filedescriptor buffer given
 * amount of file descriptors.
 */
uint32_t spiffs_buffer_bytes_for_filedescs(struct spiffs_t *self_p,
                                           uint32_t num_descs);

#    if SPIFFS_CACHE
/**
 * Returns number of bytes needed for the cache buffer given amount of
 * cache pages.
 */
uint32_t spiffs_buffer_bytes_for_cache(struct spiffs_t *self_p,
                                       uint32_t num_pages);
#    endif
#endif

/* Used internally by spiffs. */
typedef spiffs_block_ix_t spiffs_block_ix;
typedef spiffs_page_ix_t spiffs_page_ix;
typedef spiffs_obj_id_t spiffs_obj_id;
typedef spiffs_span_ix_t spiffs_span_ix;
typedef struct spiffs_t spiffs;
typedef spiffs_file_t spiffs_file;
typedef spiffs_flags_t spiffs_flags;
typedef spiffs_obj_type_t spiffs_obj_type;
typedef spiffs_mode_t spiffs_mode;
typedef enum spiffs_fileop_type_t spiffs_fileop_type;
typedef struct spiffs_config_t spiffs_config;
typedef spiffs_check_callback_t spiffs_check_callback;
typedef struct spiffs_dirent_t spiffs_dirent;
typedef struct spiffs_dir_t spiffs_DIR;
typedef spiffs_file_callback_t spiffs_file_callback;

#endif
