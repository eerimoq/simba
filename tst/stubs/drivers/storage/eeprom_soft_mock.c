/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "eeprom_soft_mock.h"

int mock_write_eeprom_soft_module_init(int res)
{
    harness_mock_write("eeprom_soft_module_init()",
                       NULL,
                       0);

    harness_mock_write("eeprom_soft_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(eeprom_soft_module_init)()
{
    int res;

    harness_mock_assert("eeprom_soft_module_init()",
                        NULL,
                        0);

    harness_mock_read("eeprom_soft_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_soft_init(struct flash_driver_t *flash_p,
                                const struct eeprom_soft_block_t *blocks_p,
                                int number_of_blocks,
                                size_t chunk_size,
                                int res)
{
    harness_mock_write("eeprom_soft_init(flash_p)",
                       flash_p,
                       sizeof(*flash_p));

    harness_mock_write("eeprom_soft_init(blocks_p)",
                       blocks_p,
                       sizeof(*blocks_p));

    harness_mock_write("eeprom_soft_init(number_of_blocks)",
                       &number_of_blocks,
                       sizeof(number_of_blocks));

    harness_mock_write("eeprom_soft_init(chunk_size)",
                       &chunk_size,
                       sizeof(chunk_size));

    harness_mock_write("eeprom_soft_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(eeprom_soft_init)(struct eeprom_soft_driver_t *self_p,
                                                  struct flash_driver_t *flash_p,
                                                  const struct eeprom_soft_block_t *blocks_p,
                                                  int number_of_blocks,
                                                  size_t chunk_size)
{
    int res;

    harness_mock_assert("eeprom_soft_init(flash_p)",
                        flash_p,
                        sizeof(*flash_p));

    harness_mock_assert("eeprom_soft_init(blocks_p)",
                        blocks_p,
                        sizeof(*blocks_p));

    harness_mock_assert("eeprom_soft_init(number_of_blocks)",
                        &number_of_blocks,
                        sizeof(number_of_blocks));

    harness_mock_assert("eeprom_soft_init(chunk_size)",
                        &chunk_size,
                        sizeof(chunk_size));

    harness_mock_read("eeprom_soft_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_soft_mount(int res)
{
    harness_mock_write("eeprom_soft_mount(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(eeprom_soft_mount)(struct eeprom_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("eeprom_soft_mount(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_soft_format(int res)
{
    harness_mock_write("eeprom_soft_format(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(eeprom_soft_format)(struct eeprom_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("eeprom_soft_format(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_soft_read(void *dst_p,
                                uintptr_t src,
                                size_t size,
                                ssize_t res)
{
    harness_mock_write("eeprom_soft_read(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("eeprom_soft_read(src)",
                       &src,
                       sizeof(src));

    harness_mock_write("eeprom_soft_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("eeprom_soft_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(eeprom_soft_read)(struct eeprom_soft_driver_t *self_p,
                                                      void *dst_p,
                                                      uintptr_t src,
                                                      size_t size)
{
    ssize_t res;

    harness_mock_read("eeprom_soft_read(): return (dst_p)",
                      dst_p,
                      size);

    harness_mock_assert("eeprom_soft_read(src)",
                        &src,
                        sizeof(src));

    harness_mock_assert("eeprom_soft_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("eeprom_soft_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_soft_write(uintptr_t dst,
                                 const void *src_p,
                                 size_t size,
                                 ssize_t res)
{
    harness_mock_write("eeprom_soft_write(dst)",
                       &dst,
                       sizeof(dst));

    harness_mock_write("eeprom_soft_write(src_p)",
                       src_p,
                       size);

    harness_mock_write("eeprom_soft_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("eeprom_soft_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(eeprom_soft_write)(struct eeprom_soft_driver_t *self_p,
                                                       uintptr_t dst,
                                                       const void *src_p,
                                                       size_t size)
{
    ssize_t res;

    harness_mock_assert("eeprom_soft_write(dst)",
                        &dst,
                        sizeof(dst));

    harness_mock_assert("eeprom_soft_write(src_p)",
                        src_p,
                        size);

    harness_mock_assert("eeprom_soft_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("eeprom_soft_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
