/**
 * @file main.c
 * @version 0.4.0
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

static int test_read_write(struct harness_t *harness_p)
{
    struct flash_driver_t drv;
    char name[] = "Kalle kula";
    char buf[16];
    uint32_t address;
    int i;

    BTASSERT(flash_init(&drv, &flash_0_dev) == 0);

    /* Write and read over a page boundary. */
    address = (FLASH1_BEGIN + FLASH1_PAGE_SIZE - 2);

    BTASSERT(flash_write(&drv, address, name, sizeof(name)) == sizeof(name));

    memset(buf, 0, sizeof(buf));
    BTASSERT(flash_read(&drv, buf, address, sizeof(buf)) == sizeof(buf));

    BTASSERT(strcmp(name, buf) == 0);

    /* Write and read over a bank boundary. */
    address = (FLASH1_BEGIN - 2);

    BTASSERT(flash_write(&drv, address, name, sizeof(name)) == sizeof(name));

    memset(buf, 0, sizeof(buf));
    BTASSERT(flash_read(&drv, buf, address, sizeof(buf)) == sizeof(buf));

    BTASSERT(strcmp(name, buf) == 0);

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
    flash_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
