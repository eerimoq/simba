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
#include "exti_mock.h"

int mock_write_exti_module_init(int res)
{
    harness_mock_write("exti_module_init()",
                       NULL,
                       0);

    harness_mock_write("exti_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(exti_module_init)()
{
    int res;

    harness_mock_assert("exti_module_init()",
                        NULL,
                        0);

    harness_mock_read("exti_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_exti_init(struct exti_device_t *dev_p,
                         int trigger,
                         void (*on_interrupt)(void *arg_p,
                         void *arg_p,
                         int res)
{
    harness_mock_write("exti_init(dev_p)",
                       &dev_p,
                       sizeof(dev_p));

    harness_mock_write("exti_init(trigger)",
                       &trigger,
                       sizeof(trigger));

    harness_mock_write("exti_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("exti_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("exti_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(exti_init)(struct exti_driver_t *self_p,
                                           struct exti_device_t *dev_p,
                                           int trigger,
                                           void (*on_interrupt)(void *arg_p,
                                           void *arg_p)
{
    int res;

    harness_mock_assert("exti_init(dev_p)",
                        &dev_p,
                        sizeof(dev_p));

    harness_mock_assert("exti_init(trigger)",
                        &trigger,
                        sizeof(trigger));

    harness_mock_assert("exti_init(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_assert("exti_init(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_read("exti_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_exti_start(int res)
{
    harness_mock_write("exti_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(exti_start)(struct exti_driver_t *self_p)
{
    int res;

    harness_mock_read("exti_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_exti_stop(int res)
{
    harness_mock_write("exti_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(exti_stop)(struct exti_driver_t *self_p)
{
    int res;

    harness_mock_read("exti_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_exti_clear(int res)
{
    harness_mock_write("exti_clear(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(exti_clear)(struct exti_driver_t *self_p)
{
    int res;

    harness_mock_read("exti_clear(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
