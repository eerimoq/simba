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

struct hx711_driver_t hx711;

static int test_start(void)
{
    BTASSERT(hx711_module_init() == 0);
    BTASSERT(hx711_module_init() == 0);
    BTASSERT(hx711_init(&hx711,
                        &pin_d2_dev,
                        &pin_d4_dev) == 0);
    BTASSERT(hx711_start(&hx711) == 0);

    return (0);
}

static int test_read(void)
{
    int i;
    uint32_t sample;

    for (i = 0; i < 10; i++) {
        BTASSERT(hx711_read(&hx711,
                            &sample,
                            hx711_channel_gain_a_128_t) == 0);
        std_printf(FSTR("sample: %lu\r\n"), sample);
        thrd_sleep_ms(200);
    }

    return (0);
}

static int test_stop(void)
{
    BTASSERT(hx711_stop(&hx711) == 0);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_start, "test_start" },
        { test_read, "test_read" },
        { test_stop, "test_stop" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
