/**
 * @file main.c
 * @version 4.0.0
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

int test_search(struct harness_t *harness_p)
{
    struct owi_driver_t owi;
    struct owi_device_t devices[4];
    int i;

    BTASSERT(owi_init(&owi, &pin_d7_dev, devices, membersof(devices)) == 0);

    thrd_sleep_us(50000);

    std_printf(FSTR("Searching...\r\n"));

    std_printf(FSTR("Number of devices found on one wire bus: %d\r\n"),
               owi_search(&owi));

    for (i = 0; i < owi.len; i++) {
      std_printf(FSTR("id[%d]: %02x %02x %02x %02x %02x %02x %02x %02x\r\n"),
                 i,
                 (unsigned int)owi.devices_p[i].id[0],
                 (unsigned int)owi.devices_p[i].id[1],
                 (unsigned int)owi.devices_p[i].id[2],
                 (unsigned int)owi.devices_p[i].id[3],
                 (unsigned int)owi.devices_p[i].id[4],
                 (unsigned int)owi.devices_p[i].id[5],
                 (unsigned int)owi.devices_p[i].id[6],
                 (unsigned int)owi.devices_p[i].id[7]);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_search, "test_search" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
