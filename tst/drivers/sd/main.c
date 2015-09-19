/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

static int test_read_write(struct harness_t *harness_p)
{
    struct sd_driver_t sd;
    char buf[512];

#if defined(ARCH_LINUX)
    BTASSERT(system("./create_sdcard_linux.sh") == 0);
#endif

    BTASSERT(sd_init(&sd) == 0);

    /* Read the first block. */
    BTASSERT(sd_read(&sd, buf, 0) == SD_BLOCK_SIZE);
    BTASSERT(memcmp(buf, "1 2 3", 5) == 0);
    BTASSERT(memcmp(&buf[500], "153 154 155 ", 12) == 0);

    /* Read the second block. */
    BTASSERT(sd_read(&sd, buf, 1) == SD_BLOCK_SIZE);
    BTASSERT(memcmp(buf, "156 157 158", 11) == 0);
    BTASSERT(memcmp(&buf[500], "281 282 283 ", 12) == 0);

    /* Read the fifth block. */
    BTASSERT(sd_read(&sd, buf, 4) == SD_BLOCK_SIZE);
    BTASSERT(memcmp(buf, "540 541 542", 11) == 0);
    BTASSERT(memcmp(&buf[500], "665 666 667 ", 12) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
