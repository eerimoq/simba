/**
 * @file main.c
 * @version 0.4.0
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

FS_COMMAND_DEFINE("/bootloader/start", cmd_bootloader_start);

static struct uart_driver_t uart;
static struct shell_args_t shell_args;

int cmd_bootloader_start(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p)
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

    /* Print the bootloader application information. */
    sys_set_stdout(&uart.chout);
    std_printf(FSTR("Welcome to the test application!\r\n"));

    /* Enter the application main loop. */
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_main(&shell_args);

    return (0);
}
