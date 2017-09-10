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
#include "settings_mock.h"

int mock_write_settings_module_init(int res)
{
    harness_mock_write("settings_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(settings_module_init)()
{
    int res;

    harness_mock_read("settings_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_settings_read(void *dst_p,
                             size_t src,
                             size_t size,
                             ssize_t res)
{
    harness_mock_write("settings_read(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("settings_read(src)",
                       &src,
                       sizeof(src));

    harness_mock_write("settings_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("settings_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(settings_read)(void *dst_p,
                                                   size_t src,
                                                   size_t size)
{
    ssize_t res;

    harness_mock_read("settings_read(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("settings_read(src)",
                        &src);

    harness_mock_assert("settings_read(size)",
                        &size);

    harness_mock_read("settings_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_settings_write(size_t dst,
                              const void *src_p,
                              size_t size,
                              ssize_t res)
{
    harness_mock_write("settings_write(dst)",
                       &dst,
                       sizeof(dst));

    harness_mock_write("settings_write(src_p)",
                       src_p,
                       size);

    harness_mock_write("settings_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("settings_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(settings_write)(size_t dst,
                                                    const void *src_p,
                                                    size_t size)
{
    ssize_t res;

    harness_mock_assert("settings_write(dst)",
                        &dst);

    harness_mock_assert("settings_write(src_p)",
                        src_p);

    harness_mock_assert("settings_write(size)",
                        &size);

    harness_mock_read("settings_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_settings_read_by_name(const char *name_p,
                                     void *dst_p,
                                     size_t size,
                                     ssize_t res)
{
    harness_mock_write("settings_read_by_name(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("settings_read_by_name(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("settings_read_by_name(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("settings_read_by_name(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(settings_read_by_name)(const char *name_p,
                                                           void *dst_p,
                                                           size_t size)
{
    ssize_t res;

    harness_mock_assert("settings_read_by_name(name_p)",
                        name_p);

    harness_mock_read("settings_read_by_name(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("settings_read_by_name(size)",
                        &size);

    harness_mock_read("settings_read_by_name(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_settings_write_by_name(const char *name_p,
                                      const void *src_p,
                                      size_t size,
                                      ssize_t res)
{
    harness_mock_write("settings_write_by_name(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("settings_write_by_name(src_p)",
                       src_p,
                       size);

    harness_mock_write("settings_write_by_name(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("settings_write_by_name(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(settings_write_by_name)(const char *name_p,
                                                            const void *src_p,
                                                            size_t size)
{
    ssize_t res;

    harness_mock_assert("settings_write_by_name(name_p)",
                        name_p);

    harness_mock_assert("settings_write_by_name(src_p)",
                        src_p);

    harness_mock_assert("settings_write_by_name(size)",
                        &size);

    harness_mock_read("settings_write_by_name(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_settings_reset(int res)
{
    harness_mock_write("settings_reset(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(settings_reset)()
{
    int res;

    harness_mock_read("settings_reset(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
