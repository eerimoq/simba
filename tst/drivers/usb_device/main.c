/**
 * @file main.c
 * @version 2.0.0
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

struct usb_device_driver_t usb;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(usb_device_init(&usb, &usb_device[0]) == 0);
    BTASSERT(usb_device_start(&usb) == 0);
    BTASSERT(usb_device_stop(&usb) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
