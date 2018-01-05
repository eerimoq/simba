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
#include "inet_mock.h"

int mock_write_inet_module_init(int res)
{
    harness_mock_write("inet_module_init()",
                       NULL,
                       0);

    harness_mock_write("inet_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(inet_module_init)()
{
    int res;

    harness_mock_assert("inet_module_init()",
                        NULL,
                        0);

    harness_mock_read("inet_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_inet_aton(const char *src_p,
                         struct inet_ip_addr_t *dst_p,
                         int res)
{
    harness_mock_write("inet_aton(src_p)",
                       src_p,
                       strlen(src_p) + 1);

    harness_mock_write("inet_aton(): return (dst_p)",
                       dst_p,
                       sizeof(*dst_p));

    harness_mock_write("inet_aton(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(inet_aton)(const char *src_p,
                                           struct inet_ip_addr_t *dst_p)
{
    int res;

    harness_mock_assert("inet_aton(src_p)",
                        src_p,
                        sizeof(*src_p));

    harness_mock_read("inet_aton(): return (dst_p)",
                      dst_p,
                      sizeof(*dst_p));

    harness_mock_read("inet_aton(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_inet_ntoa(const struct inet_ip_addr_t *src_p,
                         char *dst_p,
                         char *res)
{
    harness_mock_write("inet_ntoa(src_p)",
                       src_p,
                       sizeof(*src_p));

    harness_mock_write("inet_ntoa(): return (dst_p)",
                       dst_p,
                       strlen(dst_p) + 1);

    harness_mock_write("inet_ntoa(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

char *__attribute__ ((weak)) STUB(inet_ntoa)(const struct inet_ip_addr_t *src_p,
                                             char *dst_p)
{
    char *res;

    harness_mock_assert("inet_ntoa(src_p)",
                        src_p,
                        sizeof(*src_p));

    harness_mock_read("inet_ntoa(): return (dst_p)",
                      dst_p,
                      sizeof(*dst_p));

    harness_mock_read("inet_ntoa(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_inet_checksum(void *buf_p,
                             size_t size,
                             uint16_t res)
{
    harness_mock_write("inet_checksum(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("inet_checksum(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("inet_checksum(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

uint16_t __attribute__ ((weak)) STUB(inet_checksum)(void *buf_p,
                                                    size_t size)
{
    uint16_t res;

    harness_mock_assert("inet_checksum(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("inet_checksum(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("inet_checksum(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
