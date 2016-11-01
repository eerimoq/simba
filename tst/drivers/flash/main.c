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

#elif defined(ARCH_ESP) || defined(ARCH_ESP32)

extern char __rom_size;

static int test_read_write(struct harness_t *harness_p)
{
    struct flash_driver_t drv;
    char buf[32];
    uintptr_t flash_address;
    uintptr_t ram_address;

    /* Aligned start address*/
    flash_address = ((uintptr_t)&__rom_size - 0x10000);
    ram_address = (uintptr_t)&buf[0];
    ram_address &= 0xfffffffc;
    
    BTASSERT(flash_init(&drv, &flash_0_dev) == 0);
    BTASSERT(flash_erase(&drv, flash_address, 0x10000) == 0);

    /* Read and write aligned in ROM and RAM. */
    strcpy((void *)ram_address, "1234");
    BTASSERT(flash_write(&drv, flash_address, (void *)ram_address, 5) == 5);
    memset(buf, 0, sizeof(buf));
    BTASSERT(flash_read(&drv, buf, flash_address, 5) == 5);
    BTASSERT(strcmp(buf, "1234") == 0);

    /* Read and write non-aligned in ROM and aligned RAM. One is added
       to the ram address to align the chunk reads. */
    strcpy((void *)ram_address, "67890123");
    flash_address += 5;
    BTASSERT(flash_write(&drv, flash_address, (void *)ram_address, 9) == 9);
    memset(buf, 0, sizeof(buf));
    BTASSERT(flash_read(&drv,
                        (uint32_t *)(ram_address + 1),
                        flash_address, 9) == 9);
    BTASSERT(strcmp((void *)(ram_address + 1), "67890123") == 0);

    /* Read and write aligned in ROM and non-aligned RAM. */
    ram_address++;
    strcpy((void *)ram_address, "abcdefghij");
    flash_address += 19; /* 5 + 19 = 24. */
    BTASSERT(flash_write(&drv, flash_address, (void *)ram_address, 11) == 11);
    memset(buf, 0, sizeof(buf));
    BTASSERT(flash_read(&drv,
                        (uint32_t *)ram_address,
                        flash_address, 11) == 11);
    BTASSERT(strcmp((void *)ram_address, "abcdefghij") == 0);

    /* Read and write 1 byte at non-aligned ROM address. */
    ram_address = (uintptr_t)&buf[0];
    strcpy((void *)ram_address, "");
    flash_address++;
    BTASSERT(flash_write(&drv, flash_address, (void *)ram_address, 1) == 1);
    memset(buf, 0, sizeof(buf));
    BTASSERT(flash_read(&drv,
                        (uint32_t *)ram_address,
                        flash_address, 1) == 1);
    BTASSERT(strcmp((void *)ram_address, "") == 0);

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
