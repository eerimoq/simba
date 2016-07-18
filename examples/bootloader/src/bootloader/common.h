/**
 * @file bootloader/common.h
 * @version 2.0.0
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

#ifndef __BOOTLOADER_COMMON_H__
#define __BOOTLOADER_COMMON_H__

#include "simba.h"

/**
 * Erase the application memory region.
 *
 * @param[in] address Application start address.
 * @param[in] size Application size.
 * @param[in] flash_p Flash memory driver.
 *
 * @returns zero(0) or negative error code.
 */
int bootloader_erase_application(size_t address,
                                 size_t size,
                                 struct flash_driver_t *flash_p);

/**
 * Returns true(1) if there is a valid application in the application
 * memory region.
 *
 * @param[in] address Application start address.
 * @param[in] size Application size.
 *
 * @returns true(1) if a valid application exists in the memory
 *          region, otherwise false(0).
 */
int bootloader_is_application_valid(size_t address,
                                    size_t size);

/**
 * Write the valid applicatin flag to the flash memory.
 *
 * @param[in] address Application start address.
 * @param[in] size Application size.
 * @param[in] flash_p Flash memory driver.
 *
 * @returns zero(0) or negative error code.
 */
int bootloader_write_application_valid_flag(size_t address,
                                            size_t size,
                                            struct flash_driver_t *flash_p);

/**
 * Jump to application at given address.
 *
 * @param[in] address Application start address.
 *
 * @returns For most architechtures this function never returns.
 */
int bootloader_jump(size_t address);

#endif
