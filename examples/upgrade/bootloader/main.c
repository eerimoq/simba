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

int main()
{
    static struct shell_t shell;

    sys_start();

#if BOOTLOADER_CONFIG_APP == 0
    struct pin_driver_t stay_in_bootloader_pin;

    pin_init(&stay_in_bootloader_pin, &pin_d2_dev, PIN_INPUT);

    /* Check the "stay in bootloader" pin. */
    if (pin_read(&stay_in_bootloader_pin) == 0) {
        std_printf(FSTR("stay in bootloader pin (d2) low\r\n"));

        /* Call the application if it is valid. */
        if (bootloader_application_is_valid() == 1) {
            std_printf(FSTR("calling application\r\n"));
            bootloader_application_jump();
        } else {
            std_printf(FSTR("application invalid\r\n"));
        }
    }

    std_printf(FSTR("staying in the bootloader\r\n"));
#endif

    upgrade_bootloader_start();

    shell_init(&shell,
               sys_get_stdin(),
               sys_get_stdout(),
               NULL,
               NULL,
               NULL,
               NULL);
    shell_main(&shell);

    return (0);
}
