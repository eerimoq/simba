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

#define NVM_FILENAME "nvm.bin"

static int nvm_port_module_init()
{
    module.port.nvm_p = NULL;

    return (0);
}

static int nvm_port_mount()
{
    module.port.nvm_p = fopen(NVM_FILENAME, "r+");

    if (module.port.nvm_p == NULL) {
        return (-1);
    } else {
        return (0);
    }
}

static int nvm_port_format()
{
    if (module.port.nvm_p == NULL) {
        module.port.nvm_p = fopen(NVM_FILENAME, "w+");
    }

    ASSERTN(module.port.nvm_p != NULL, EINVAL);

    int i;
    uint8_t byte;

    byte = 0xff;
    (void)fseek(module.port.nvm_p, 0, SEEK_SET);

    for (i = 0; i < CONFIG_NVM_SIZE; i++) {
        fwrite(&byte, 1, sizeof(byte), module.port.nvm_p);
    }

    fclose(module.port.nvm_p);
    module.port.nvm_p = NULL;

    return (0);
}

static ssize_t nvm_port_read(void *dst_p, size_t src, size_t size)
{
    if (src >= CONFIG_NVM_SIZE) {
        return (-EINVAL);
    }

    if (src + size > CONFIG_NVM_SIZE) {
        return (-EINVAL);
    }

    (void)fseek(module.port.nvm_p, src, SEEK_SET);

    return (fread(dst_p, 1, size, module.port.nvm_p));
}

static ssize_t nvm_port_write(size_t dst, const void *src_p, size_t size)
{

    if (dst >= CONFIG_NVM_SIZE) {
        return (-EINVAL);
    }

    if (dst + size > CONFIG_NVM_SIZE) {
        return (-EINVAL);
    }

    (void)fseek(module.port.nvm_p, dst, SEEK_SET);

    return (fwrite(src_p, 1, size, module.port.nvm_p));
}
