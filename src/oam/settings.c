/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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
    int32_t int32;
    char buf[32];
    size_t size;

    /* Print the header. */
    std_fprintf(chout_p,
                OSTR("NAME                  TYPE      SIZE  VALUE\r\n"));

    /* Print all settings. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        /* Print the name. */
        if (std_strlen(setting_p->name_p) >= sizeof(buf)) {
            continue;
        }

        std_strcpy(buf, setting_p->name_p);
        std_fprintf(chout_p, OSTR("%-20s  "), buf);
        size = setting_p->size;

        switch (setting_p->type) {

        case setting_type_int32_t:
            int32 = 0;
            settings_read(&int32, setting_p->address, size);
            std_fprintf(chout_p,
                        OSTR("int32_t      4  %ld\r\n"),
                        (long)int32);
            break;

        case setting_type_string_t:
            std_fprintf(chout_p, OSTR("string_t  %4u  "), (int)size);

            for (i = 0; i < size; i++) {
                settings_read(&buf[0], setting_p->address + i, 1);

                if (buf[0] == '\0') {
                    break;
                }

                std_fprintf(chout_p, OSTR("%c"), buf[0]);
            }

            std_fprintf(chout_p, OSTR("\r\n"));
            break;

#if CONFIG_SETTINGS_BLOB == 1

        case setting_type_blob_t:
            std_fprintf(chout_p, OSTR("blob_t    %4u  "), (int)size);

            for (i = 0; i < size; i++) {
                buf[0] = 0x00;
                settings_read(&buf[0], setting_p->address + i, 1);
                std_fprintf(chout_p, OSTR("\\x%02x"), buf[0]);
            }

            std_fprintf(chout_p, OSTR("\r\n"));
            break;

#endif

        default:
            std_fprintf(chout_p,
                        OSTR("bad setting type %d\r\n"),
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
    int32_t int32;
    char buf[1];

    if (argc != 2) {
        std_fprintf(chout_p, OSTR("Usage: read <name>\r\n"));

        return (-1);
    }

    /* Find the setting in the settings array. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        if (std_strcmp(argv[1], setting_p->name_p) == 0) {
            switch (setting_p->type) {

            case setting_type_int32_t:
                int32 = 0;
                settings_read(&int32, setting_p->address, setting_p->size);
                std_fprintf(chout_p, OSTR("%ld\r\n"), (long)int32);
                break;

            case setting_type_string_t:
                for (i = 0; i < setting_p->size; i++) {
                    buf[0] = '\0';
                    settings_read(&buf[0], setting_p->address + i, 1);

                    if (buf[0] == '\0') {
                        break;
                    }

                    std_fprintf(chout_p, OSTR("%c"), buf[0]);
                }

                std_fprintf(chout_p, OSTR("\r\n"));
                break;

#if CONFIG_SETTINGS_BLOB == 1

            case setting_type_blob_t:
                for (i = 0; i < setting_p->size; i++) {
                    buf[0] = 0;
                    settings_read(&buf[0], setting_p->address + i, 1);
                    std_fprintf(chout_p, OSTR("\\x%02x"), buf[0]);
                }

                std_fprintf(chout_p, OSTR("\r\n"));
                break;

#endif

            default:
                std_fprintf(chout_p,
                            OSTR("bad setting type %d\r\n"),
                            setting_p->type);
            }

            return (0);
        }

        setting_p++;
    }

    std_fprintf(chout_p, OSTR("%s: setting not found\r\n"), argv[1]);

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
    size_t size;
    long value;
    int32_t int32;
    char buf[5];
    char *buf_p;
    int i;

    if (argc != 3) {
        std_fprintf(chout_p, OSTR("Usage: write <name> <value>\r\n"));

        return (-1);
    }

    /* Find the setting in the settings array. */
    setting_p = &settings[0];

    while (setting_p->name_p != NULL) {
        if (std_strcmp(argv[1], setting_p->name_p) == 0) {
            switch (setting_p->type) {

            case setting_type_int32_t:
                if (std_strtol(argv[2], &value) == NULL) {
                    return (-1);
                }

                /* Range check. */
                if ((value > 2147483647) || (value < -2147483648)) {
                    std_fprintf(chout_p,
                                OSTR("%ld: value out of range\r\n"),
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
                                OSTR("%s: string too long\r\n"),
                                argv[2]);
                    return (-1);
                }

                settings_write(setting_p->address, argv[2], setting_p->size);
                break;

#if CONFIG_SETTINGS_BLOB == 1

            case setting_type_blob_t:
                /* Range check. */
                size = DIV_CEIL(strlen(argv[2]), 4);

                if (size != setting_p->size) {
                    std_fprintf(chout_p,
                                OSTR("%u: bad blob data length\r\n"),
                                size);
                    return (-1);
                }

                for (i = 0; i < 4 * size; i += 4) {
                    if ((argv[2][i] != '\\')
                        || (argv[2][i + 1] != 'x')
                        || !isxdigit((int)argv[2][i + 2])
                        || !isxdigit((int)argv[2][i + 3])) {
                        std_fprintf(chout_p, OSTR("bad blob data\r\n"));
                        return (-1);
                    }
                }

                /* For std_strtol(). */
                buf[0] = '0';
                buf[4] = '\0';

                /* argv pointers shall not be const in the furute. */
                buf_p = (char *)argv[2];

                for (i = 0; i < size; i++) {
                    memcpy(&buf[1], &argv[2][4 * i + 1], 3);
                    (void)std_strtol(&buf[0], &value);
                    *buf_p++ = value;
                }

                settings_write(setting_p->address, argv[2], size);
                break;

#endif

            default:
                std_fprintf(chout_p,
                            OSTR("bad setting type %d\r\n"),
                            setting_p->type);
            }

            return (0);
        }

        setting_p++;
    }

    std_fprintf(chout_p, OSTR("%s: setting not found\r\n"), argv[1]);

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
                    CSTR("/oam/settings/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

#if CONFIG_FS_CMD_SETTINGS_RESET == 1

    fs_command_init(&module.cmd_reset,
                    CSTR("/oam/settings/reset"),
                    cmd_reset_cb,
                    NULL);
    fs_command_register(&module.cmd_reset);

#endif

#if CONFIG_FS_CMD_SETTINGS_READ == 1

    fs_command_init(&module.cmd_read,
                    CSTR("/oam/settings/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&module.cmd_read);

#endif

#if CONFIG_FS_CMD_SETTINGS_WRITE == 1

    fs_command_init(&module.cmd_write,
                    CSTR("/oam/settings/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&module.cmd_write);

#endif

    return (settings_port_module_init());
}

ssize_t settings_read(void *dst_p, size_t src, size_t size)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (settings_port_read(dst_p, src, size));
}

ssize_t settings_write(size_t dst, const void *src_p, size_t size)
{
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
