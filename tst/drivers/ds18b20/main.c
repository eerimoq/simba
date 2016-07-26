/**
 * @file main.c
 * @version 3.1.0
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

#define BEDROOM_ID  { 0x28, 0xf2, 0x6a, 0x5d, 0x05, 0x00, 0x00, 0xde }
#define OUTDOORS_ID { 0x28, 0x09, 0x1e, 0xa3, 0x05, 0x00, 0x00, 0x42 }

int test_get_temp(struct harness_t *harness_p)
{
    struct owi_driver_t owi;
    struct ds18b20_driver_t ds;
    struct owi_device_t devices[4];
    char buf[24];

    BTASSERT(owi_init(&owi, &pin_d7_dev, devices, membersof(devices)) == 0);
    BTASSERT(ds18b20_init(&ds, &owi) == 0);

    time_busy_wait_us(50000);

    strcpy(buf, "drivers/ds18b20/list");
    BTASSERT(fs_call(buf, NULL, sys_get_stdout(), NULL) == 0);

    time_busy_wait_us(50000);

    BTASSERT(owi_search(&owi) == 2);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_get_temp, "test_get_temp" },
        { NULL, NULL }
    };

    sys_start();
    ds18b20_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
