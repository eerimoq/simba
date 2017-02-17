/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#if !defined(SSID)
#    pragma message "WiFi connection variable SSID is not set. Using default value MySSID"
#    define SSID MySSID
#endif

#if !defined(PASSWORD)
#    pragma message "WiFi connection variable PASSWORD is not set. Using default value MyPassword"
#    define PASSWORD MyPassword
#endif

static struct bcm43362_driver_t bcm43362;

static int test_start(struct harness_t *harness_p)
{
    BTASSERT(bcm43362_init(&bcm43362,
                           &sdio_0_dev) == 0);
    BTASSERT(bcm43362_start(&bcm43362) == 0);

    return (0);
}

static int test_connect(struct harness_t *harness_p)
{
    BTASSERT(bcm43362_connect(&bcm43362,
                              STRINGIFY(SSID),
                              STRINGIFY(PASSWORD)) == 0);

    return (0);
}

static int test_write(struct harness_t *harness_p)
{
    BTASSERT(bcm43362_write(&bcm43362, NULL, 3) == 3);

    return (0);
}

static int test_disconnect(struct harness_t *harness_p)
{
    BTASSERT(bcm43362_disconnect(&bcm43362) == 0);

    return (0);
}

static int test_stop(struct harness_t *harness_p)
{
    BTASSERT(bcm43362_stop(&bcm43362) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_start, "test_start" },
        { test_connect, "test_connect" },
        { test_write, "test_write" },
        { test_disconnect, "test_disconnect" },
        { test_stop, "test_stop" },
        { NULL, NULL }
    };

    sys_start();
    sdio_module_init();
    bcm43362_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
