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
#include "soam_mock.h"

int mock_write_soam_module_init(int res)
{
    harness_mock_write("soam_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(soam_module_init)()
{
    int res;

    harness_mock_read("soam_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_init(void *buf_p,
                         size_t size,
                         void *chout_p,
                         int res)
{
    harness_mock_write("soam_init(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("soam_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("soam_init(chout_p)",
                       chout_p,
                       size);

    harness_mock_write("soam_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(soam_init)(struct soam_t *self_p,
                                           void *buf_p,
                                           size_t size,
                                           void *chout_p)
{
    int res;

    harness_mock_assert("soam_init(buf_p)",
                        buf_p);

    harness_mock_assert("soam_init(size)",
                        &size);

    harness_mock_assert("soam_init(chout_p)",
                        chout_p);

    harness_mock_read("soam_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_input(uint8_t *buf_p,
                          size_t size,
                          int res)
{
    harness_mock_write("soam_input(buf_p)",
                       buf_p,
                       sizeof(*buf_p));

    harness_mock_write("soam_input(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("soam_input(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(soam_input)(struct soam_t *self_p,
                                            uint8_t *buf_p,
                                            size_t size)
{
    int res;

    harness_mock_assert("soam_input(buf_p)",
                        buf_p);

    harness_mock_assert("soam_input(size)",
                        &size);

    harness_mock_read("soam_input(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_write_begin(int type,
                                ssize_t res)
{
    harness_mock_write("soam_write_begin(type)",
                       &type,
                       sizeof(type));

    harness_mock_write("soam_write_begin(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(soam_write_begin)(struct soam_t *self_p,
                                                      int type)
{
    ssize_t res;

    harness_mock_assert("soam_write_begin(type)",
                        &type);

    harness_mock_read("soam_write_begin(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_write_chunk(const void *buf_p,
                                size_t size,
                                ssize_t res)
{
    harness_mock_write("soam_write_chunk(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("soam_write_chunk(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("soam_write_chunk(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(soam_write_chunk)(struct soam_t *self_p,
                                                      const void *buf_p,
                                                      size_t size)
{
    ssize_t res;

    harness_mock_assert("soam_write_chunk(buf_p)",
                        buf_p);

    harness_mock_assert("soam_write_chunk(size)",
                        &size);

    harness_mock_read("soam_write_chunk(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_write_end(ssize_t res)
{
    harness_mock_write("soam_write_end(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(soam_write_end)(struct soam_t *self_p)
{
    ssize_t res;

    harness_mock_read("soam_write_end(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_write(int type,
                          const void *buf_p,
                          size_t size,
                          ssize_t res)
{
    harness_mock_write("soam_write(type)",
                       &type,
                       sizeof(type));

    harness_mock_write("soam_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("soam_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("soam_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(soam_write)(struct soam_t *self_p,
                                                int type,
                                                const void *buf_p,
                                                size_t size)
{
    ssize_t res;

    harness_mock_assert("soam_write(type)",
                        &type);

    harness_mock_assert("soam_write(buf_p)",
                        buf_p);

    harness_mock_assert("soam_write(size)",
                        &size);

    harness_mock_read("soam_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_get_log_input_channel(void *res)
{
    harness_mock_write("soam_get_log_input_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(soam_get_log_input_channel)(struct soam_t *self_p)
{
    void *res;

    harness_mock_read("soam_get_log_input_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_soam_get_stdout_input_channel(void *res)
{
    harness_mock_write("soam_get_stdout_input_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(soam_get_stdout_input_channel)(struct soam_t *self_p)
{
    void *res;

    harness_mock_read("soam_get_stdout_input_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
