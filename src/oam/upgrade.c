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

struct upgrade_binary_header_t {
    uint32_t size;
    uint8_t sha1[20];
    char description[128];
};

struct module_t {
    int initialized;
    uint8_t buf[256];
    ssize_t header_size;
    size_t offset;
    struct upgrade_binary_header_t header;
#if CONFIG_FS_CMD_UPGRADE_BOOTLOADER_ENTER == 1
    struct fs_command_t cmd_bootloader_enter;
#endif
#if CONFIG_FS_CMD_UPGRADE_APPLICATION_ENTER == 1
    struct fs_command_t cmd_application_enter;
#endif
#if CONFIG_FS_CMD_UPGRADE_APPLICATION_ERASE == 1
    struct fs_command_t cmd_application_erase;
#endif
#if CONFIG_FS_CMD_UPGRADE_APPLICATION_IS_VALID == 1
    struct fs_command_t cmd_application_is_valid;
#endif
};

static struct module_t module;

#include "upgrade.i"

static int binary_header_parse(struct upgrade_binary_header_t *header_p,
                               uint8_t *src_p,
                               size_t size)
{
    uint32_t version;
    uint32_t crc;

    version = ((src_p[0] << 24)
               | (src_p[1] << 16)
               | (src_p[2] << 8)
               | src_p[3]);

    if (version != 1) {
        return (-1);
    }

    crc = ((src_p[size - 4] << 24)
           | (src_p[size - 3] << 16)
           | (src_p[size - 2] << 8)
           | src_p[size - 1]);

    if (crc_32(0, src_p, size - 4) != crc) {
        return (-1);
    }

    header_p->size = ((src_p[8] << 24)
                      | (src_p[9] << 16)
                      | (src_p[10] << 8)
                      | src_p[11]);
    memcpy(&header_p->sha1[0], &src_p[12], sizeof(header_p->sha1));

    if (strlen((char *)&src_p[32]) >= sizeof(header_p->description)) {
        return (-1);
    }

    strcpy(&header_p->description[0], (char *)&src_p[32]);

    return (0);
}

#if CONFIG_FS_CMD_UPGRADE_BOOTLOADER_ENTER == 1

/**
 * File system command to enter the bootloader.
 */
static int cmd_bootloader_enter_cb(int argc,
                                   const char *argv[],
                                   void *out_p,
                                   void *in_p,
                                   void *arg_p,
                                   void *call_arg_p)
{
    return (upgrade_bootloader_enter());
}

#endif

#if CONFIG_FS_CMD_UPGRADE_APPLICATION_ENTER == 1

/**
 * File system command to enter the application.
 */
static int cmd_application_enter_cb(int argc,
                                    const char *argv[],
                                    void *out_p,
                                    void *in_p,
                                    void *arg_p,
                                    void *call_arg_p)
{
    return (upgrade_application_enter());
}

#endif

#if CONFIG_FS_CMD_UPGRADE_APPLICATION_ERASE == 1

/**
 * Shell command that erases the application from the flash memory.
 */
static int cmd_application_erase_cb(int argc,
                                    const char *argv[],
                                    void *out_p,
                                    void *in_p,
                                    void *arg_p,
                                    void *call_arg_p)
{
    return (upgrade_application_erase());
}

#endif

#if CONFIG_FS_CMD_UPGRADE_APPLICATION_IS_VALID == 1

/**
 * Check if the application is valid.
 */
static int cmd_application_is_valid_cb(int argc,
                                       const char *argv[],
                                       void *out_p,
                                       void *in_p,
                                       void *arg_p,
                                       void *call_arg_p)
{
    int res;

    res = upgrade_application_is_valid(0);

    if (res == 1) {
        std_fprintf(out_p, OSTR("yes\r\n"));
    } else if (res == 0) {
        std_fprintf(out_p, OSTR("no\r\n"));
    } else {
        std_fprintf(out_p, OSTR("failed\r\n"));
    }

    return (0);
}

#endif

int upgrade_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_UPGRADE_BOOTLOADER_ENTER == 1
    fs_command_init(&module.cmd_bootloader_enter,
                    CSTR("/oam/upgrade/bootloader/enter"),
                    cmd_bootloader_enter_cb,
                    NULL);
    fs_command_register(&module.cmd_bootloader_enter);
#endif

#if CONFIG_FS_CMD_UPGRADE_APPLICATION_ENTER == 1
    fs_command_init(&module.cmd_application_enter,
                    CSTR("/oam/upgrade/application/enter"),
                    cmd_application_enter_cb,
                    NULL);
    fs_command_register(&module.cmd_application_enter);
#endif

#if CONFIG_FS_CMD_UPGRADE_APPLICATION_ERASE == 1
    fs_command_init(&module.cmd_application_erase,
                    CSTR("/oam/upgrade/application/erase"),
                    cmd_application_erase_cb,
                    NULL);
    fs_command_register(&module.cmd_application_erase);
#endif

#if CONFIG_FS_CMD_UPGRADE_APPLICATION_IS_VALID == 1
    fs_command_init(&module.cmd_application_is_valid,
                    CSTR("/oam/upgrade/application/is_valid"),
                    cmd_application_is_valid_cb,
                    NULL);
    fs_command_register(&module.cmd_application_is_valid);
#endif

    return (0);
}

int upgrade_bootloader_enter()
{
    return (upgrade_port_bootloader_enter());
}

int upgrade_bootloader_stay_set()
{
    return (upgrade_port_bootloader_stay_set());
}

int upgrade_bootloader_stay_clear()
{
    return (upgrade_port_bootloader_stay_clear());
}

int upgrade_bootloader_stay_get()
{
    return (upgrade_port_bootloader_stay_get());
}

int upgrade_application_enter()
{
    return (upgrade_port_application_enter());
}

int upgrade_application_erase()
{
    return (upgrade_port_application_erase());
}

int upgrade_application_is_valid(int quick)
{
    return (upgrade_port_application_is_valid(quick));
}

int upgrade_binary_upload_begin()
{
    module.header_size = -1;
    module.offset = 0;

    return (upgrade_port_binary_upload_begin());
}

int upgrade_binary_upload(const void *buf_p,
                          size_t size)
{
    size_t chunk_size;

    /* Parse the header if not already parsed. */
    if (module.header_size != 0) {
        chunk_size = MIN(size, sizeof(module.buf) - module.offset);

        memcpy(&module.buf[module.offset], buf_p, chunk_size);
        module.offset += chunk_size;

        if (module.header_size == -1) {
            if (module.offset < 8) {
                return (0);
            }

            module.header_size = (ssize_t)((module.buf[4] << 24)
                                           | (module.buf[5] << 16)
                                           | (module.buf[6] << 8)
                                           | module.buf[7]);

            if ((module.header_size > sizeof(module.buf))
                || (module.header_size < 40)) {
                log_object_print(NULL,
                                 LOG_ERROR,
                                 OSTR("bad upgrade file header size %u\r\n"),
                                 module.header_size);
                return (-1);
            }
        }

        if (module.offset < module.header_size) {
            return (0);
        }

        if (binary_header_parse(&module.header,
                                (uint8_t *)&module.buf[0],
                                module.header_size) != 0) {
            log_object_print(NULL,
                             LOG_ERROR,
                             OSTR("failed to parse upgrade file header\r\n"));
            return (-1);
        }

        log_object_print(NULL,
                         LOG_INFO,
                         OSTR("parsed upgrade file header description '%s'"
                              " and data size %u\r\n"),
                         module.header.description,
                         module.header.size);

        chunk_size = (module.header_size - (module.offset - chunk_size));
        size -= chunk_size;
        buf_p += chunk_size;
        module.header_size = 0;

        if (size == 0) {
            return (0);
        }
    }

    return (upgrade_port_binary_upload(buf_p, size));
}

int upgrade_binary_upload_end()
{
    return (upgrade_port_binary_upload_end());
}
