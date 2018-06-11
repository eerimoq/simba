/*
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
#include "watchdog_mock.h"

int mock_write_watchdog_module_init(int res)
{
    harness_mock_write("watchdog_module_init()",
                       NULL,
                       0);

    harness_mock_write("watchdog_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(watchdog_module_init)()
{
    int res;

    harness_mock_assert("watchdog_module_init()",
                        NULL,
                        0);

    harness_mock_read("watchdog_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_watchdog_start_ms(int timeout,
                                 watchdog_isr_fn_t on_interrupt,
                                 int res)
{
    harness_mock_write("watchdog_start_ms(timeout)",
                       &timeout,
                       sizeof(timeout));

    harness_mock_write("watchdog_start_ms(on_interrupt)",
                       &on_interrupt,
                       sizeof(on_interrupt));

    harness_mock_write("watchdog_start_ms(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(watchdog_start_ms)(int timeout,
                                                   watchdog_isr_fn_t on_interrupt)
{
    int res;

    harness_mock_assert("watchdog_start_ms(timeout)",
                        &timeout,
                        sizeof(timeout));

    harness_mock_assert("watchdog_start_ms(on_interrupt)",
                        &on_interrupt,
                        sizeof(on_interrupt));

    harness_mock_read("watchdog_start_ms(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_watchdog_stop(int res)
{
    harness_mock_write("watchdog_stop()",
                       NULL,
                       0);

    harness_mock_write("watchdog_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(watchdog_stop)()
{
    int res;

    harness_mock_assert("watchdog_stop()",
                        NULL,
                        0);

    harness_mock_read("watchdog_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_watchdog_kick(int res)
{
    harness_mock_write("watchdog_kick()",
                       NULL,
                       0);

    harness_mock_write("watchdog_kick(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(watchdog_kick)()
{
    int res;

    harness_mock_assert("watchdog_kick()",
                        NULL,
                        0);

    harness_mock_read("watchdog_kick(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
