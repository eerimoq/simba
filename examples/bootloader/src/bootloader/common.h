/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
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
