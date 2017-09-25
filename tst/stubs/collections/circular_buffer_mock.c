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
#include "circular_buffer_mock.h"

int mock_write_circular_buffer_init(void *buf_p,
                                    size_t size,
                                    int res)
{
    harness_mock_write("circular_buffer_init(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("circular_buffer_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("circular_buffer_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(circular_buffer_init)(struct circular_buffer_t *self_p,
                                                      void *buf_p,
                                                      size_t size)
{
    int res;

    harness_mock_assert("circular_buffer_init(buf_p)",
                        buf_p);

    harness_mock_assert("circular_buffer_init(size)",
                        &size);

    harness_mock_read("circular_buffer_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_write(const void *buf_p,
                                     size_t size,
                                     ssize_t res)
{
    harness_mock_write("circular_buffer_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("circular_buffer_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("circular_buffer_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_write)(struct circular_buffer_t *self_p,
                                                           const void *buf_p,
                                                           size_t size)
{
    ssize_t res;

    harness_mock_assert("circular_buffer_write(buf_p)",
                        buf_p);

    harness_mock_assert("circular_buffer_write(size)",
                        &size);

    harness_mock_read("circular_buffer_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_read(void *buf_p,
                                    size_t size,
                                    ssize_t res)
{
    harness_mock_write("circular_buffer_read(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("circular_buffer_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("circular_buffer_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_read)(struct circular_buffer_t *self_p,
                                                          void *buf_p,
                                                          size_t size)
{
    ssize_t res;

    harness_mock_assert("circular_buffer_read(buf_p)",
                        buf_p);

    harness_mock_assert("circular_buffer_read(size)",
                        &size);

    harness_mock_read("circular_buffer_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_used_size(ssize_t res)
{
    harness_mock_write("circular_buffer_used_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_used_size)(struct circular_buffer_t *self_p)
{
    ssize_t res;

    harness_mock_read("circular_buffer_used_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_unused_size(ssize_t res)
{
    harness_mock_write("circular_buffer_unused_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_unused_size)(struct circular_buffer_t *self_p)
{
    ssize_t res;

    harness_mock_read("circular_buffer_unused_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_skip_front(size_t size,
                                          ssize_t res)
{
    harness_mock_write("circular_buffer_skip_front(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("circular_buffer_skip_front(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_skip_front)(struct circular_buffer_t *self_p,
                                                                size_t size)
{
    ssize_t res;

    harness_mock_assert("circular_buffer_skip_front(size)",
                        &size);

    harness_mock_read("circular_buffer_skip_front(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_reverse_skip_back(size_t size,
                                                 ssize_t res)
{
    harness_mock_write("circular_buffer_reverse_skip_back(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("circular_buffer_reverse_skip_back(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_reverse_skip_back)(struct circular_buffer_t *self_p,
                                                                       size_t size)
{
    ssize_t res;

    harness_mock_assert("circular_buffer_reverse_skip_back(size)",
                        &size);

    harness_mock_read("circular_buffer_reverse_skip_back(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_array_one(void **buf_pp,
                                         size_t size,
                                         ssize_t res)
{
    harness_mock_write("circular_buffer_array_one(): return (buf_pp)",
                       buf_pp,
                       size);

    harness_mock_write("circular_buffer_array_one(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("circular_buffer_array_one(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_array_one)(struct circular_buffer_t *self_p,
                                                               void **buf_pp,
                                                               size_t size)
{
    ssize_t res;

    harness_mock_read("circular_buffer_array_one(): return (buf_pp)",
                      buf_pp,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("circular_buffer_array_one(size)",
                        &size);

    harness_mock_read("circular_buffer_array_one(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_circular_buffer_array_two(void **buf_pp,
                                         size_t size,
                                         ssize_t res)
{
    harness_mock_write("circular_buffer_array_two(): return (buf_pp)",
                       buf_pp,
                       size);

    harness_mock_write("circular_buffer_array_two(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("circular_buffer_array_two(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(circular_buffer_array_two)(struct circular_buffer_t *self_p,
                                                               void **buf_pp,
                                                               size_t size)
{
    ssize_t res;

    harness_mock_read("circular_buffer_array_two(): return (buf_pp)",
                      buf_pp,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("circular_buffer_array_two(size)",
                        &size);

    harness_mock_read("circular_buffer_array_two(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
