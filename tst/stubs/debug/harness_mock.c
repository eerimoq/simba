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
#include "harness_mock.h"

int mock_write_harness_run(struct harness_testcase_t *testcases_p,
                           int res)
{
    harness_mock_write("harness_run(testcases_p)",
                       testcases_p,
                       sizeof(*testcases_p));

    harness_mock_write("harness_run(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(harness_run)(struct harness_testcase_t *testcases_p)
{
    int res;

    harness_mock_assert("harness_run(testcases_p)",
                        testcases_p,
                        sizeof(*testcases_p));

    harness_mock_read("harness_run(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_harness_expect(void *chan_p,
                              const char *pattern_p,
                              const struct time_t *timeout_p,
                              int res)
{
    harness_mock_write("harness_expect(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("harness_expect(pattern_p)",
                       pattern_p,
                       strlen(pattern_p) + 1);

    harness_mock_write("harness_expect(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("harness_expect(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(harness_expect)(void *chan_p,
                                                const char *pattern_p,
                                                const struct time_t *timeout_p)
{
    int res;

    harness_mock_assert("harness_expect(chan_p)",
                        chan_p,
                        sizeof(*chan_p));

    harness_mock_assert("harness_expect(pattern_p)",
                        pattern_p,
                        sizeof(*pattern_p));

    harness_mock_assert("harness_expect(timeout_p)",
                        timeout_p,
                        sizeof(*timeout_p));

    harness_mock_read("harness_expect(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_harness_mock_write(const char *id_p,
                                  const void *buf_p,
                                  size_t size,
                                  ssize_t res)
{
    harness_mock_write("harness_mock_write(id_p)",
                       id_p,
                       strlen(id_p) + 1);

    harness_mock_write("harness_mock_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("harness_mock_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("harness_mock_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(harness_mock_write)(const char *id_p,
                                                        const void *buf_p,
                                                        size_t size)
{
    ssize_t res;

    harness_mock_assert("harness_mock_write(id_p)",
                        id_p,
                        sizeof(*id_p));

    harness_mock_assert("harness_mock_write(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("harness_mock_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("harness_mock_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_harness_mock_read(const char *id_p,
                                 void *buf_p,
                                 size_t size,
                                 ssize_t res)
{
    harness_mock_write("harness_mock_read(id_p)",
                       id_p,
                       strlen(id_p) + 1);

    harness_mock_write("harness_mock_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("harness_mock_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("harness_mock_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(harness_mock_read)(const char *id_p,
                                                       void *buf_p,
                                                       size_t size)
{
    ssize_t res;

    harness_mock_assert("harness_mock_read(id_p)",
                        id_p,
                        sizeof(*id_p));

    harness_mock_read("harness_mock_read(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("harness_mock_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("harness_mock_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_harness_mock_try_read(const char *id_p,
                                     void *buf_p,
                                     size_t size,
                                     ssize_t res)
{
    harness_mock_write("harness_mock_try_read(id_p)",
                       id_p,
                       strlen(id_p) + 1);

    harness_mock_write("harness_mock_try_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("harness_mock_try_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("harness_mock_try_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(harness_mock_try_read)(const char *id_p,
                                                           void *buf_p,
                                                           size_t size)
{
    ssize_t res;

    harness_mock_assert("harness_mock_try_read(id_p)",
                        id_p,
                        sizeof(*id_p));

    harness_mock_read("harness_mock_try_read(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("harness_mock_try_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("harness_mock_try_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_harness_mock_assert(const char *id_p,
                                   const void *buf_p,
                                   size_t size,
                                   int res)
{
    harness_mock_write("harness_mock_assert(id_p)",
                       id_p,
                       strlen(id_p) + 1);

    harness_mock_write("harness_mock_assert(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("harness_mock_assert(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("harness_mock_assert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(harness_mock_assert)(const char *id_p,
                                                     const void *buf_p,
                                                     size_t size)
{
    int res;

    harness_mock_assert("harness_mock_assert(id_p)",
                        id_p,
                        sizeof(*id_p));

    harness_mock_assert("harness_mock_assert(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("harness_mock_assert(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("harness_mock_assert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_harness_mock_write_notify(const char *id_p,
                                         const void *buf_p,
                                         size_t size,
                                         ssize_t res)
{
    harness_mock_write("harness_mock_write_notify(id_p)",
                       id_p,
                       strlen(id_p) + 1);

    harness_mock_write("harness_mock_write_notify(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("harness_mock_write_notify(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("harness_mock_write_notify(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(harness_mock_write_notify)(const char *id_p,
                                                               const void *buf_p,
                                                               size_t size)
{
    ssize_t res;

    harness_mock_assert("harness_mock_write_notify(id_p)",
                        id_p,
                        sizeof(*id_p));

    harness_mock_assert("harness_mock_write_notify(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("harness_mock_write_notify(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("harness_mock_write_notify(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_harness_mock_read_wait(const char *id_p,
                                      void *buf_p,
                                      size_t size,
                                      struct time_t *timeout_p,
                                      ssize_t res)
{
    harness_mock_write("harness_mock_read_wait(id_p)",
                       id_p,
                       strlen(id_p) + 1);

    harness_mock_write("harness_mock_read_wait(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("harness_mock_read_wait(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("harness_mock_read_wait(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("harness_mock_read_wait(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(harness_mock_read_wait)(const char *id_p,
                                                            void *buf_p,
                                                            size_t size,
                                                            struct time_t *timeout_p)
{
    ssize_t res;

    harness_mock_assert("harness_mock_read_wait(id_p)",
                        id_p,
                        sizeof(*id_p));

    harness_mock_read("harness_mock_read_wait(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("harness_mock_read_wait(size)",
                        &size,
                        sizeof(size));

    harness_mock_assert("harness_mock_read_wait(timeout_p)",
                        timeout_p,
                        sizeof(*timeout_p));

    harness_mock_read("harness_mock_read_wait(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
