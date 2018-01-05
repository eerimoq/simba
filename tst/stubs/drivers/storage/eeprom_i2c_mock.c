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
#include "eeprom_i2c_mock.h"

int mock_write_eeprom_i2c_module_init(int res)
{
    harness_mock_write("eeprom_i2c_module_init()",
                       NULL,
                       0);

    harness_mock_write("eeprom_i2c_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(eeprom_i2c_module_init)()
{
    int res;

    harness_mock_assert("eeprom_i2c_module_init()",
                        NULL,
                        0);

    harness_mock_read("eeprom_i2c_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_i2c_init(struct i2c_driver_t *i2c_p,
                               int i2c_address,
                               uint32_t size,
                               int res)
{
    harness_mock_write("eeprom_i2c_init(i2c_p)",
                       i2c_p,
                       sizeof(*i2c_p));

    harness_mock_write("eeprom_i2c_init(i2c_address)",
                       &i2c_address,
                       sizeof(i2c_address));

    harness_mock_write("eeprom_i2c_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("eeprom_i2c_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(eeprom_i2c_init)(struct eeprom_i2c_driver_t *self_p,
                                                 struct i2c_driver_t *i2c_p,
                                                 int i2c_address,
                                                 uint32_t size)
{
    int res;

    harness_mock_assert("eeprom_i2c_init(i2c_p)",
                        i2c_p,
                        sizeof(*i2c_p));

    harness_mock_assert("eeprom_i2c_init(i2c_address)",
                        &i2c_address,
                        sizeof(i2c_address));

    harness_mock_assert("eeprom_i2c_init(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("eeprom_i2c_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_i2c_read(void *dst_p,
                               uint32_t src,
                               size_t size,
                               ssize_t res)
{
    harness_mock_write("eeprom_i2c_read(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("eeprom_i2c_read(src)",
                       &src,
                       sizeof(src));

    harness_mock_write("eeprom_i2c_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("eeprom_i2c_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(eeprom_i2c_read)(struct eeprom_i2c_driver_t *self_p,
                                                     void *dst_p,
                                                     uint32_t src,
                                                     size_t size)
{
    ssize_t res;

    harness_mock_read("eeprom_i2c_read(): return (dst_p)",
                      dst_p,
                      size);

    harness_mock_assert("eeprom_i2c_read(src)",
                        &src,
                        sizeof(src));

    harness_mock_assert("eeprom_i2c_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("eeprom_i2c_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_eeprom_i2c_write(uint32_t dst,
                                const void *src_p,
                                size_t size,
                                ssize_t res)
{
    harness_mock_write("eeprom_i2c_write(dst)",
                       &dst,
                       sizeof(dst));

    harness_mock_write("eeprom_i2c_write(src_p)",
                       src_p,
                       size);

    harness_mock_write("eeprom_i2c_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("eeprom_i2c_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(eeprom_i2c_write)(struct eeprom_i2c_driver_t *self_p,
                                                      uint32_t dst,
                                                      const void *src_p,
                                                      size_t size)
{
    ssize_t res;

    harness_mock_assert("eeprom_i2c_write(dst)",
                        &dst,
                        sizeof(dst));

    harness_mock_assert("eeprom_i2c_write(src_p)",
                        src_p,
                        size);

    harness_mock_assert("eeprom_i2c_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("eeprom_i2c_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
