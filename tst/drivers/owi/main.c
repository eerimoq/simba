/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
