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
#include "base64_mock.h"

int mock_write_base64_encode(char *dst_p,
                             const void *src_p,
                             size_t size,
                             int res)
{
    harness_mock_write("base64_encode(): return (dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("base64_encode(src_p)",
                       src_p,
                       size);

    harness_mock_write("base64_encode(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("base64_encode(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(base64_encode)(char *dst_p,
                                               const void *src_p,
                                               size_t size)
{
    int res;

    harness_mock_read("base64_encode(): return (dst_p)",
                      dst_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("base64_encode(src_p)",
                        src_p);

    harness_mock_assert("base64_encode(size)",
                        &size);

    harness_mock_read("base64_encode(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_base64_decode(void *dst_p,
                             const char *src_p,
                             size_t size,
                             int res)
{
    harness_mock_write("base64_decode(): return (dst_p)",
                       dst_p,
                       size);

    harness_mock_write("base64_decode(src_p)",
                       src_p,
                       strlen(src_p) + 1);

    harness_mock_write("base64_decode(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("base64_decode(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(base64_decode)(void *dst_p,
                                               const char *src_p,
                                               size_t size)
{
    int res;

    harness_mock_read("base64_decode(): return (dst_p)",
                      dst_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("base64_decode(src_p)",
                        src_p);

    harness_mock_assert("base64_decode(size)",
                        &size);

    harness_mock_read("base64_decode(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
