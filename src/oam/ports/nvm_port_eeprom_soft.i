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

uint8_t nvm_eeprom_soft_block_0[CONFIG_NVM_EEPROM_SOFT_BLOCK_0_SIZE]
__attribute__ ((section (".nvm.eeprom_soft.block_0"), weak));

uint8_t nvm_eeprom_soft_block_1[CONFIG_NVM_EEPROM_SOFT_BLOCK_1_SIZE]
__attribute__ ((section (".nvm.eeprom_soft.block_1"), weak));

static int nvm_port_module_init(void)
{
    int res;

    res = eeprom_soft_module_init();

    if (res != 0) {
        return (res);
    }

    res = flash_init(&module.port.flash,
                     &flash_device[CONFIG_NVM_EEPROM_SOFT_FLASH_DEVICE_INDEX]);

    if (res != 0) {
        return (res);
    }

    module.port.blocks[0].address = (uintptr_t)&nvm_eeprom_soft_block_0[0];
    module.port.blocks[0].size = sizeof(nvm_eeprom_soft_block_0);
    module.port.blocks[1].address = (uintptr_t)&nvm_eeprom_soft_block_1[0];
    module.port.blocks[1].size = sizeof(nvm_eeprom_soft_block_1);

    return (eeprom_soft_init(&module.port.eeprom_soft,
                             &module.port.flash,
                             &module.port.blocks[0],
                             membersof(module.port.blocks),
                             CONFIG_NVM_EEPROM_SOFT_CHUNK_SIZE));
}

static int nvm_port_mount()
{
    return (eeprom_soft_mount(&module.port.eeprom_soft));
}

static int nvm_port_format()
{
    return (eeprom_soft_format(&module.port.eeprom_soft));
}

static ssize_t nvm_port_read(void *dst_p, size_t src, size_t size)
{
    return (eeprom_soft_read(&module.port.eeprom_soft, dst_p, src, size));
}

static ssize_t nvm_port_write(size_t dst, const void *src_p, size_t size)
{
    return (eeprom_soft_write(&module.port.eeprom_soft, dst, src_p, size));
}
