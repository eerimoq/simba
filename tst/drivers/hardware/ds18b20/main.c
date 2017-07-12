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

static struct owi_driver_t owi;
static struct ds18b20_driver_t ds;
static struct owi_device_t devices[4];

int test_init(struct harness_t *harness_p)
{
    BTASSERT(owi_init(&owi, &pin_d7_dev, devices, membersof(devices)) == 0);
    BTASSERT(ds18b20_init(&ds, &owi) == 0);

    time_busy_wait_us(50000);

    return (0);
}

int test_scan(struct harness_t *harness_p)
{
    BTASSERTI(owi_search(&owi), ==, 2);

    return (0);
}

int test_convert(struct harness_t *harness_p)
{
    BTASSERT(ds18b20_convert(&ds) == 0);

    return (0);
}

int test_read(struct harness_t *harness_p)
{
    int i;
    float temperature;

    for (i = 0; i < 2; i++) {
        BTASSERT(ds18b20_read(&ds,
                              &devices[i].id[0],
                              &temperature) == 0);
        std_printf(OSTR("temperature: %f\r\n"), temperature);
        BTASSERT(temperature > 0);
    }

    return (0);
}

int test_read_fixed_point(struct harness_t *harness_p)
{
    int i;
    int temperature;

    for (i = 0; i < 2; i++) {
        BTASSERT(ds18b20_read_fixed_point(&ds,
                                          &devices[i].id[0],
                                          &temperature) == 0);
        std_printf(OSTR("temperature: 0x%x\r\n"), temperature);
        BTASSERT(temperature > 0);
    }

    return (0);
}

int test_read_string(struct harness_t *harness_p)
{
    int i;
    char temperature[16];

    for (i = 0; i < 2; i++) {
        BTASSERT(ds18b20_read_string(&ds,
                                     &devices[i].id[0],
                                     &temperature[0]) != NULL);
        std_printf(OSTR("temperature: %s\r\n"), &temperature[0]);
    }

    return (0);
}

int test_command_list(struct harness_t *harness_p)
{
    char buf[24];

    strcpy(&buf[0], "drivers/ds18b20/list");
    BTASSERT(fs_call(&buf[0], NULL, sys_get_stdout(), NULL) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_scan, "test_scan" },
        { test_convert, "test_convert" },
        { test_read, "test_read" },
        { test_read_fixed_point, "test_read_fixed_point" },
        { test_read_string, "test_read_string" },
        { test_command_list, "test_command_list" },
        { NULL, NULL }
    };

    sys_start();
    ds18b20_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
