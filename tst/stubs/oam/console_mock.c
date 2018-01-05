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
#include "console_mock.h"

int mock_write_console_module_init(int res)
{
    harness_mock_write("console_module_init()",
                       NULL,
                       0);

    harness_mock_write("console_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(console_module_init)()
{
    int res;

    harness_mock_assert("console_module_init()",
                        NULL,
                        0);

    harness_mock_read("console_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_console_init(int res)
{
    harness_mock_write("console_init()",
                       NULL,
                       0);

    harness_mock_write("console_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(console_init)()
{
    int res;

    harness_mock_assert("console_init()",
                        NULL,
                        0);

    harness_mock_read("console_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_console_start(int res)
{
    harness_mock_write("console_start()",
                       NULL,
                       0);

    harness_mock_write("console_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(console_start)()
{
    int res;

    harness_mock_assert("console_start()",
                        NULL,
                        0);

    harness_mock_read("console_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_console_stop(int res)
{
    harness_mock_write("console_stop()",
                       NULL,
                       0);

    harness_mock_write("console_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(console_stop)()
{
    int res;

    harness_mock_assert("console_stop()",
                        NULL,
                        0);

    harness_mock_read("console_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_console_set_input_channel(void *chan_p,
                                         int res)
{
    harness_mock_write("console_set_input_channel(): return (chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("console_set_input_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(console_set_input_channel)(void *chan_p)
{
    int res;

    harness_mock_read("console_set_input_channel(): return (chan_p)",
                      chan_p,
                      sizeof(*chan_p));

    harness_mock_read("console_set_input_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_console_get_input_channel(void *res)
{
    harness_mock_write("console_get_input_channel()",
                       NULL,
                       0);

    harness_mock_write("console_get_input_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(console_get_input_channel)()
{
    void *res;

    harness_mock_assert("console_get_input_channel()",
                        NULL,
                        0);

    harness_mock_read("console_get_input_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_console_set_output_channel(void *chan_p,
                                          void *res)
{
    harness_mock_write("console_set_output_channel(): return (chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("console_set_output_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(console_set_output_channel)(void *chan_p)
{
    void *res;

    harness_mock_read("console_set_output_channel(): return (chan_p)",
                      chan_p,
                      sizeof(*chan_p));

    harness_mock_read("console_set_output_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_console_get_output_channel(void *res)
{
    harness_mock_write("console_get_output_channel()",
                       NULL,
                       0);

    harness_mock_write("console_get_output_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(console_get_output_channel)()
{
    void *res;

    harness_mock_assert("console_get_output_channel()",
                        NULL,
                        0);

    harness_mock_read("console_get_output_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
