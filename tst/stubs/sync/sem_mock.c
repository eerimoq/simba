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
#include "sem_mock.h"

int mock_write_sem_module_init(int res)
{
    harness_mock_write("sem_module_init()",
                       NULL,
                       0);

    harness_mock_write("sem_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sem_module_init)()
{
    int res;

    harness_mock_assert("sem_module_init()",
                        NULL);

    harness_mock_read("sem_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sem_init(int count,
                        int count_max,
                        int res)
{
    harness_mock_write("sem_init(count)",
                       &count,
                       sizeof(count));

    harness_mock_write("sem_init(count_max)",
                       &count_max,
                       sizeof(count_max));

    harness_mock_write("sem_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sem_init)(struct sem_t *self_p,
                                          int count,
                                          int count_max)
{
    int res;

    harness_mock_assert("sem_init(count)",
                        &count);

    harness_mock_assert("sem_init(count_max)",
                        &count_max);

    harness_mock_read("sem_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sem_take(struct time_t *timeout_p,
                        int res)
{
    harness_mock_write("sem_take(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("sem_take(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sem_take)(struct sem_t *self_p,
                                          struct time_t *timeout_p)
{
    int res;

    harness_mock_assert("sem_take(timeout_p)",
                        timeout_p);

    harness_mock_read("sem_take(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sem_give(int count,
                        int res)
{
    harness_mock_write("sem_give(count)",
                       &count,
                       sizeof(count));

    harness_mock_write("sem_give(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sem_give)(struct sem_t *self_p,
                                          int count)
{
    int res;

    harness_mock_assert("sem_give(count)",
                        &count);

    harness_mock_read("sem_give(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sem_give_isr(int count,
                            int res)
{
    harness_mock_write("sem_give_isr(count)",
                       &count,
                       sizeof(count));

    harness_mock_write("sem_give_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sem_give_isr)(struct sem_t *self_p,
                                              int count)
{
    int res;

    harness_mock_assert("sem_give_isr(count)",
                        &count);

    harness_mock_read("sem_give_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
