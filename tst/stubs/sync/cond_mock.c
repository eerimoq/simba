/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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
#include "cond_mock.h"

int mock_write_cond_module_init(int res)
{
    harness_mock_write("cond_module_init()",
                       NULL,
                       0);

    harness_mock_write("cond_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(cond_module_init)()
{
    int res;

    harness_mock_assert("cond_module_init()",
                        NULL);

    harness_mock_read("cond_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_cond_init(int res)
{
    harness_mock_write("cond_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(cond_init)(struct cond_t *self_p)
{
    int res;

    harness_mock_read("cond_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_cond_wait(struct mutex_t *mutex_p,
                         struct time_t *timeout_p,
                         int res)
{
    harness_mock_write("cond_wait(mutex_p)",
                       mutex_p,
                       sizeof(*mutex_p));

    harness_mock_write("cond_wait(): return (timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("cond_wait(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(cond_wait)(struct cond_t *self_p,
                                           struct mutex_t *mutex_p,
                                           struct time_t *timeout_p)
{
    int res;

    harness_mock_assert("cond_wait(mutex_p)",
                        mutex_p);

    harness_mock_read("cond_wait(): return (timeout_p)",
                      timeout_p,
                      -1);

    harness_mock_read("cond_wait(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_cond_signal(int res)
{
    harness_mock_write("cond_signal(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(cond_signal)(struct cond_t *self_p)
{
    int res;

    harness_mock_read("cond_signal(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_cond_broadcast(int res)
{
    harness_mock_write("cond_broadcast(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(cond_broadcast)(struct cond_t *self_p)
{
    int res;

    harness_mock_read("cond_broadcast(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
