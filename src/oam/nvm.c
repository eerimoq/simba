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

#if CONFIG_NVM_EEPROM_SOFT == 1
#    include "ports/nvm_port_eeprom_soft.h"
#else
#    include "nvm_port.h"
#endif

struct module_t {
    int initialized;
    struct module_port_t port;
#if CONFIG_FS_CMD_NVM_READ == 1
    struct fs_command_t cmd_read;
#endif
#if CONFIG_FS_CMD_NVM_WRITE == 1
    struct fs_command_t cmd_write;
#endif
};

static struct module_t module;

#if CONFIG_NVM_EEPROM_SOFT == 1
#    include "ports/nvm_port_eeprom_soft.i"
#else
#    include "nvm_port.i"
#endif

#if CONFIG_FS_CMD_NVM_READ == 1

static int cmd_read_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    long address;
    uint8_t byte;
    ssize_t size;

    if (argc != 2) {
        std_fprintf(chout_p, OSTR("Usage: read <address>\r\n"));

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &address) == NULL) {
        std_fprintf(chout_p, OSTR("%s: bad address\r\n"), argv[1]);

        return (-EINVAL);
    }

    size = nvm_read(&byte, address, sizeof(byte));

    if (size != sizeof(byte)) {
        return (-1);
    }

    std_fprintf(chout_p, OSTR("0x%02x\r\n"), byte);

    return (0);
}

#endif

#if CONFIG_FS_CMD_NVM_WRITE == 1

static int cmd_write_cb(int argc,
                        const char *argv[],
                        void *chout_p,
                        void *chin_p,
                        void *arg_p,
                        void *call_arg_p)
{
    long address;
    long data;
    uint8_t byte;
    ssize_t size;

    if (argc != 3) {
        std_fprintf(chout_p, OSTR("Usage: write <address> <data>\r\n"));

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &address) == NULL) {
        std_fprintf(chout_p, OSTR("%s: bad address\r\n"), argv[1]);

        return (-EINVAL);
    }

    if (std_strtol(argv[2], &data) == NULL) {
        std_fprintf(chout_p, OSTR("%s: bad data\r\n"), argv[2]);

        return (-EINVAL);
    }

    if ((data < 0) || (data > 0xff)) {
        std_fprintf(chout_p, OSTR("%s: data out of range\r\n"), argv[2]);

        return (-EINVAL);
    }

    byte = (uint8_t)data;

    size = nvm_write(address, &byte, sizeof(byte));

    if (size != sizeof(byte)) {
        return (-1);
    }

    return (0);
}

#endif

int nvm_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_NVM_READ == 1

    fs_command_init(&module.cmd_read,
                    CSTR("/oam/nvm/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&module.cmd_read);

#endif

#if CONFIG_FS_CMD_NVM_WRITE == 1

    fs_command_init(&module.cmd_write,
                    CSTR("/oam/nvm/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&module.cmd_write);

#endif

    return (nvm_port_module_init());
}

int nvm_mount()
{
    return (nvm_port_mount());
}

int nvm_format()
{
    return (nvm_port_format());
}

ssize_t nvm_read(void *dst_p, uint32_t src, size_t size)
{
    ASSERTN(module.initialized == 1, EINVAL);
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (nvm_port_read(dst_p, src, size));
}

ssize_t nvm_write(uint32_t dst, const void *src_p, size_t size)
{
    ASSERTN(module.initialized == 1, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (nvm_port_write(dst, src_p, size));
}
