/**
 * @file main.c
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

#if defined(BOARD_ARDUINO_DUE)

static int test_read_write(struct harness_t *harness_p)
{
    struct flash_driver_t drv;
    char name[] = "Kalle kula";
    char buf[16];
    uint32_t address;

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

#elif defined(ARCH_ESP)

extern char __rom_end;

static int test_read_write(struct harness_t *harness_p)
{
    struct flash_driver_t drv;
    char name[] = "Kalle kula";
    char buf[16];
    uint32_t address;

    address = ((uintptr_t)&__rom_end - 0x10000);

    BTASSERT(flash_init(&drv, &flash_0_dev) == 0);
    BTASSERT(flash_erase(&drv, address, sizeof(name)) == 0);
    BTASSERT(flash_write(&drv, address, name, sizeof(name)) == sizeof(name));

    memset(buf, 0, sizeof(buf));
    BTASSERT(flash_read(&drv, buf, address, sizeof(buf)) == sizeof(buf));

    BTASSERT(strcmp(name, buf) == 0);

    return (0);
}

#else

static int test_read_write(struct harness_t *harness_p)
{
    return (1);
}

#endif

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { NULL, NULL }
    };

    sys_start();
    flash_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
