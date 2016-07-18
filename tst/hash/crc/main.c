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

int test_crc_32(struct harness_t *harness_p)
{
    char string[] = "The quick brown fox jumps over the lazy dog";

    BTASSERT(crc_32(0, string, strlen(string)) == 0x414fa339);

    return (0);
}

int test_crc_ccitt(struct harness_t *harness_p)
{
    uint16_t crc;

    /* CCITT with 0xffff as initial value. */
    BTASSERT(crc_ccitt(0xffff, "", 0) == 0xffff);
    BTASSERT(crc_ccitt(0xffff, "1", 1) == 0xc782);
    BTASSERT(crc_ccitt(0xffff, "123456789", 9) == 0x29b1);
    crc = crc_ccitt(0xffff, "12345", 5);
    BTASSERT(crc_ccitt(crc, "6789", 4) == 0x29b1);

    /* XModem uses the same algorithm as CCITT but with 0x0000 as
       initial value. */
    BTASSERT(crc_ccitt(0x0000, "", 0) == 0x0000);
    BTASSERT(crc_ccitt(0x0000, "1", 1) == 0x2672);
    BTASSERT(crc_ccitt(0x0000, "123456789", 9) == 0x31c3);
    crc = crc_ccitt(0x0000, "12345", 5);
    BTASSERT(crc_ccitt(crc, "6789", 4) == 0x31c3);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_crc_32, "test_crc_32" },
        { test_crc_ccitt, "test_crc_ccitt" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
