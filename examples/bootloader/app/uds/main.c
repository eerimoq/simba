/**
 * @file main.c
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

#include "simba.h"

#include "bootloader.h"

#if !defined(APPLICATION_ADDRESS)
#    define APPLICATION_ADDRESS                      0x00000000
#endif

#if !defined(APPLICATION_SIZE)
#    define APPLICATION_SIZE                         0x20000000
#endif

int main()
{
    struct bootloader_uds_t uds;
    struct flash_driver_t flash;

    /* Start the system. */
    sys_start();

    /* Initialize the pin moudle. */
    pin_module_init();

    /* Initialize the flash memory objects. */
    flash_module_init();
    flash_init(&flash, &flash_device[0]);

    /* Print the bootloader application information. */
    std_printf(sys_get_info());

    /* Initialize the bootloader object and enter the main loop. */
    bootloader_uds_init(&uds,
                        console_get_input_channel(),
                        console_get_output_channel(),
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    bootloader_uds_main(&uds);

    return (0);
}
