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
                        sys_get_stdin(),
                        sys_get_stdout(),
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    bootloader_uds_main(&uds);

    return (0);
}
