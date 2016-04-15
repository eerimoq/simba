/**
 * @file bootloader/console.h
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

#ifndef __BOOTLOADER_CONSOLE_H__
#define __BOOTLOADER_CONSOLE_H__

#include "simba.h"

/**
 * Initialize the bootloader object.
 *
 * @param[in,out] self_p Bootloader object.
 * @param[in] chin_p Input channel.
 * @param[in] chout_p Output channel.
 * @param[in] application_address Start address of the application
 *                                flash area and the application entry
 *                                address.
 * @param[in] application_size Size of the application flash area. The
 *                             maximum appliction size is this size
 *                             minus one, because the last byte is
 *                             used as application valid flag.
 * @param[in] flash_p Flash memory to store the application in.
 *
 * @returns zero(0) or negative error code.
 */
int console_init(chan_t *chin_p,
                 chan_t *chout_p,
                 uint32_t application_address,
                 uint32_t application_size,
                 struct flash_driver_t *flash_p);

/**
 * Run the main loop of the bootloader.
 *
 * @param[in] self_p Bootloader object.
 *
 * @returns Never returns.
 */
void console_main();

#endif
