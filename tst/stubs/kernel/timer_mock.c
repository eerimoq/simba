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
#include "timer_mock.h"

int mock_write_timer_module_init(int res)
{
    harness_mock_write("timer_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(timer_module_init)()
{
    int res;

    harness_mock_read("timer_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_timer_init(const struct time_t *timeout_p,
                          timer_callback_t callback,
                          void *arg_p,
                          int flags,
                          int res)
{
    harness_mock_write("timer_init(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("timer_init(callback)",
                       &callback,
                       sizeof(callback));

    harness_mock_write("timer_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("timer_init(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("timer_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(timer_init)(struct timer_t *self_p,
                                            const struct time_t *timeout_p,
                                            timer_callback_t callback,
                                            void *arg_p,
                                            int flags)
{
    int res;

    harness_mock_assert("timer_init(timeout_p)",
                        timeout_p);

    harness_mock_assert("timer_init(callback)",
                        &callback);

    harness_mock_assert("timer_init(arg_p)",
                        arg_p);

    harness_mock_assert("timer_init(flags)",
                        &flags);

    harness_mock_read("timer_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_timer_start(int res)
{
    harness_mock_write("timer_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(timer_start)(struct timer_t *self_p)
{
    int res;

    harness_mock_read("timer_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_timer_start_isr(int res)
{
    harness_mock_write("timer_start_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(timer_start_isr)(struct timer_t *self_p)
{
    int res;

    harness_mock_read("timer_start_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_timer_stop(int res)
{
    harness_mock_write("timer_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(timer_stop)(struct timer_t *self_p)
{
    int res;

    harness_mock_read("timer_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_timer_stop_isr(int res)
{
    harness_mock_write("timer_stop_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(timer_stop_isr)(struct timer_t *self_p)
{
    int res;

    harness_mock_read("timer_stop_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
