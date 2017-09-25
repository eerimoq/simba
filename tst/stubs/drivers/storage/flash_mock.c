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
#include "flash_mock.h"

int mock_write_flash_module_init(int res)
{
    harness_mock_write("flash_module_init()",
                       NULL,
                       0);

    harness_mock_write("flash_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(flash_module_init)()
{
    int res;

    harness_mock_assert("flash_module_init()",
                        NULL,
                        0);

    harness_mock_read("flash_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_flash_init(struct flash_device_t *dev_p,
                          int res)
{
    harness_mock_write("flash_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("flash_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(flash_init)(struct flash_driver_t *self_p,
                                            struct flash_device_t *dev_p)
{
    int res;

    harness_mock_assert("flash_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_read("flash_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_flash_read(void *dst_p,
                          uintptr_t src,
                          size_t size,
                          ssize_t res)
{
    harness_mock_write("flash_read(dst_p)",
                       dst_p,
                       size);

    harness_mock_write("flash_read(src)",
                       &src,
                       sizeof(src));

    harness_mock_write("flash_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("flash_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(flash_read)(struct flash_driver_t *self_p,
                                                void *dst_p,
                                                uintptr_t src,
                                                size_t size)
{
    ssize_t res;

    harness_mock_assert("flash_read(dst_p)",
                        dst_p,
                        size);

    harness_mock_assert("flash_read(src)",
                        &src,
                        sizeof(src));

    harness_mock_assert("flash_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("flash_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_flash_write(uintptr_t dst,
                           const void *src_p,
                           size_t size,
                           ssize_t res)
{
    harness_mock_write("flash_write(dst)",
                       &dst,
                       sizeof(dst));

    harness_mock_write("flash_write(src_p)",
                       src_p,
                       size);

    harness_mock_write("flash_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("flash_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(flash_write)(struct flash_driver_t *self_p,
                                                 uintptr_t dst,
                                                 const void *src_p,
                                                 size_t size)
{
    ssize_t res;

    harness_mock_assert("flash_write(dst)",
                        &dst,
                        sizeof(dst));

    harness_mock_assert("flash_write(src_p)",
                        src_p,
                        size);

    harness_mock_assert("flash_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("flash_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_flash_erase(uintptr_t addr,
                           size_t size,
                           int res)
{
    harness_mock_write("flash_erase(addr)",
                       &addr,
                       sizeof(addr));

    harness_mock_write("flash_erase(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("flash_erase(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(flash_erase)(struct flash_driver_t *self_p,
                                             uintptr_t addr,
                                             size_t size)
{
    int res;

    harness_mock_assert("flash_erase(addr)",
                        &addr,
                        sizeof(addr));

    harness_mock_assert("flash_erase(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("flash_erase(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
