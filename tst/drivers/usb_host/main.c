/**
 * @file main.c
 * @version 3.0.0
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

struct usb_host_driver_t usb;
static struct usb_host_device_t host_devices[1];

static int test_list_devices(struct harness_t *harness_p)
{
    BTASSERT(usb_host_init(&usb,
                           &usb_device[0],
                           host_devices,
                           membersof(host_devices)) == 0);
    BTASSERT(usb_host_start(&usb) == 0);

    std_printf(FSTR("ADDRESS  CLASS  VENDOR  PRODUCT\r\n"));

    BTASSERT(usb_host_stop(&usb) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_list_devices, "test_list_devices" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
