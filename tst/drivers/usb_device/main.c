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

static struct usb_device_driver_t usb;
static struct usb_device_driver_base_t *drivers[1];
static struct usb_device_class_cdc_driver_t cdc;
static uint8_t rxbuf[32];

static int test_init(struct harness_t *harness_p)
{
    char c;

    BTASSERT(usb_device_module_init() == 0);
    BTASSERT(usb_device_class_cdc_module_init() == 0);

    /* Initialize the CDC driver object. */
    BTASSERT(usb_device_class_cdc_init(&cdc,
                                       2,
                                       3,
                                       rxbuf,
                                       sizeof(rxbuf)) == 0);
    drivers[0] = &cdc.base;

    /* Initialize the USB device driver. */
    BTASSERT(usb_device_init(&usb,
                             &usb_device[0],
                             drivers,
                             membersof(drivers),
                             usb_device_descriptors,
                             usb_device_descriptor_string_language,
                             usb_device_descriptor_string_iproduct,
                             usb_device_descriptor_string_imanufacturer) == 0);
    BTASSERT(usb_device_start(&usb) == 0);

    while (1) {
        /* Write read data back to the sender. */
        if (usb_device_class_cdc_read(&cdc,
                                      &c,
                                      sizeof(c)) == sizeof(c)) {
            usb_device_class_cdc_write(&cdc, &c, sizeof(c));
        }
    }

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

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
