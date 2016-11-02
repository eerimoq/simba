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

#if !defined(BOOTLOADER_ADDRESS)
#    define BOOTLOADER_ADDRESS 0x00080000
#endif

static struct fs_command_t bootloader_start;

static struct uart_driver_t uart;
static struct shell_t shell;

static int cmd_bootloader_start(int argc,
                                const char *argv[],
                                void *out_p,
                                void *in_p,
                                void *arg_p,
                                void *call_arg_p)
{
    bootloader_jump(BOOTLOADER_ADDRESS);

    return (0);
}

int main()
{
    /* Start the system. */
    sys_start();

    /* Initialize and start the UART. */
    uart_module_init();
    uart_init(&uart,
              &uart_device[0],
              115200,
              NULL,
              0);
    uart_start(&uart);

    fs_command_init(&bootloader_start,
                    FSTR("/bootloader/start"),
                    cmd_bootloader_start,
                    NULL);
    fs_command_register(&bootloader_start);


    /* Print the bootloader application information. */
    sys_set_stdout(&uart.chout);
    std_printf(FSTR("Welcome to the test application!\r\n"));

    /* Enter the application main loop. */
    shell_init(&shell, &uart.chin, &uart.chout, NULL, NULL, NULL, NULL);
    shell_main(&shell);

    return (0);
}
