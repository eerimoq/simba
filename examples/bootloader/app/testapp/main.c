/**
 * @file main.c
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
