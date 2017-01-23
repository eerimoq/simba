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

#if defined(ARCH_ESP32)
#    include "application_esp32.i"
#elif defined(ARCH_LINUX)
#    include "application_linux.i"
#else
#    error "Unsupported port."
#endif

static struct fs_command_t cmd_application_erase;
static struct fs_command_t cmd_application_sha1;

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
 * Compute the SHA1 hash of the application area.
 */
static int cmd_application_sha1_cb(int argc,
                                   const char *argv[],
                                   void *out_p,
                                   void *in_p,
                                   void *arg_p,
                                   void *call_arg_p)
{
    uint8_t sha1[20];
    char sha1hex[41];
    int i;

    if (upgrade_bootloader_application_sha1(&sha1[0]) != 0) {
        std_fprintf(out_p, FSTR("failed\r\n"));
        return (-1);
    }

    for (i = 0; i < membersof(sha1); i++) {
        std_sprintf(&sha1hex[2 * i], FSTR("%02x"), sha1[i]);
    }

    std_fprintf(out_p, FSTR("SHA1: %s\r\n"), &sha1hex[0]);

    return (0);
}

int upgrade_bootloader_application_module_init()
{
    fs_command_init(&cmd_application_erase,
                    FSTR("/oam/upgrade/application/erase"),
                    cmd_application_erase_cb,
                    NULL);
    fs_command_register(&cmd_application_erase);

    fs_command_init(&cmd_application_sha1,
                    FSTR("/oam/upgrade/application/sha1"),
                    cmd_application_sha1_cb,
                    NULL);
    fs_command_register(&cmd_application_sha1);

    return (0);
}

int upgrade_bootloader_application_erase()
{
    return (upgrade_bootloader_application_port_erase());
}

int upgrade_bootloader_application_write_begin(void)
{
    return (upgrade_bootloader_application_port_write_begin());
}

int upgrade_bootloader_application_write_chunk(const void *buf_p,
                                               size_t size)
{
    return (upgrade_bootloader_application_port_write_chunk(buf_p, size));
}

int upgrade_bootloader_application_write_end(void)
{
    return (upgrade_bootloader_application_port_write_end());
}

int upgrade_bootloader_application_sha1(uint8_t *dst_p)
{
    return (upgrade_bootloader_application_port_sha1(dst_p));
}

int upgrade_bootloader_application_is_valid()
{
    return (upgrade_bootloader_application_port_is_valid());
}

int upgrade_bootloader_application_write_valid_flag()
{
    return (upgrade_bootloader_application_port_write_valid_flag());
}

int upgrade_bootloader_application_jump()
{
    return (upgrade_bootloader_application_port_jump());
}
