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
#include "mutex_mock.h"

int mock_write_mutex_module_init(int res)
{
    harness_mock_write("mutex_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mutex_module_init)()
{
    int res;

    harness_mock_read("mutex_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mutex_init(int res)
{
    harness_mock_write("mutex_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mutex_init)(struct mutex_t *self_p)
{
    int res;

    harness_mock_read("mutex_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mutex_lock(int res)
{
    harness_mock_write("mutex_lock(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mutex_lock)(struct mutex_t *self_p)
{
    int res;

    harness_mock_read("mutex_lock(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mutex_unlock(int res)
{
    harness_mock_write("mutex_unlock(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mutex_unlock)(struct mutex_t *self_p)
{
    int res;

    harness_mock_read("mutex_unlock(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mutex_lock_isr(int res)
{
    harness_mock_write("mutex_lock_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mutex_lock_isr)(struct mutex_t *self_p)
{
    int res;

    harness_mock_read("mutex_lock_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mutex_unlock_isr(int res)
{
    harness_mock_write("mutex_unlock_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mutex_unlock_isr)(struct mutex_t *self_p)
{
    int res;

    harness_mock_read("mutex_unlock_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
