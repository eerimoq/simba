/**
 * @file settings.c
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

struct module_t {
    int initialized;
#if CONFIG_FS_CMD_SETTINGS_LIST == 1
    struct fs_command_t cmd_list;
#endif
#if CONFIG_FS_CMD_SETTINGS_RESET == 1
    struct fs_command_t cmd_reset;
#endif
#if CONFIG_FS_CMD_SETTINGS_READ == 1
    struct fs_command_t cmd_read;
#endif
#if CONFIG_FS_CMD_SETTINGS_WRITE == 1
    struct fs_command_t cmd_write;
#endif
};

#include "settings_port.i"

static struct module_t module;

extern const FAR struct setting_t settings[];

#if CONFIG_FS_CMD_SETTINGS_LIST == 1

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    const FAR struct setting_t *setting_p;
    int i;
    int8_t int8;
    int16_t int16;
    int32_t int32;
    char buf[32];
    size_t size;

    /* Print the header. */
    std_fprintf(chout_p,
                FSTR("NAME                  TYPE     SIZE  VALUE\r\n"));

    /* Print all settings. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        /* Print the name. */
        if (std_strlen(setting_p->name_p) >= sizeof(buf)) {
            continue;
        }

        std_strcpy(buf, setting_p->name_p);
        std_fprintf(chout_p, FSTR("%-20s  "), buf);
        size = setting_p->size;

        switch (setting_p->type) {

        case setting_type_int8_t:
            settings_read(&int8, setting_p->address, size);
            std_fprintf(chout_p,
                        FSTR("int8_t      1  %d\r\n"),
                        (int)int8);
            break;

        case setting_type_int16_t:
            settings_read(&int16, setting_p->address, size);
            std_fprintf(chout_p,
                        FSTR("int16_t     2  %d\r\n"),
                        (int)int16);
            break;

        case setting_type_int32_t:
            settings_read(&int32, setting_p->address, size);
            std_fprintf(chout_p,
                        FSTR("int32_t     4  %ld\r\n"),
                        (long)int32);
            break;

        case setting_type_string_t:
            std_fprintf(chout_p, FSTR("string   %4u  "), (int)size);

            for (i = 0; i < size; i++) {
                settings_read(&buf[0], setting_p->address + i, 1);

                if (buf[0] == '\0') {
                    break;
                }

                std_fprintf(chout_p, FSTR("%c"), buf[0]);
            }

            std_fprintf(chout_p, FSTR("\r\n"));
            break;

        default:
            std_fprintf(chout_p,
                        FSTR("bad setting type %d\r\n"),
                        setting_p->type);
        }

        setting_p++;
    }

    return (0);
}

#endif

#if CONFIG_FS_CMD_SETTINGS_RESET == 1

static int cmd_reset_cb(int argc,
                        const char *argv[],
                        void *chout_p,
                        void *chin_p,
                        void *arg_p,
                        void *call_arg_p)
{
    return (settings_reset());
}

#endif

#if CONFIG_FS_CMD_SETTINGS_READ == 1

static int cmd_read_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    const FAR struct setting_t *setting_p;
    int i;
    int8_t int8;
    int16_t int16;
    int32_t int32;
    char buf[1];

    if (argc != 2) {
        std_fprintf(chout_p, FSTR("Usage: %s <name>\r\n"), argv[0]);

        return (-1);
    }

    /* Find the setting in the settings array. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        if (std_strcmp(argv[1], setting_p->name_p) == 0) {
            switch (setting_p->type) {

            case setting_type_int8_t:
                settings_read(&int8, setting_p->address, setting_p->size);
                std_fprintf(chout_p, FSTR("%d\r\n"), (int)int8);
                break;

            case setting_type_int16_t:
                settings_read(&int16, setting_p->address, setting_p->size);
                std_fprintf(chout_p, FSTR("%d\r\n"), (int)int16);
                break;

            case setting_type_int32_t:
                settings_read(&int32, setting_p->address, setting_p->size);
                std_fprintf(chout_p, FSTR("%ld\r\n"), (long)int32);
                break;

            case setting_type_string_t:
                for (i = 0; i < setting_p->size; i++) {
                    settings_read(&buf[0], setting_p->address + i, 1);

                    if (buf[0] == '\0') {
                        break;
                    }

                    std_fprintf(chout_p, FSTR("%c"), buf[0]);
                }

                std_fprintf(chout_p, FSTR("\r\n"));
                break;

            default:
                std_fprintf(chout_p,
                            FSTR("bad setting type %d\r\n"),
                            setting_p->type);
            }

            return (0);
        }

        setting_p++;
    }

    std_fprintf(chout_p, FSTR("%s: setting not found\r\n"), argv[1]);

    return (-1);
}

#endif

#if CONFIG_FS_CMD_SETTINGS_WRITE == 1

static int cmd_write_cb(int argc,
                        const char *argv[],
                        void *chout_p,
                        void *chin_p,
                        void *arg_p,
                        void *call_arg_p)
{
    const FAR struct setting_t *setting_p;
    long value;
    int8_t int8;
    int16_t int16;
    int32_t int32;

    if (argc != 3) {
        std_fprintf(chout_p, FSTR("Usage: %s <name> <value>\r\n"), argv[0]);

        return (-1);
    }

    /* Find the setting in the settings array. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        if (std_strcmp(argv[1], setting_p->name_p) == 0) {
            switch (setting_p->type) {

            case setting_type_int8_t:
                if (std_strtol(argv[2], &value) == NULL) {
                    return (-1);
                }

                /* Range check. */
                if ((value > 127) || (value < -128)) {
                    std_fprintf(chout_p,
                                FSTR("%ld: value out of range\r\n"),
                                value);
                    return (-1);
                }

                int8 = (int8_t)value;
                settings_write(setting_p->address, &int8, setting_p->size);
                break;

            case setting_type_int16_t:
                if (std_strtol(argv[2], &value) == NULL) {
                    return (-1);
                }

                /* Range check. */
                if ((value > 32767) || (value < -32768)) {
                    std_fprintf(chout_p,
                                FSTR("%ld: value out of range\r\n"),
                                value);
                    return (-1);
                }

                int16 = (int16_t)value;
                settings_write(setting_p->address, &int16, setting_p->size);
                break;

            case setting_type_int32_t:
                if (std_strtol(argv[2], &value) == NULL) {
                    return (-1);
                }

                /* Range check. */
                if ((value > 2147483647) || (value < -2147483648)) {
                    std_fprintf(chout_p,
                                FSTR("%ld: value out of range\r\n"),
                                value);
                    return (-1);
                }

                int32 = (int32_t)value;
                settings_write(setting_p->address, &int32, setting_p->size);
                break;

            case setting_type_string_t:
                /* Range check. */
                if (strlen(argv[2]) >= setting_p->size) {
                    std_fprintf(chout_p,
                                FSTR("%s: string too long\r\n"),
                                argv[2]);
                    return (-1);
                }

                settings_write(setting_p->address, argv[2], setting_p->size);
                break;

            default:
                std_fprintf(chout_p,
                            FSTR("bad setting type %d\r\n"),
                            setting_p->type);
            }

            return (0);
        }

        setting_p++;
    }

    std_fprintf(chout_p, FSTR("%s: setting not found\r\n"), argv[1]);

    return (-1);
}

#endif

int settings_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_SETTINGS_LIST == 1

    fs_command_init(&module.cmd_list,
                    FSTR("/oam/settings/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

#if CONFIG_FS_CMD_SETTINGS_RESET == 1

    fs_command_init(&module.cmd_reset,
                    FSTR("/oam/settings/reset"),
                    cmd_reset_cb,
                    NULL);
    fs_command_register(&module.cmd_reset);

#endif

#if CONFIG_FS_CMD_SETTINGS_READ == 1

    fs_command_init(&module.cmd_read,
                    FSTR("/oam/settings/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&module.cmd_read);

#endif

#if CONFIG_FS_CMD_SETTINGS_WRITE == 1

    fs_command_init(&module.cmd_write,
                    FSTR("/oam/settings/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&module.cmd_write);

#endif

    return (settings_port_module_init());
}

ssize_t settings_read(void *dst_p, size_t src, size_t size)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(src >= 0, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (settings_port_read(dst_p, src, size));
}

ssize_t settings_write(size_t dst, const void *src_p, size_t size)
{
    ASSERTN(dst >= 0, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (settings_port_write(dst, src_p, size));
}

ssize_t settings_read_by_name(const char *name_p,
                              void *dst_p,
                              size_t size)
{
    ASSERTN(name_p != NULL, EINVAL);
    ASSERTN(dst_p  != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    const FAR struct setting_t *setting_p;

    /* Find the setting in the settings array. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        if (std_strcmp(name_p, setting_p->name_p) == 0) {
            if (size > setting_p->size) {
                return (-1);
            }

            return (settings_read(dst_p, setting_p->address, size));
        }

        setting_p++;
    }

    return (-1);
}

ssize_t settings_write_by_name(const char *name_p,
                               const void *src_p,
                               size_t size)
{
    ASSERTN(name_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    const FAR struct setting_t *setting_p;

    /* Find the setting in the settings array. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        if (std_strcmp(name_p, setting_p->name_p) == 0) {
            if (size > setting_p->size) {
                return (-1);
            }

            return (settings_write(setting_p->address, src_p, size));
        }

        setting_p++;
    }

    return (-1);
}

int settings_reset()
{
    return (settings_port_reset());
}
