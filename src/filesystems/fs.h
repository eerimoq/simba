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

#ifndef __FILESYSTEMS_FS_H__
#define __FILESYSTEMS_FS_H__

#include "simba.h"

/**
 * The offset is relative to the start of the file.
 */
#define FS_SEEK_SET 0

/**
 * The offset is relative to the current position indicator.
 */
#define FS_SEEK_CUR 1

/**
 * The offset is relative to the end of the file.
 */
#define FS_SEEK_END 2

/**
 * Open for reading.
 */
#define FS_READ             0x01

/**
 * Open for write.
 */
#define FS_WRITE            0x02

/**
 * Open for reading and writing.
 */
#define FS_RDWR             (FS_READ | FS_WRITE)

/**
 * The file position indicator shall be set to the end of the file
 * prior to each write.
 */
#define FS_APPEND           0x04

/**
 * Synchronous writes.
 */
#define FS_SYNC             0x08

/**
 * Create the file if non-existent.
 */
#define FS_CREAT            0x10

/**
 * If FS_CREAT and FS_EXCL are set, file open shall fail if the file
 * exists.
 */
#define FS_EXCL             0x20

/**
 * Truncate the file to zero length.
 */
#define FS_TRUNC            0x40

#define FS_TYPE_FILE                1
#define FS_TYPE_DIR                 2
#define FS_TYPE_HARD_LINK           3
#define FS_TYPE_SOFT_LINK           4

/**
 * Command callback prototype.
 *
 * @param[in] argc Number of arguements in argv.
 * @param[in] argv An array of agruments.
 * @param[in] out_p Output channel.
 * @param[in] in_p Input channel.
 * @param[in] arg_p Argument passed to the init function of given
 *                  command.
 * @param[in] call_arg_p Argument passed to the ``fs_call`` function.
 *
 * @return zero(0) or negative error code.
 */
typedef int (*fs_callback_t)(int argc,
                             const char *argv[],
                             void *out_p,
                             void *in_p,
                             void *arg_p,
                             void *call_arg_p);

/**
 * Parameter setter callback prototype.
 *
 * @param[out] value_p Buffer the new value should be written to.
 * @param[in] src_p Value to set as a string.
 *
 * @return zero(0) or negative error code.
 */
typedef int (*fs_parameter_set_callback_t)(void *value_p,
                                           const char *src_p);

/**
 * Parameter printer callback prototype.
 *
 * @param[in] chout_p Channel to write the formatted value to.
 * @param[in] value_p Value to format and print to the output channel.
 *
 * @return zero(0) or negative error code.
 */
typedef int (*fs_parameter_print_callback_t)(void *chout_p,
                                             void *value_p);

enum fs_type_t {
    fs_type_fat16_t = 0,
    fs_type_spiffs_t,
    fs_type_generic_t
};

#if CONFIG_SPIFFS == 1

/**
 * A SPIFFS file system.
 */
struct fs_filesystem_spiffs_config_t {
    struct spiffs_config_t *config_p;
    uint8_t *workspace_p;
    struct {
        uint8_t *buf_p;
        size_t size;
    } fdworkspace;
    struct {
        uint8_t *buf_p;
        size_t size;
    } cache;
};

#endif

#if CONFIG_FAT16 == 1

/**
 * A FAT16 file system.
 */
struct fs_filesystem_fat16_t {
    struct fat16_t *fat16_p;
};

#endif

/* File system. */
struct fs_filesystem_t {
    const char *name_p;
    enum fs_type_t type;
    union {
#if CONFIG_FAT16 == 1
        struct fat16_t *fat16_p;
#endif
#if CONFIG_SPIFFS == 1
        struct spiffs_t *spiffs_p;
#endif
        struct {
            struct fs_filesystem_operations_t *ops_p;
        } generic;
    } fs;
    union {
#if CONFIG_SPIFFS == 1
        struct fs_filesystem_spiffs_config_t *spiffs_p;
#endif
    } config;
    struct fs_filesystem_t *next_p;
};

/* A file. */
struct fs_file_t {
    struct fs_filesystem_t *filesystem_p;
    union {
#if CONFIG_FAT16 == 1
        struct fat16_file_t fat16;
#endif
#if CONFIG_SPIFFS == 1
        spiffs_file_t spiffs;
#endif
    } u;
};

/** Path stats. */
struct fs_stat_t {
    uint32_t size;
    uint8_t type;
};

/* Command. */
struct fs_command_t {
    far_string_t path_p;
    fs_callback_t callback;
    void *arg_p;
    struct fs_command_t *next_p;
};

/* Counter. */
struct fs_counter_t {
    struct fs_command_t command;
    long long unsigned int value;
    struct fs_counter_t *next_p;
};

/* Parameter. */
struct fs_parameter_t {
    struct fs_command_t command;
    fs_parameter_set_callback_t set_cb;
    fs_parameter_print_callback_t print_cb;
    void *value_p;
    struct fs_parameter_t *next_p;
};

struct fs_dir_t {
    struct fs_filesystem_t *filesystem_p;
    union {
#if CONFIG_FAT16 == 1
        struct fat16_dir_t fat16;
#endif
#if CONFIG_SPIFFS == 1
        struct spiffs_dir_t spiffs;
#endif
    } u;
};

struct fs_dir_entry_t {
    char name[256];
    int type;
    size_t size;
    struct date_t latest_mod_date;
};

struct fs_filesystem_operations_t {
    int (*file_open)(struct fs_filesystem_t *filesystem_p,
                     struct fs_file_t *self_p,
                     const char *path_p,
                     int flags);
    int (*file_close)(struct fs_file_t *self_p);
    ssize_t (*file_read)(struct fs_file_t *self_p, void *dst_p, size_t size);
    ssize_t (*file_write)(struct fs_file_t *self_p, const void *src_p, size_t size);
    int (*file_seek)(struct fs_file_t *self_p, int offset, int whence);
    ssize_t (*file_tell)(struct fs_file_t *self_p);
};

/**
 * Initialize the file system module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int fs_module_init(void);

/**
 * Call given file system command with given input and output
 * channels. Quote an argument if it contains spaces, otherwise it is
 * parsed as multiple arguments. Any quotation mark in an argument
 * string must be escaped with a backslash (``\``), otherwise it is
 * interpreted as a string quotation mask.
 *
 * @param[in,out] command_p Command string to call. The command string
 *                          will be modified by this function, so
 *                          don't use it after this function returns.
 * @param[in] chin_p Input channel.
 * @param[in] chout_p Output channel.
 * @param[in] arg_p User argument passed to the command callback
 *                  function as ``call_arg_p``.
 *
 * @return zero(0) or negative error code.
 */
int fs_call(char *command_p,
            void *chin_p,
            void *chout_p,
            void *arg_p);

/**
 * Open a file by file path and mode flags. File operations are
 * permitted after the file has been opened.
 *
 * The path can be either absolute or relative. It's an absolute path
 * if it starts with a forward slash ``/``, and relative
 * otherwise. Relative paths are are relative to the current working
 * directory, given by the thread environment variable ``CWD``.
 *
 * @param[out] self_p File object to be initialized.
 * @param[in] path_p Path of the file to open. The path can be
 *                   absolute or relative.
 * @param[in] flags Mode of file open. A combination of ``FS_READ``,
 *                  ``FS_RDONLY``, ``FS_WRITE``, ``FS_WRONLY``,
 *                  ``FS_RDWR``, ``FS_APPEND``, ``FS_SYNC``,
 *                  ``FS_CREAT``, ``FS_EXCL`` and ``FS_TRUNC``.
 *
 * @return zero(0) or negative error code.
 */
int fs_open(struct fs_file_t *self_p, const char *path_p, int flags);

/**
 * Close given file. No file operations are permitted on a closed
 * file.
 *
 * @param[in] self_p Initialized file object.
 *
 * @return zero(0) or negative error code.
 */
int fs_close(struct fs_file_t *self_p);

/**
 * Read from given file into given buffer.
 *
 * @param[in] self_p Initialized file object.
 * @param[out] dst_p Buffer to read data into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t fs_read(struct fs_file_t *self_p, void *dst_p, size_t size);

/**
 * Read one line from given file into given buffer. The function reads
 * one character at a time from given file until the destination
 * buffer is full, a newline ``\n`` is found or end of file is
 * reached.
 *
 * @param[in] self_p Initialized file object.
 * @param[out] dst_p Buffer to read data into. Should fit the whole
 *                   line and null-termination.
 * @param[in] size Size of the destination buffer.
 *
 * @return If a line was found the number of bytes read not including
 *         the null-termination is returned. If the destination buffer
 *         becomes full before a newline character, the destination
 *         buffer size is returned. Otherwise a negative error code is
 *         returned.
 */
ssize_t fs_read_line(struct fs_file_t *self_p, void *dst_p, size_t size);

/**
 * Write from given buffer into given file.
 *
 * @param[in] self_p Initialized file object.
 * @param[in] dst_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t fs_write(struct fs_file_t *self_p, const void *src_p, size_t size);

/**
 * Sets the file's read/write position relative to whence.
 *
 * @param[in] self_p Initialized file object.
 * @param[in] offset New position in bytes from given whence.
 * @param[in] whence Absolute (``FS_SEEK_SET``), relative
 *                   (``FS_SEEK_CUR``) or from end (``FS_SEEK_END``).
 *
 * @return zero(0) or negative error code.
 */
int fs_seek(struct fs_file_t *self_p, int offset, int whence);

/**
 * Return current position in the file.
 *
 * @param[in] self_p Initialized file object.
 *
 * @return Current position or negative error code.
 */
ssize_t fs_tell(struct fs_file_t *self_p);

/**
 * Open a directory by directory path and mode flags.
 *
 * @param[out] dir_p Directory object to be initialized.
 * @param[in] path_p A valid path name for a directory path.
 * @param[in] oflag mode of the directory to open (create, read, etc).
 *
 * @return zero(0) or negative error code.
 */
int fs_dir_open(struct fs_dir_t *dir_p,
                const char *path_p,
                int oflag);

/**
 * Close given directory.
 *
 * @param[in] dir_p Directory object.
 *
 * @return zero(0) or negative error code.
 */
int fs_dir_close(struct fs_dir_t *dir_p);

/**
 * Read the next file or directory within the opened directory.
 *
 * @param[in] dir_p Directory object.
 * @param[out] entry_p Read entry.
 *
 * @return true(1) if an entry was read or false(0) if no entry could
 *         be read, otherwise negative error code.
 */
int fs_dir_read(struct fs_dir_t *dir_p,
                struct fs_dir_entry_t *entry_p);

/**
 * Remove file by given path.
 *
 * @param[in] path_p The path of the file to remove.
 *
 * @return zero(0) or negative error code.
 */
int fs_remove(const char *path_p);

/**
 * Gets file status by path.
 *
 * @param[in] path_p The path of the file to stat.
 * @param[in] stat_p The stat struct to populate.
 *
 * @return zero(0) or negative error code.
 */
int fs_stat(const char *path_p, struct fs_stat_t *stat_p);

/**
 * Craete a directory with given path.
 *
 * @param[in] path_p The path of the directoy to create.
 *
 * @return zero(0) or negative error code.
 */
int fs_mkdir(const char *path_p);

/**
 * Format file system at given path.
 *
 * @param[in] path_p The path to the root of the file system to
 *                   format. All data in the file system will be
 *                   deleted.
 *
 * @return zero(0) or negative error code.
 */
int fs_format(const char *path_p);

/**
 * List files and folders in given path. Optionally
 * with given filter. The list is written to the output channel.
 *
 * @param[in] path_p Directory to list.
 * @param[in] filter_p Filter out files and folders.
 * @param[in] chout_p Output chan.
 *
 * @return zero(0) or negative error code.
 */
int fs_ls(const char *path_p,
          const char *filter_p,
          void *chout_p);

/**
 * List files (callbacks) and directories in given path. Optionally
 * with given filter. The list is written to the output channel.
 *
 * @param[in] path_p Directory to list.
 * @param[in] filter_p Filter out files and folders.
 * @param[in] chout_p Output chan.
 *
 * @return zero(0) or negative error code.
 */
int fs_list(const char *path_p,
            const char *filter_p,
            void *chout_p);

/**
 * Auto-complete given path.
 *
 * @param[in,out] path_p Absolute or relative path to auto-complete.
 *
 * @return >=1 if completion happened. Number of autocompleted characters
 *         added to the path.
 *         0 if no completion happend,
 *         or negative error code.
 */
int fs_auto_complete(char *path_p);

/**
 * Split buffer into path and command inplace.
 *
 * @param[in] buf_p Buffer to split.
 * @param[out] path_pp Path or NULL if no path was found.
 * @param[out] cmd_pp Command or empty string.
 *
 * @return zero(0) or negative error code.
 */
void fs_split(char *buf_p, char **path_pp, char **cmd_pp);

/**
 * Merge path and command previously split using `fs_split()`.
 *
 * @param[in] path_p Path from spilt.
 * @param[in] cmd_p Command from split.
 *
 * @return zero(0) or negative error code.
 */
void fs_merge(char *path_p, char *cmd_p);

/**
 * Initialize given generic file system.
 *
 * @param[in] self_p File system to initialize.
 * @param[in] name_p Path to register.
 * @param[in] ops_p File system function callbacks.
 *
 * @return zero(0) or negative error code.
 */
int fs_filesystem_init_generic(struct fs_filesystem_t *self_p,
                               const char *name_p,
                               struct fs_filesystem_operations_t *ops_p);

#if CONFIG_FAT16 == 1

/**
 * Initialize given FAT16 file system.
 *
 * @param[in] self_p File system to initialize.
 * @param[in] name_p Path to register.
 * @param[in] fat16_p File system pointer.
 *
 * @return zero(0) or negative error code.
 */
int fs_filesystem_init_fat16(struct fs_filesystem_t *self_p,
                             const char *name_p,
                             struct fat16_t *fat16_p);

#endif

#if CONFIG_SPIFFS == 1

/**
 * Initialize given SPIFFS file system.
 *
 * @param[in] self_p File system to initialize.
 * @param[in] name_p Path to register.
 * @param[in] spiffs_p File system pointer.
 * @param[in] config_p File system configuration.
 *
 * @return zero(0) or negative error code.
 */
int fs_filesystem_init_spiffs(struct fs_filesystem_t *self_p,
                              const char *name_p,
                              struct spiffs_t *spiffs_p,
                              struct fs_filesystem_spiffs_config_t *config_p);

#endif

/**
 * Register given file system. Use the functions `fs_open()`,
 * `fs_read()`, `fs_write()`, `fs_close()`, `fs_seek()`, fs_tell() and
 * `fs_read_line()` to access files in a registerd file system.
 *
 * @param[in] self_p File system to register.
 *
 * @return zero(0) or negative error code.
 */
int fs_filesystem_register(struct fs_filesystem_t *self_p);

/**
 * Deregister given file system.
 *
 * @param[in] self_p File system to deregister.
 *
 * @return zero(0) or negative error code.
 */
int fs_filesystem_deregister(struct fs_filesystem_t *self_p);

/**
 * Initialize given command.
 *
 * @param[in] self_p Command to initialize.
 * @param[in] path_p Path to register.
 * @param[in] callback Command callback function.
 * @param[in] arg_p Callback argument.
 *
 * @return zero(0) or negative error code.
 */
int fs_command_init(struct fs_command_t *self_p,
                    far_string_t path_p,
                    fs_callback_t callback,
                    void *arg_p);

/**
 * Register given command. Registered commands are called by the
 * function `fs_call()`.
 *
 * @param[in] command_p Command to register.
 *
 * @return zero(0) or negative error code.
 */
int fs_command_register(struct fs_command_t *command_p);

/**
 * Deregister given command.
 *
 * @param[in] command_p Command to deregister.
 *
 * @return zero(0) or negative error code.
 */
int fs_command_deregister(struct fs_command_t *command_p);

/**
 * Initialize given counter.
 *
 * @param[in] self_p Counter to initialize.
 * @param[in] path_p Path to register.
 * @param[in] value Initial value of the counter.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_init(struct fs_counter_t *self_p,
                    far_string_t path_p,
                    uint64_t value);

/**
 * Increment given counter.
 *
 * @param[in] self_p Command to initialize.
 * @param[in] value Increment value.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_increment(struct fs_counter_t *self_p,
                         uint64_t value);

/**
 * Register given counter.
 *
 * @param[in] counter_p Counter to register.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_register(struct fs_counter_t *counter_p);

/**
 * Deregister given counter.
 *
 * @param[in] counter_p Counter to deregister.
 *
 * @return zero(0) or negative error code.
 */
int fs_counter_deregister(struct fs_counter_t *counter_p);

/**
 * Initialize given parameter.
 *
 * @param[in] self_p Parameter to initialize.
 * @param[in] path_p Path to register.
 * @param[in] set_cb Callback function set set the parameter value.
 * @param[in] print_cb Callback function set print the parameter
 *                     value.
 * @param[in] value_p Value storage area.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_init(struct fs_parameter_t *self_p,
                      far_string_t path_p,
                      fs_parameter_set_callback_t set_cb,
                      fs_parameter_print_callback_t print_cb,
                      void *value_p);

/**
 * Register given parameter.
 *
 * @param[in] parameter_p Parameter to register.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_register(struct fs_parameter_t *parameter_p);

/**
 * Deregister given parameter.
 *
 * @param[in] parameter_p Parameter to deregister.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_deregister(struct fs_parameter_t *parameter_p);

/**
 * Integer parameter setter function callback
 *
 * @param[out] value_p Buffer the new value should be written to.
 * @param[in] src_p Value to set as a string.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_int_set(void *value_p, const char *src_p);

/**
 * Integer parameter printer function callback
 *
 * @param[in] chout_p Channel to write the formatted value to.
 * @param[in] value_p Value to format and print to the output channel.
 *
 * @return zero(0) or negative error code.
 */
int fs_parameter_int_print(void *chout_p, void *value_p);

#endif
