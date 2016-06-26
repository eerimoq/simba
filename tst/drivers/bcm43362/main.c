/**
 * @file main.c
 * @version 1.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
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
                           &sdio_device[0]) == 0);
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
    uart_module_init();
    sdio_module_init();
    bcm43362_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
