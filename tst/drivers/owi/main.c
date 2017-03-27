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
