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
#include "rwlock_mock.h"

int mock_write_rwlock_module_init(int res)
{
    harness_mock_write("rwlock_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_module_init)()
{
    int res;

    harness_mock_read("rwlock_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_rwlock_init(int res)
{
    harness_mock_write("rwlock_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_init)(struct rwlock_t *self_p)
{
    int res;

    harness_mock_read("rwlock_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_rwlock_reader_take(int res)
{
    harness_mock_write("rwlock_reader_take(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_reader_take)(struct rwlock_t *self_p)
{
    int res;

    harness_mock_read("rwlock_reader_take(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_rwlock_reader_give(int res)
{
    harness_mock_write("rwlock_reader_give(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_reader_give)(struct rwlock_t *self_p)
{
    int res;

    harness_mock_read("rwlock_reader_give(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_rwlock_reader_give_isr(int res)
{
    harness_mock_write("rwlock_reader_give_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_reader_give_isr)(struct rwlock_t *self_p)
{
    int res;

    harness_mock_read("rwlock_reader_give_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_rwlock_writer_take(int res)
{
    harness_mock_write("rwlock_writer_take(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_writer_take)(struct rwlock_t *self_p)
{
    int res;

    harness_mock_read("rwlock_writer_take(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_rwlock_writer_give(int res)
{
    harness_mock_write("rwlock_writer_give(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_writer_give)(struct rwlock_t *self_p)
{
    int res;

    harness_mock_read("rwlock_writer_give(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_rwlock_writer_give_isr(int res)
{
    harness_mock_write("rwlock_writer_give_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(rwlock_writer_give_isr)(struct rwlock_t *self_p)
{
    int res;

    harness_mock_read("rwlock_writer_give_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
