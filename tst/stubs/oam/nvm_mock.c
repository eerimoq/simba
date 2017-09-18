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
#include "nvm_mock.h"

int mock_write_nvm_module_init(int res)
{
    harness_mock_write("nvm_module_init()",
                       NULL,
                       0);

    harness_mock_write("nvm_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nvm_module_init)()
{
    int res;

    harness_mock_assert("nvm_module_init()",
                        NULL);

    harness_mock_read("nvm_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nvm_mount(int res)
{
    harness_mock_write("nvm_mount()",
                       NULL,
                       0);

    harness_mock_write("nvm_mount(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nvm_mount)()
{
    int res;

    harness_mock_assert("nvm_mount()",
                        NULL);

    harness_mock_read("nvm_mount(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nvm_format(int res)
{
    harness_mock_write("nvm_format()",
                       NULL,
                       0);

    harness_mock_write("nvm_format(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(nvm_format)()
{
    int res;

    harness_mock_assert("nvm_format()",
                        NULL);

    harness_mock_read("nvm_format(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nvm_read(void *dst_p,
                        uint32_t src,
                        size_t size,
                        ssize_t res)
{
    harness_mock_write("nvm_read(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("nvm_read(src)",
                       &src,
                       sizeof(src));

    harness_mock_write("nvm_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("nvm_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(nvm_read)(void *dst_p,
                                              uint32_t src,
                                              size_t size)
{
    ssize_t res;

    harness_mock_read("nvm_read(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("nvm_read(src)",
                        &src);

    harness_mock_assert("nvm_read(size)",
                        &size);

    harness_mock_read("nvm_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_nvm_write(uint32_t dst,
                         const void *src_p,
                         size_t size,
                         ssize_t res)
{
    harness_mock_write("nvm_write(dst)",
                       &dst,
                       sizeof(dst));

    harness_mock_write("nvm_write(src_p)",
                       src_p,
                       size);

    harness_mock_write("nvm_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("nvm_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(nvm_write)(uint32_t dst,
                                               const void *src_p,
                                               size_t size)
{
    ssize_t res;

    harness_mock_assert("nvm_write(dst)",
                        &dst);

    harness_mock_assert("nvm_write(src_p)",
                        src_p);

    harness_mock_assert("nvm_write(size)",
                        &size);

    harness_mock_read("nvm_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
