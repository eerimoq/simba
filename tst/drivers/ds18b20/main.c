/**
 * @file main.c
 * @version 0.3.0
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
    uint8_t bedroom_id[8] = BEDROOM_ID;
    uint8_t outdoors_id[8] = OUTDOORS_ID;

    BTASSERT(owi_init(&owi, &pin_d7_dev, devices, membersof(devices)) == 0);
    BTASSERT(ds18b20_init(&ds, &owi) == 0);

    time_sleep(50000);

    BTASSERT(owi_search(&owi) == 3);

    /* Create a temperature samle in all sensors. */
    BTASSERT(ds18b20_convert(&ds) == 0);

    std_printf(FSTR("bedroom temperature: %s C"),
               ds18b20_get_temperature_str(&ds, bedroom_id, buf));

    std_printf(FSTR("outdoors temperature: %s C"),
               ds18b20_get_temperature_str(&ds, outdoors_id, buf));

    strcpy(buf, "drivers/ds18b20/list");
    fs_call(buf, NULL, sys_get_stdout());

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
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
