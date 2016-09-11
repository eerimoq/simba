/**
 * @file fs.c
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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
#include <ctype.h>

#define FS_COMMAND_ARGS_MAX 16
#define FS_COMMAND_MAX 64

#define FS_NAME_MAX 64

struct module_t {
    int initialized;
    struct fs_command_t *commands_p;
    struct fs_filesystem_t *filesystems_p;
    struct fs_counter_t *counters_p;
    struct fs_parameter_t *parameters_p;
#if CONFIG_FS_CMD_FS_FILESYSTEMS_LIST == 1
    struct fs_command_t cmd_filesystems_list;
#endif
#if CONFIG_FS_CMD_FS_READ == 1
    struct fs_command_t cmd_read;
#endif
#if CONFIG_FS_CMD_FS_WRITE == 1
    struct fs_command_t cmd_write;
#endif
#if CONFIG_FS_CMD_FS_APPEND == 1
    struct fs_command_t cmd_append;
#endif
#if CONFIG_FS_CMD_FS_LIST == 1
    struct fs_command_t cmd_list;
#endif
#if CONFIG_FS_CMD_FS_COUNTERS_LIST == 1
    struct fs_command_t cmd_counters_list;
#endif
#if CONFIG_FS_CMD_FS_COUNTERS_RESET == 1
    struct fs_command_t cmd_counters_reset;
#endif
#if CONFIG_FS_CMD_FS_PARAMETERS_LIST == 1
    struct fs_command_t cmd_parameters_list;
#endif
};

static struct module_t module;
static char empty_path[] = "";

static int counter_get(struct fs_counter_t *counter_p,
                       void *chout_p)
{
    std_fprintf(chout_p,
                FSTR("%08lx%08lx\r\n"),
                (long)(counter_p->value >> 32),
                (long)(counter_p->value & 0xffffffff));

    return (0);
}

static int counter_set(struct fs_counter_t *counter_p)
{
    counter_p->value = 0;

    return (0);
}

static int cmd_parameter_cb(int argc,
                            const char *argv[],
                            void *chout_p,
                            void *chin_p,
                            void *arg_p,
                            void *call_arg_p)
{
    int res = -1;
    struct fs_parameter_t *parameter_p;

    parameter_p = arg_p;

    if (argc > 1) {
        res = parameter_p->set_cb(parameter_p->value_p, argv[1]);
    } else {
        res = parameter_p->print_cb(chout_p, parameter_p->value_p);
        std_fprintf(chout_p, FSTR("\r\n"));
    }

    return (res);
}

#if CONFIG_FS_CMD_FS_FILESYSTEMS_LIST == 1

static int cmd_filesystems_list_cb(int argc,
                                   const char *argv[],
                                   void *chout_p,
                                   void *chin_p,
                                   void *arg_p,
                                   void *call_arg_p)
{
    struct fs_filesystem_t *filesystem_p;
    char buf[FS_NAME_MAX];
    char *type_p;

    std_fprintf(chout_p,
                FSTR("MOUNT-POINT                    MEDIUM   TYPE     AVAILABLE  SIZE  USAGE\r\n"));

    filesystem_p = module.filesystems_p;

    while (filesystem_p != NULL) {
        strcpy(buf, filesystem_p->name_p);

        switch (filesystem_p->type) {
        case fs_type_fat16_t: type_p = "fat16"; break;
        case fs_type_spiffs_t: type_p = "spiffs"; break;
        default: type_p = "-"; break;
        }

        std_fprintf(chout_p,
                    FSTR("%-29s  %-7s  %-7s  %9s  %4s  %5s\r\n"),
                    buf,
                    "-",
                    type_p,
                    "-",
                    "-",
                    "-%");

        filesystem_p = filesystem_p->next_p;
    }

    return (0);
}

#endif

#if CONFIG_FS_CMD_FS_READ == 1

static int cmd_read_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct fs_file_t file;
    ssize_t size;
    char buf[32];

    if (argc != 2) {
        std_fprintf(chout_p, FSTR("Usage: %s <file>\r\n"), argv[0]);
        return (-1);
    }

    if (fs_open(&file, argv[1], FS_READ) != 0) {
        std_fprintf(chout_p, FSTR("Failed to open %s.\r\n"), argv[1]);
        return (-1);
    }

    while ((size = fs_read(&file, buf, sizeof(buf))) > 0) {
        chan_write(chout_p, buf, size);
    }

    std_fprintf(chout_p, FSTR("\r\n"));

    fs_close(&file);

    return (0);
}

#endif

#if CONFIG_FS_CMD_FS_WRITE == 1

static int cmd_write_cb(int argc,
                        const char *argv[],
                        void *chout_p,
                        void *chin_p,
                        void *arg_p,
                        void *call_arg_p)
{
    struct fs_file_t file;
    size_t size;

    if (argc != 3) {
        std_fprintf(chout_p, FSTR("Usage: %s <file> <data>\r\n"), argv[0]);
        return (-1);
    }

    if (fs_open(&file, argv[1], FS_CREAT | FS_TRUNC | FS_RDWR) != 0) {
        std_fprintf(chout_p, FSTR("Failed to open %s.\r\n"), argv[1]);
        return (-1);
    }

    size = strlen(argv[2]);

    if (fs_write(&file, argv[2], size) != size) {
        std_fprintf(chout_p, FSTR("Failed to write %s to the file.\r\n"), argv[2]);
        return (-1);
    }

    fs_close(&file);

    return (0);
}

#endif

#if CONFIG_FS_CMD_FS_APPEND == 1

static int cmd_append_cb(int argc,
                         const char *argv[],
                         void *chout_p,
                         void *chin_p,
                         void *arg_p,
                         void *call_arg_p)
{
    struct fs_file_t file;
    size_t size;

    if (argc != 3) {
        std_fprintf(chout_p, FSTR("Usage: %s <file> <data>\r\n"), argv[0]);
        return (-1);
    }

    if (fs_open(&file, argv[1], FS_RDWR | FS_APPEND) != 0) {
        std_fprintf(chout_p, FSTR("Failed to open %s.\r\n"), argv[1]);
        return (-1);
    }

    size = strlen(argv[2]);

    if (fs_write(&file, argv[2], size) != size) {
        std_fprintf(chout_p, FSTR("Failed to append %s to the file.\r\n"), argv[2]);
        return (-1);
    }

    fs_close(&file);

    return (0);
}

#endif

#if CONFIG_FS_CMD_FS_LIST == 1

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    if (argc != 2) {
        std_fprintf(chout_p, FSTR("Usage: %s <path>\r\n"), argv[0]);
        return (-1);
    }

    return (fs_ls(argv[1], NULL, chout_p));
}

#endif

#if CONFIG_FS_CMD_FS_COUNTERS_LIST == 1

static int cmd_counters_list_cb(int argc,
                                const char *argv[],
                                void *chout_p,
                                void *chin_p,
                                void *arg_p,
                                void *call_arg_p)
{
    struct fs_counter_t *counter_p;
    char buf[FS_NAME_MAX];

    std_fprintf(chout_p,
                FSTR("NAME                                                 VALUE\r\n"));

    counter_p = module.counters_p;

    while (counter_p != NULL) {
        std_strcpy(buf, counter_p->command.path_p);
        std_fprintf(chout_p, FSTR("%-53s"), buf);
        counter_p->command.callback(1,
                                    NULL,
                                    chout_p,
                                    NULL,
                                    counter_p->command.arg_p,
                                    arg_p);

        counter_p = counter_p->next_p;
    }

    return (0);
}

#endif

#if CONFIG_FS_CMD_FS_COUNTERS_RESET == 1

static int cmd_counters_reset_cb(int argc,
                                 const char *argv[],
                                 void *chout_p,
                                 void *chin_p,
                                 void *arg_p,
                                 void *call_arg_p)
{
    struct fs_counter_t *counter_p;

    counter_p = module.counters_p;

    while (counter_p != NULL) {
        counter_p->command.callback(0,
                                    NULL,
                                    chout_p,
                                    NULL,
                                    counter_p->command.arg_p,
                                    arg_p);

        counter_p = counter_p->next_p;
    }

    return (0);
}

#endif

#if CONFIG_FS_CMD_FS_PARAMETERS_LIST == 1

static int cmd_parameters_list_cb(int argc,
                                  const char *argv[],
                                  void *chout_p,
                                  void *chin_p,
                                  void *arg_p,
                                  void *call_arg_p)
{
    struct fs_parameter_t *parameter_p;
    char buf[FS_NAME_MAX];

    std_fprintf(chout_p,
                FSTR("NAME                                                 VALUE\r\n"));

    parameter_p = module.parameters_p;

    while (parameter_p != NULL) {
        std_strcpy(buf, parameter_p->command.path_p);
        std_fprintf(chout_p, FSTR("%-53s"), buf);
        parameter_p->command.callback(1,
                                      NULL,
                                      chout_p,
                                      NULL,
                                      parameter_p->command.arg_p,
                                      call_arg_p);

        parameter_p = parameter_p->next_p;
    }

    return (0);
}

#endif

/**
 * Parse one argument from given string. An argument must be in quotes
 * if it contains spaces.
 */
static char *argument_parse(char *command_p, const char **begin_pp)
{
    int in_quote;

    in_quote = 0;
    *begin_pp = command_p;

    while (*command_p != '\0') {
        if (*command_p == '\\') {
            if (command_p[1] == '\"') {
                /* Remove the \. */
                memmove(command_p, &command_p[1], strlen(&command_p[1]) + 1);
            }
        } else {
            if (in_quote == 1) {
                if (*command_p == '\"') {
                    /* Remove the ". */
                    memmove(command_p, &command_p[1], strlen(&command_p[1]) + 1);
                    in_quote = 0;
                    command_p--;
                }
            } else {
                if (*command_p == '\"') {
                    /* Remove the ". */
                    memmove(command_p, &command_p[1], strlen(&command_p[1]) + 1);
                    in_quote = 1;
                    command_p--;
                } else if (*command_p == ' ') {
                    *command_p = '\0';
                    command_p++;
                    break;
                }
            }
        }

        command_p++;
    }

    if (in_quote == 0) {
        return (command_p);
    } else {
        return (NULL);
    }
}

/**
 * Split given command into argv strings.
 */
static int command_parse(char *command_p, const char *argv[])
{
    int argc;

    /* Remove white spaces at the beginning and end of the string. */
    command_p = std_strip(command_p, NULL);
    argc = 0;

    /* Command string missing. */
    if (strlen(command_p) == 0) {
        return (-1);
    }

    while (*command_p != '\0') {
        /* Too many arguemnts? */
        if (argc == FS_COMMAND_ARGS_MAX) {
            return (-E2BIG);
        }

        /* Remove white spaces before the next argument. */
        command_p = std_strip(command_p, NULL);

        if ((command_p = argument_parse(command_p, &argv[argc++])) == NULL) {
            return (-1);
        }
    }

    return (argc);
}

static int cmd_counter_cb(int argc,
                          const char *argv[],
                          void *chout_p,
                          void *chin_p,
                          void *arg_p,
                          void *call_arg_p)
{
    if (argc == 1) {
        return (counter_get(arg_p, chout_p));
    } else {
        return (counter_set(arg_p));
    }
}

int fs_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;
    module.commands_p = NULL;
    module.filesystems_p = NULL;
    module.counters_p = NULL;
    module.parameters_p = NULL;

#if CONFIG_FS_CMD_FS_FILESYSTEMS_LIST == 1

    fs_command_init(&module.cmd_filesystems_list,
                    FSTR("/filesystems/fs/filesystems/list"),
                    cmd_filesystems_list_cb,
                    NULL);
    fs_command_register(&module.cmd_filesystems_list);

#endif

#if CONFIG_FS_CMD_FS_READ == 1

    fs_command_init(&module.cmd_read,
                    FSTR("/filesystems/fs/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&module.cmd_read);

#endif

#if CONFIG_FS_CMD_FS_WRITE == 1

    fs_command_init(&module.cmd_write,
                    FSTR("/filesystems/fs/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&module.cmd_write);

#endif

#if CONFIG_FS_CMD_FS_APPEND == 1

    fs_command_init(&module.cmd_append,
                    FSTR("/filesystems/fs/append"),
                    cmd_append_cb,
                    NULL);
    fs_command_register(&module.cmd_append);

#endif

#if CONFIG_FS_CMD_FS_LIST == 1

    fs_command_init(&module.cmd_list,
                    FSTR("/filesystems/fs/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

#if CONFIG_FS_CMD_FS_COUNTERS_LIST == 1

    fs_command_init(&module.cmd_counters_list,
                    FSTR("/filesystems/fs/counters/list"),
                    cmd_counters_list_cb,
                    NULL);
    fs_command_register(&module.cmd_counters_list);

#endif

#if CONFIG_FS_CMD_FS_COUNTERS_RESET == 1

    fs_command_init(&module.cmd_counters_reset,
                    FSTR("/filesystems/fs/counters/reset"),
                    cmd_counters_reset_cb,
                    NULL);
    fs_command_register(&module.cmd_counters_reset);

#endif

#if CONFIG_FS_CMD_FS_PARAMETERS_LIST == 1

    fs_command_init(&module.cmd_parameters_list,
                    FSTR("/filesystems/fs/parameters/list"),
                    cmd_parameters_list_cb,
                    NULL);
    fs_command_register(&module.cmd_parameters_list);

#endif

    return (0);
}

int fs_call(char *command_p,
            void *chin_p,
            void *chout_p,
            void *arg_p)
{
    ASSERTN(command_p != NULL, -EINVAL);

    int argc, skip_slash;
    const char *argv[FS_COMMAND_ARGS_MAX];
    struct fs_command_t *current_p;

    argc = command_parse(command_p, argv);

    if (argc <= 0) {
        return (argc);
    }

    /* Find given command. */
    current_p = module.commands_p;
    skip_slash = (argv[0][0] != '/');

    while (current_p != NULL) {
        if (std_strcmp(argv[0], &current_p->path_p[skip_slash]) == 0) {
            return (current_p->callback(argc,
                                        argv,
                                        chout_p,
                                        chin_p,
                                        current_p->arg_p,
                                        arg_p));
        }

        current_p = current_p->next_p;
    }

    std_fprintf(chout_p, FSTR("%s: command not found\r\n"), argv[0]);

    return (-ENOENT);
}

/**
 * Find the file system for given path.
 */
static int get_filesystem_path_from_path(struct fs_filesystem_t **filesystem_pp,
                                         const char **path_pp,
                                         const char *path_p)
{
    struct fs_filesystem_t *filesystem_p;
    const char *name_p;
    int name_length;

    /* Skip leading slash. */
    if (path_p[0] == '/') {
        path_p++;
    }

    filesystem_p = module.filesystems_p;

    /* Find the file system registered on given path. */
    while (filesystem_p != NULL) {
        name_p = filesystem_p->name_p;

        /* Skip leading slash. */
        if (name_p[0] == '/') {
            name_p++;
        }

        name_length = strlen(name_p);

        if (strncmp(name_p, path_p, name_length) == 0) {
            if (path_p[name_length] != '\0') {
                *path_pp = (path_p + name_length + 1);
            } else {
                *path_pp = "";
            }

            *filesystem_pp = filesystem_p;

            return (0);
        }

        filesystem_p = filesystem_p->next_p;
    }

    return (-1);
}

static int format_entry_fat16(void *chout_p,
                              const struct fat16_dir_entry_t *entry_p)
{
    /* Print modify date/time if requested */
    std_fprintf(chout_p,
                FSTR("%04u-%02u-%02u %02u:%02u "),
                entry_p->latest_mod_date.year,
                entry_p->latest_mod_date.month,
                entry_p->latest_mod_date.day,
                entry_p->latest_mod_date.hour,
                entry_p->latest_mod_date.minute);

    /* Print size if requested */
    std_fprintf(chout_p, FSTR("%8lu "), (unsigned long)entry_p->size);

    /* Print file name with possible blank fill */
    std_fprintf(chout_p, FSTR("%s"), entry_p->name);

    if (entry_p->is_dir == 1) {
        chan_write(chout_p, "/", 1);
    }

    std_fprintf(chout_p, FSTR("\r\n"));

    return (0);
}

/**
 * List all files in given fat16 directory.
 */
static int ls_fat16(struct fat16_t *fs_p,
                    const char *path_p,
                    void *chout_p)
{
    struct fat16_dir_t dir;
    struct fat16_dir_entry_t entry;

    /* Re-open the directory with read option set. */
    if (fat16_dir_open(fs_p, &dir, path_p, O_READ) != 0) {
        std_fprintf(chout_p, FSTR("%s: directory not found\r\n"), path_p);
        return (-1);
    }

    while (fat16_dir_read(&dir, &entry) == 1) {
        format_entry_fat16(chout_p, &entry);
    }

    fat16_dir_close(&dir);

    return (0);
}

#if CONFIG_SPIFFS == 1

static int format_entry_spiffs(void *chout_p,
                               const struct spiffs_dirent_t *entry_p)
{
    /* Print file name with possible blank fill */
    std_fprintf(chout_p,
                FSTR("xxxx-xx-xx xx-xx %8lu %s\r\n"),
                (unsigned long)entry_p->size,
                entry_p->name);

    return (0);
}

/**
 * List all files in given spiffs directory.
 */
static int ls_spiffs(struct spiffs_t *fs_p,
                     const char *path_p,
                     void *chout_p)
{
    struct spiffs_dir_t dir;
    struct spiffs_dirent_t entry;

    /* Open the directory. */
    if (spiffs_opendir(fs_p, path_p, &dir) == NULL) {
        std_fprintf(chout_p, FSTR("%s: directory not found\r\n"), path_p);
        return (-1);
    }

    while (spiffs_readdir(&dir, &entry) != NULL) {
        format_entry_spiffs(chout_p, &entry);
    }

    spiffs_closedir(&dir);

    return (0);
}

#endif

/**
 * Create an absolute path of given path by prepending the CWD
 * environment variable.
 */
static int create_absolute_path(char *dst_p,
                                const char *path_p)
{
    int size;
    const char *cwd_p;
    const char *slash_p;

    cwd_p = "";
    slash_p = "";

    /* Prepend CWD if given path is relative. */
    if (path_p[0] != '/') {
        cwd_p = thrd_get_env("CWD");

        if ((cwd_p == NULL) || (cwd_p[0] == '\0')) {
            cwd_p = "/";
        }

        if (cwd_p[strlen(cwd_p) - 1] != '/') {
            slash_p = "/";
        }
    }

    size = std_snprintf(dst_p,
                        CONFIG_FS_PATH_MAX,
                        FSTR("%s%s%s"),
                        cwd_p,
                        slash_p,
                        path_p);

    return (size < CONFIG_FS_PATH_MAX ? 0 : -1);
}

int fs_open(struct fs_file_t *self_p, const char *path_p, int flags)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(path_p != NULL, -EINVAL);

    struct fs_filesystem_t *filesystem_p;
    char path[CONFIG_FS_PATH_MAX];

    if (create_absolute_path(path, path_p) != 0) {
        return (-1);
    }

    if (get_filesystem_path_from_path(&filesystem_p, &path_p, &path[0]) != 0) {
        return (-1);
    }

    self_p->filesystem_p = filesystem_p;

    switch (filesystem_p->type) {

    case fs_type_fat16_t:
        return (fat16_file_open(filesystem_p->fs_p,
                                &self_p->u.fat16,
                                path_p,
                                flags));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        self_p->u.spiffs = spiffs_open(filesystem_p->fs_p, path_p, flags, 0);
        return (self_p->u.spiffs > 0 ? 0 : -1);

#endif

    default:
        return (-1);
    }
}

int fs_close(struct fs_file_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    switch (self_p->filesystem_p->type) {

    case fs_type_fat16_t:
        return (fat16_file_close(&self_p->u.fat16));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        return (spiffs_close(self_p->filesystem_p->fs_p, self_p->u.spiffs));

#endif

    default:
        return (-1);
    }
}

ssize_t fs_read(struct fs_file_t *self_p, void *dst_p, size_t size)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(dst_p != NULL, -EINVAL);

    ssize_t res;

    switch (self_p->filesystem_p->type) {

    case fs_type_fat16_t:
        res = fat16_file_read(&self_p->u.fat16, dst_p, size);

        return (res);

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        res = spiffs_read(self_p->filesystem_p->fs_p,
                          self_p->u.spiffs,
                          dst_p,
                          size);

        if (res == SPIFFS_ERR_END_OF_OBJECT) {
            return (0);
        } else {
            return (res);
        }
#endif

    default:
        return (-1);
    }
}

ssize_t fs_read_line(struct fs_file_t *self_p, void *dst_p, size_t size)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(dst_p != NULL, -EINVAL);

    ssize_t i;
    char *d_p;

    d_p = dst_p;
    i = 0;

    /* Read one byte at a time until a newline is found, the
       destination buffer is full, or end of file is reached. */
    while (i < size) {
        if (fs_read(self_p, &d_p[i], 1) != 1) {
            d_p[i] = '\0';
            return (i > 0 ? i : -1);
        }

        if (d_p[i] == '\n') {
            d_p[i] = '\0';
            return (i);
        }

        i++;
    }

    return (i == size ? size : -1);
}

ssize_t fs_write(struct fs_file_t *self_p, const void *src_p, size_t size)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(src_p != NULL, -EINVAL);

    switch (self_p->filesystem_p->type) {

    case fs_type_fat16_t:
        return (fat16_file_write(&self_p->u.fat16, src_p, size));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        return (spiffs_write(self_p->filesystem_p->fs_p, self_p->u.spiffs, (void *)src_p, size));

#endif

    default:
        return (-1);
    }
}

int fs_seek(struct fs_file_t *self_p, int offset, int whence)
{
    ASSERTN(self_p != NULL, -EINVAL);

    switch (self_p->filesystem_p->type) {

    case fs_type_fat16_t:
        return (fat16_file_seek(&self_p->u.fat16, offset, whence));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        if (spiffs_lseek(self_p->filesystem_p->fs_p,
                         self_p->u.spiffs,
                         offset,
                         whence) >= 0) {
            return (0);
        } else {
            return (-1);
        }

#endif

    default:
        return (-1);
    }
}

ssize_t fs_tell(struct fs_file_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);

    switch (self_p->filesystem_p->type) {

    case fs_type_fat16_t:
        return (fat16_file_tell(&self_p->u.fat16));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        return (spiffs_tell(self_p->filesystem_p->fs_p, self_p->u.spiffs));

#endif

    default:
        return (-1);
    }
}

int fs_mkdir(const char *path_p)
{
    ASSERTN(path_p != NULL, -EINVAL);

    struct fs_filesystem_t *filesystem_p;
    char path[CONFIG_FS_PATH_MAX];
    struct fat16_dir_t dir;

    if (create_absolute_path(path, path_p) != 0) {
        return (-1);
    }

    if (get_filesystem_path_from_path(&filesystem_p, &path_p, &path[0]) != 0) {
        return (-1);
    }

    switch (filesystem_p->type) {

    case fs_type_fat16_t:
        if (fat16_dir_open(filesystem_p->fs_p,
                           &dir,
                           path_p,
                           O_CREAT | O_WRITE | O_SYNC) != 0) {
            return (-1);
        }

        fat16_dir_close(&dir);

        return (0);

    default:
        return (-1);
    }
}

int fs_dir_open(struct fs_dir_t *dir_p,
                const char *path_p,
                int oflag)
{
    ASSERTN(dir_p != NULL, -EINVAL);
    ASSERTN(path_p != NULL, -EINVAL);

    struct fs_filesystem_t *filesystem_p;
    char path[CONFIG_FS_PATH_MAX];

    if (create_absolute_path(path, path_p) != 0) {
        return (-1);
    }

    if (get_filesystem_path_from_path(&filesystem_p, &path_p, &path[0]) != 0) {
        return (-1);
    }

    if (*path_p == '\0') {
        path_p = ".";
    }

    dir_p->filesystem_p = filesystem_p;

    switch (filesystem_p->type) {

    case fs_type_fat16_t:
        return (fat16_dir_open(filesystem_p->fs_p, &dir_p->u.fat16, path_p, O_READ));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        return (spiffs_opendir(filesystem_p->fs_p, path_p, &dir_p->u.spiffs) != NULL
                ? 0
                : -1);

#endif

    default:
        return (-1);
    }
}

int fs_dir_close(struct fs_dir_t *dir_p)
{
    ASSERTN(dir_p != NULL, -EINVAL);

    switch (dir_p->filesystem_p->type) {

    case fs_type_fat16_t:
        return (fat16_dir_close(&dir_p->u.fat16));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        return (spiffs_closedir(&dir_p->u.spiffs));

#endif

    default:
        return (-1);
    }
}

int fs_dir_read(struct fs_dir_t *dir_p,
                struct fs_dir_entry_t *entry_p)
{
    ASSERTN(dir_p != NULL, -EINVAL);

    int res;
    
    switch (dir_p->filesystem_p->type) {

    case fs_type_fat16_t:
        {
            struct fat16_dir_entry_t entry;
            
            res = fat16_dir_read(&dir_p->u.fat16, &entry);
            strncpy(&entry_p->name[0],
                    &entry.name[0],
                    membersof(entry_p->name));
            entry_p->name[membersof(entry_p->name) - 1] = '\0';
            entry_p->type = (entry.is_dir == 1 ? FS_TYPE_DIR : FS_TYPE_FILE);
            entry_p->size = entry.size;
            entry_p->latest_mod_date = entry.latest_mod_date;
        }
        break;

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        {
            struct spiffs_dirent_t entry;
            
            if (spiffs_readdir(&dir_p->u.spiffs, &entry) != NULL) {
                res = 1;
                strncpy(&entry_p->name[0],
                        (const char *)&entry.name[0],
                        membersof(entry_p->name));
                entry_p->name[membersof(entry_p->name) - 1] = '\0';
                entry_p->type = entry.type;
                entry_p->size = entry.size;
            } else {
                res = 0;
            }
        }
        break;

#endif

    default:
        res = -1;
    }

    return (res);
}

int fs_stat(const char *path_p, struct fs_stat_t *stat_p)
{
    ASSERTN(path_p != NULL, -EINVAL);
    ASSERTN(stat_p != NULL, -EINVAL);

    struct fs_filesystem_t *filesystem_p;
    char path[CONFIG_FS_PATH_MAX];

    if (create_absolute_path(path, path_p) != 0) {
        return (-1);
    }

    if (get_filesystem_path_from_path(&filesystem_p, &path_p, &path[0]) != 0) {
        return (-1);
    }

    switch (filesystem_p->type) {

    case fs_type_fat16_t:
        {
            struct fat16_stat_t stat;
            
            if (fat16_stat(filesystem_p->fs_p, path_p, &stat) != 0) {
                return (-1);
            }

            stat_p->size = stat.size;
            stat_p->type = (stat.is_dir == 1 ? FS_TYPE_DIR : FS_TYPE_FILE);

            return (0);
        }

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        {
            struct spiffs_stat_t stat;

            if (spiffs_stat(filesystem_p->fs_p, path_p, &stat) != 0) {
                return (-1);
            }

            stat_p->size = stat.size;
            stat_p->type = stat.type;

            return (0);
        }
#endif

    default:
        return (-1);
    }
}

int fs_ls(const char *path_p,
          const char *filter_p,
          void *chout_p)
{
    ASSERTN(path_p != NULL, -EINVAL);
    ASSERTN(chout_p != NULL, -EINVAL);

    struct fs_filesystem_t *filesystem_p;
    char path[CONFIG_FS_PATH_MAX];

    if (create_absolute_path(path, path_p) != 0) {
        return (-1);
    }

    if (get_filesystem_path_from_path(&filesystem_p, &path_p, &path[0]) != 0) {
        return (-1);
    }

    if (*path_p == '\0') {
        path_p = ".";
    }

    switch (filesystem_p->type) {

    case fs_type_fat16_t:
        return (ls_fat16(filesystem_p->fs_p, path_p, chout_p));

#if CONFIG_SPIFFS == 1

    case fs_type_spiffs_t:
        return (ls_spiffs(filesystem_p->fs_p, path_p, chout_p));

#endif

    default:
        return (-1);
    }
}

int fs_list(const char *path_p,
            const char *filter_p,
            void *chout_p)
{
    ASSERTN(path_p != NULL, -EINVAL);
    ASSERTN(chout_p != NULL, -EINVAL);

    int buf_length, path_offset, filter_offset, path_length;
    struct fs_command_t *command_p;
    char buf[64], next_char;

    filter_offset = path_length = strlen(path_p);
    path_offset = 0;

    if (path_p[0] != '/') {
        path_offset = 1;
        filter_offset++;
    }

    if (path_length > 0) {
        if (path_p[path_length - 1] == '/') {
            path_length--;
        } else {
            filter_offset++;
        }
    }

    buf_length = -1;

    /* Find all paths matching given path and filter and output the
       file or folder matching the filter. */
    command_p = module.commands_p;

    while (command_p != NULL) {
        /* Path match? */
        if (std_strncmp(&command_p->path_p[path_offset],
                        path_p,
                        path_length) == 0) {
            /* Filter match? */
            if ((filter_p == NULL)
                || (std_strncmp(&command_p->path_p[filter_offset],
                                filter_p,
                                strlen(filter_p)) == 0)) {
                /* Output new files and folders. */
                if ((buf_length == -1)
                    || (std_strncmp(&command_p->path_p[filter_offset],
                                    buf,
                                    buf_length) != 0)) {
                    /* Get file or folder name. */
                    buf_length = 0;

                    do {
                        next_char = command_p->path_p[filter_offset + buf_length];

                        buf[buf_length] = next_char;
                        buf_length++;
                    } while ((next_char != '\0') && (next_char != '/'));

                    buf[buf_length] = '\0';

                    std_fprintf(chout_p, FSTR("%s\r\n"), buf);
                }
            }
        }

        command_p = command_p->next_p;
    }

    return (0);
}

int fs_auto_complete(char *path_p)
{
    ASSERTN(path_p != NULL, -EINVAL);

    char next_char;
    int mismatch, path_length, offset, size;
    struct fs_command_t *command_p, *next_p;

    offset = 0;
    size = path_length = strlen(path_p);

    /* Skip the leading slash in the registered commands if it is
       missing in the input path. */
    if (path_p[0] != '/') {
        offset = 1;
    }

    /* Find the first command matching given path. */
    command_p = module.commands_p;

    while (command_p != NULL) {
        if (std_strncmp(&command_p->path_p[offset],
                        path_p,
                        size) == 0) {
            break;
        }

        command_p = command_p->next_p;
    }

    /* No command matching the path. */
    if (command_p == NULL) {
        return (-ENOENT);
    }

    /* Auto-complete given path. Compare the next character each
       iteration.

       Example:
       path_p = "/tm"
       commands = ["/tmp/foo", "/tmp/bar", "/zoo/lander"]
       auto-completed = "/tmp/"
    */
    while (1) {
        mismatch = 0;
        next_char = command_p->path_p[offset + size];

        /* It's a match if all commands matching path has the same
           next character. */
        next_p = command_p;

        while ((next_p != NULL)
               && (std_strncmp(&next_p->path_p[offset],
                               path_p,
                               size) == 0)) {
            if (next_p->path_p[offset + size] != next_char) {
                mismatch = 1;
                break;
            }

            next_p = next_p->next_p;
        }

        /* Completion happend? */
        if (mismatch == 0) {
            path_p[size] = next_char;
            size++;

            /* Auto-complete one directory at a time. */
            if (next_char == '/') {
                break;
            } else if (next_char == '\0') {
                /* Append a space on commands. */
                path_p[size - 1] = ' ';
                path_p[size] = '\0';
                break;
            }
        } else {
            break;
        }
    }

    path_p[size] = '\0';

    return (size - path_length);
}

void fs_split(char *buf_p, char **path_pp, char **cmd_pp)
{
    ASSERTN(buf_p != NULL, -EINVAL);
    ASSERTN(path_pp != NULL, -EINVAL);
    ASSERTN(cmd_pp != NULL, -EINVAL);

    char *last_slash_p = NULL;

    *path_pp = buf_p;

    /* Find the last slash. */
    while (*buf_p != '\0') {
        if (*buf_p == '/') {
            last_slash_p = buf_p;
        }

        buf_p++;
    }

    if (last_slash_p != NULL) {
        *last_slash_p = '\0';
        *cmd_pp = (last_slash_p + 1);
    } else {
        *cmd_pp = *path_pp;
        *path_pp = empty_path;
    }
}

void fs_merge(char *path_p, char *cmd_p)
{
    ASSERTN(path_p != NULL, -EINVAL);
    ASSERTN(cmd_p != NULL, -EINVAL);

    if (path_p != empty_path) {
        cmd_p[-1] = '/';
    }
}

int fs_filesystem_init(struct fs_filesystem_t *self_p,
                       const char *name_p,
                       enum fs_type_t type,
                       void *fs_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(name_p != NULL, -EINVAL);
    ASSERTN((type >= fs_type_fat16_t) && (type <= fs_type_spiffs_t), -EINVAL);
    ASSERTN(fs_p != NULL, -EINVAL);

    self_p->name_p = name_p;
    self_p->type = type;
    self_p->fs_p = fs_p;

    return (0);
}

int fs_filesystem_register(struct fs_filesystem_t *filesystem_p)
{
    ASSERTN(filesystem_p != NULL, -EINVAL);

    filesystem_p->next_p = module.filesystems_p;
    module.filesystems_p = filesystem_p;

    return (0);
}

int fs_filesystem_deregister(struct fs_filesystem_t *self_p)
{
    ASSERTN(0, -ENOSYS);

    return (-1);
}

int fs_command_init(struct fs_command_t *self_p,
                    const FAR char *path_p,
                    fs_callback_t callback,
                    void *arg_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(path_p != NULL, -EINVAL);
    ASSERTN(callback != NULL, -EINVAL);

    self_p->next_p = NULL;
    self_p->path_p = path_p;
    self_p->callback = callback;
    self_p->arg_p = arg_p;

    return (0);
}

int fs_command_register(struct fs_command_t *command_p)
{
    ASSERTN(command_p != NULL, -EINVAL);

    struct fs_command_t *current_p, *prev_p;

    /* Insert in alphabetical order. */
    prev_p = NULL;
    current_p = module.commands_p;

    while (current_p != NULL) {
        if (std_strcmp_f(command_p->path_p, current_p->path_p) < 0) {
            break;
        }

        prev_p = current_p;
        current_p = current_p->next_p;
    }

    /* Insert into the list. */
    command_p->next_p = current_p;

    if (prev_p == NULL) {
        module.commands_p = command_p;
    } else {
        prev_p->next_p = command_p;
    }

    return (0);
}

int fs_command_deregister(struct fs_command_t *command_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

int fs_counter_init(struct fs_counter_t *self_p,
                    const FAR char *path_p,
                    uint64_t value)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(path_p != NULL, -EINVAL);

    fs_command_init(&self_p->command,
                    path_p,
                    cmd_counter_cb,
                    self_p);

    self_p->value = value;
    self_p->next_p = NULL;

    return (0);
}

int fs_counter_increment(struct fs_counter_t *self_p,
                         uint64_t value)
{
    ASSERTN(self_p != NULL, -EINVAL);

    self_p->value += value;

    return (0);
}

int fs_counter_register(struct fs_counter_t *counter_p)
{
    ASSERTN(counter_p != NULL, -EINVAL);

    /* Insert counter into the command list and the counter list. */
    fs_command_register(&counter_p->command);

    counter_p->next_p = module.counters_p;
    module.counters_p = counter_p;

    return (0);
}

int fs_counter_deregister(struct fs_counter_t *counter_p)
{
    ASSERTN(0, ENOSYS);

    return (0);
}

int fs_parameter_init(struct fs_parameter_t *self_p,
                      const FAR char *path_p,
                      fs_parameter_set_callback_t set_cb,
                      fs_parameter_print_callback_t print_cb,
                      void *value_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(path_p != NULL, -EINVAL);
    ASSERTN(set_cb != NULL, -EINVAL);
    ASSERTN(print_cb != NULL, -EINVAL);
    ASSERTN(value_p != NULL, -EINVAL);

    fs_command_init(&self_p->command,
                    path_p,
                    cmd_parameter_cb,
                    self_p);

    self_p->set_cb = set_cb;
    self_p->print_cb = print_cb;
    self_p->value_p = value_p;
    self_p->next_p = NULL;

    return (0);
}

int fs_parameter_register(struct fs_parameter_t *parameter_p)
{
    ASSERTN(parameter_p != NULL, -EINVAL);

    /* Insert counter into the command list and the counter list. */
    fs_command_register(&parameter_p->command);

    parameter_p->next_p = module.parameters_p;
    module.parameters_p = parameter_p;

    return (0);
}

int fs_parameter_deregister(struct fs_parameter_t *parameter_p)
{
    ASSERTN(0, ENOSYS);

    return (0);
}

int fs_remove(void)
{
    return (-1);
}

int fs_parameter_int_set(void *value_p, const char *src_p)
{
    long value;

    if (std_strtol(src_p, &value) == NULL) {
        return (-1);
    }

    *(int *)value_p = value;

    return (0);
}

int fs_parameter_int_print(void *chout_p, void *value_p)
{
    std_fprintf(chout_p, FSTR("%d"), *(int *)value_p);

    return (0);
}
