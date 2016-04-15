/**
 * @file common.c
 * @version 0.3.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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
#include "bootloader.h"

/* Application valid flag. */
#define APPLICATION_VALID_FLAG                             0xbe

/**
 * Erase the application memory region.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns zero(0) or negative error code.
 */
int bootloader_erase_application(size_t address,
                                 size_t size,
                                 struct flash_driver_t *flash_p)
{
    uint8_t flag;
    uint32_t flag_address;

    /* Erase the flag at the end of the application flash area. */
    flag = 0xff;
    flag_address = (address + size);
    flash_write(flash_p, flag_address, &flag, sizeof(flag));

    return (0);
}

/**
 * Returns true(1) if there is a valid application in the application
 * memory region.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns true(1) if a valid application exists in the memory
 *          region, otherwise false(0).
 */
int bootloader_is_application_valid(size_t address,
                                    size_t size)
{
    uint32_t flag_address;
    uint8_t flag;

    flag_address = (address + size);

#if defined(MCU_SAM_3X8E)
    flag = *((uint8_t *)(uintptr_t)flag_address);
#else
    flag = 0;
    (void)flag_address;
#endif

    return (flag == APPLICATION_VALID_FLAG);
}

/**
 * Write the valid applicatin flag to the flash memory.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns zero(0) or negative error code.
 */
int bootloader_write_application_valid_flag(size_t address,
                                            size_t size,
                                            struct flash_driver_t *flash_p)
{
    uint8_t flag;
    uint32_t flag_address;

    /* Write the flag at the end of the application flash area. */
    flag = APPLICATION_VALID_FLAG;
    flag_address = (address + size);
    flash_write(flash_p, flag_address, &flag, sizeof(flag));

    return (0);
}

/**
 * Start application at given address.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns For most architechtures this function never returns.
 */
int bootloader_jump(size_t address)
{
#if defined(MCU_SAM_3X8E)
    uint32_t reset_address;
    uint32_t stack_address;
    uint32_t tbloff;

    sys_lock();

    /* Set the vector offset to the application vector. */
    tbloff = (address >> 7);
    SAM_SCB->VTOR = SCB_VTOR_TBLOFF(tbloff);

    /* Read the reset address. */
    reset_address = *(uint32_t *)(address + 4);

    /* Setup the stack pointer. */
    stack_address = *(uint32_t *)(address);

    /* Setup the stack pointer and call the application reset
       function. */
    asm volatile ("mov sp, %0" : : "r" (stack_address));
    asm volatile ("blx %0" : : "r" (reset_address));
#elif defined(MCU_LINUX)
#else
#    error "Unsupported mcu."
#endif

    return (0);
}
