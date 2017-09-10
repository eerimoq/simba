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
#include "pcint_mock.h"

int mock_write_pcint_module_init(int res)
{
    harness_mock_write("pcint_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pcint_module_init)()
{
    int res;

    harness_mock_read("pcint_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pcint_init(struct pcint_device_t *dev_p,
                          int trigger,
                          void (*on_interrupt)(void *arg_p,
                          void *arg_p,
                          int res)
{
    harness_mock_write("pcint_init(dev_p)",
                       dev_p,
                       sizeof(*dev_p));

    harness_mock_write("pcint_init(trigger)",
                       &trigger,
                       sizeof(trigger));

    harness_mock_write("pcint_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("pcint_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("pcint_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pcint_init)(struct pcint_driver_t *self_p,
                                            struct pcint_device_t *dev_p,
                                            int trigger,
                                            void (*on_interrupt)(void *arg_p,
                                            void *arg_p)
{
    int res;

    harness_mock_assert("pcint_init(dev_p)",
                        dev_p);

    harness_mock_assert("pcint_init(trigger)",
                        &trigger);

    harness_mock_assert("pcint_init(arg_p)",
                        arg_p);

    harness_mock_assert("pcint_init(arg_p)",
                        arg_p);

    harness_mock_read("pcint_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pcint_start(int res)
{
    harness_mock_write("pcint_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pcint_start)(struct pcint_driver_t *self_p)
{
    int res;

    harness_mock_read("pcint_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_pcint_stop(int res)
{
    harness_mock_write("pcint_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(pcint_stop)(struct pcint_driver_t *self_p)
{
    int res;

    harness_mock_read("pcint_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
