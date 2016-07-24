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
static struct usb_device_driver_base_t *drivers[2];
static struct usb_device_class_cdc_driver_t cdc[2];
static uint8_t rxbuf[2][32];

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(usb_device_module_init() == 0);
    BTASSERT(usb_device_class_cdc_module_init() == 0);
    
    /* Initialize the first CDC driver object. */
    BTASSERT(usb_device_class_cdc_init(&cdc[0],
                                       0,
                                       2,
                                       3,
                                       &rxbuf[0][0],
                                       sizeof(rxbuf[0])) == 0);
    drivers[0] = &cdc[0].base;

    /* Initialize the second CDC driver object. */
    BTASSERT(usb_device_class_cdc_init(&cdc[1],
                                       2,
                                       5,
                                       6,
                                       &rxbuf[1][0],
                                       sizeof(rxbuf[0])) == 0);
    drivers[1] = &cdc[1].base;

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

    return (0);
}

static int test_echo(struct harness_t *harness_p)
{
    char c;
    struct chan_list_t list;
    int worksapce[32];
    chan_t *chan_p;

    /* Poll both channels. */
    chan_list_init(&list, worksapce, sizeof(worksapce));
    chan_list_add(&list, &cdc[0].chin);
    chan_list_add(&list, &cdc[1].chin);

    while (1) {
        /* Wait for data to be available. */
        chan_p = chan_list_poll(&list, NULL);

        /* Read a byte and write it back the sender. */
        if (chan_p == &cdc[0].chin) {
            chan_read(&cdc[0].chin, &c, sizeof(c));
            chan_write(&cdc[0].chout, &c, sizeof(c));
        } else if (chan_p == &cdc[1].chin) {
            chan_read(&cdc[1].chin, &c, sizeof(c));
            chan_write(&cdc[1].chout, &c, sizeof(c));
        }
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_echo, "test_echo" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
