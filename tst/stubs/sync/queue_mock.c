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
#include "queue_mock.h"

int mock_write_queue_init(void *buf_p,
                          size_t size,
                          int res)
{
    harness_mock_write("queue_init(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("queue_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("queue_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(queue_init)(struct queue_t *self_p,
                                            void *buf_p,
                                            size_t size)
{
    int res;

    harness_mock_assert("queue_init(buf_p)",
                        buf_p);

    harness_mock_assert("queue_init(size)",
                        &size);

    harness_mock_read("queue_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_start(int res)
{
    harness_mock_write("queue_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(queue_start)(struct queue_t *self_p)
{
    int res;

    harness_mock_read("queue_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_stop(int res)
{
    harness_mock_write("queue_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(queue_stop)(struct queue_t *self_p)
{
    int res;

    harness_mock_read("queue_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_stop_isr(int res)
{
    harness_mock_write("queue_stop_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(queue_stop_isr)(struct queue_t *self_p)
{
    int res;

    harness_mock_read("queue_stop_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_read(void *buf_p,
                          size_t size,
                          ssize_t res)
{
    harness_mock_write("queue_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("queue_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("queue_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(queue_read)(struct queue_t *self_p,
                                                void *buf_p,
                                                size_t size)
{
    ssize_t res;

    harness_mock_read("queue_read(): return (buf_p)",
                      buf_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("queue_read(size)",
                        &size);

    harness_mock_read("queue_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_write(const void *buf_p,
                           size_t size,
                           ssize_t res)
{
    harness_mock_write("queue_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("queue_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("queue_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(queue_write)(struct queue_t *self_p,
                                                 const void *buf_p,
                                                 size_t size)
{
    ssize_t res;

    harness_mock_assert("queue_write(buf_p)",
                        buf_p);

    harness_mock_assert("queue_write(size)",
                        &size);

    harness_mock_read("queue_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_write_isr(const void *buf_p,
                               size_t size,
                               ssize_t res)
{
    harness_mock_write("queue_write_isr(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("queue_write_isr(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("queue_write_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(queue_write_isr)(struct queue_t *self_p,
                                                     const void *buf_p,
                                                     size_t size)
{
    ssize_t res;

    harness_mock_assert("queue_write_isr(buf_p)",
                        buf_p);

    harness_mock_assert("queue_write_isr(size)",
                        &size);

    harness_mock_read("queue_write_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_size(ssize_t res)
{
    harness_mock_write("queue_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(queue_size)(struct queue_t *self_p)
{
    ssize_t res;

    harness_mock_read("queue_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_unused_size(ssize_t res)
{
    harness_mock_write("queue_unused_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(queue_unused_size)(struct queue_t *self_p)
{
    ssize_t res;

    harness_mock_read("queue_unused_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_unused_size_isr(ssize_t res)
{
    harness_mock_write("queue_unused_size_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(queue_unused_size_isr)(struct queue_t *self_p)
{
    ssize_t res;

    harness_mock_read("queue_unused_size_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_queue_ignore(size_t size,
                            ssize_t res)
{
    harness_mock_write("queue_ignore(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("queue_ignore(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(queue_ignore)(struct queue_t *self_p,
                                                  size_t size)
{
    ssize_t res;

    harness_mock_assert("queue_ignore(size)",
                        &size);

    harness_mock_read("queue_ignore(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
