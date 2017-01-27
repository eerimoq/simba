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
    uint8_t buf[256];
    ssize_t header_size;
    size_t offset;
    struct upgrade_binary_header_t header;
    struct fs_command_t cmd_application_erase;
    struct fs_command_t cmd_application_is_valid;
};

static struct module_t module;

#if defined(ARCH_ESP32)
#    include "application.esp32.i"
#elif defined(ARCH_LINUX)
#    include "application.linux.i"
#else
#    error "Unsupported port."
#endif

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
    return (upgrade_bootloader_application_erase());
}

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

    res = upgrade_bootloader_application_is_valid();

    if (res == 1) {
        std_fprintf(out_p, FSTR("yes\r\n"));
    } else if (res == 0) {
        std_fprintf(out_p, FSTR("no\r\n"));
    } else {
        std_fprintf(out_p, FSTR("failed\r\n"));
    }
    
    return (0);
}

int upgrade_bootloader_application_module_init()
{
    fs_command_init(&module.cmd_application_erase,
                    FSTR("/oam/upgrade/application/erase"),
                    cmd_application_erase_cb,
                    NULL);
    fs_command_register(&module.cmd_application_erase);

    fs_command_init(&module.cmd_application_is_valid,
                    FSTR("/oam/upgrade/application/is_valid"),
                    cmd_application_is_valid_cb,
                    NULL);
    fs_command_register(&module.cmd_application_is_valid);

    return (0);
}

int upgrade_bootloader_application_erase()
{
    return (upgrade_bootloader_application_port_erase());
}

int upgrade_bootloader_application_write_begin()
{
    module.header_size = -1;
    module.offset = 0;
    
    return (upgrade_bootloader_application_port_write_begin());
}

int upgrade_bootloader_application_write_chunk(const void *buf_p,
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

            module.header_size = ((module.buf[4] << 24)
                                  | (module.buf[5] << 16)
                                  | (module.buf[6] << 8)
                                  | module.buf[7]);

            if (module.header_size > sizeof(module.buf)) {
                return (-1);
            }
        }

        if (module.offset < module.header_size) {
            return (0);
        }

        if (upgrade_binary_header_parse(&module.header,
                                        (uint8_t *)&module.buf[0],
                                        module.header_size) != 0) {
            return (-1);
        }

        chunk_size = (module.header_size - (module.offset - chunk_size));
        size -= chunk_size;
        buf_p += chunk_size;
        module.header_size = 0;
        
        if (size == 0) {
            return (0);
        }
    }

    return (upgrade_bootloader_application_port_write_chunk(buf_p, size));
}

int upgrade_bootloader_application_write_end()
{
    return (upgrade_bootloader_application_port_write_end());
}

int upgrade_bootloader_application_is_valid()
{
    return (upgrade_bootloader_application_port_is_valid());
}
