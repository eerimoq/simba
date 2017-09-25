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
#include "event_mock.h"

int mock_write_event_init(int res)
{
    harness_mock_write("event_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(event_init)(struct event_t *self_p)
{
    int res;

    harness_mock_read("event_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_event_read(void *buf_p,
                          size_t size,
                          ssize_t res)
{
    harness_mock_write("event_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("event_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("event_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(event_read)(struct event_t *self_p,
                                                void *buf_p,
                                                size_t size)
{
    ssize_t res;

    harness_mock_read("event_read(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("event_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("event_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_event_write(const void *buf_p,
                           size_t size,
                           ssize_t res)
{
    harness_mock_write("event_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("event_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("event_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(event_write)(struct event_t *self_p,
                                                 const void *buf_p,
                                                 size_t size)
{
    ssize_t res;

    harness_mock_assert("event_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("event_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("event_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_event_write_isr(const void *buf_p,
                               size_t size,
                               ssize_t res)
{
    harness_mock_write("event_write_isr(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("event_write_isr(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("event_write_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(event_write_isr)(struct event_t *self_p,
                                                     const void *buf_p,
                                                     size_t size)
{
    ssize_t res;

    harness_mock_assert("event_write_isr(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("event_write_isr(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("event_write_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_event_size(ssize_t res)
{
    harness_mock_write("event_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(event_size)(struct event_t *self_p)
{
    ssize_t res;

    harness_mock_read("event_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_event_clear(uint32_t mask,
                           int res)
{
    harness_mock_write("event_clear(mask)",
                       &mask,
                       sizeof(mask));

    harness_mock_write("event_clear(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(event_clear)(struct event_t *self_p,
                                             uint32_t mask)
{
    int res;

    harness_mock_assert("event_clear(mask)",
                        &mask,
                        sizeof(mask));

    harness_mock_read("event_clear(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
