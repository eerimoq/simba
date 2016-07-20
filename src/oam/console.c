/**
 * @file console.c
 * @version 2.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

#if CONFIG_CONSOLE == CONFIG_CONSOLE_UART

struct console_t {
    struct uart_driver_t uart;
    char rxbuf[32];
};

static struct console_t console;

int console_module_init(void)
{
    return (uart_module_init());
}

int console_init(void)
{
    return (uart_init(&console.uart,
                      &uart_device[CONFIG_CONSOLE_DEVICE],
                      CONFIG_CONSOLE_BAUDRATE,
                      console.rxbuf,
                      sizeof(console.rxbuf)));
}

int console_start(void)
{
    return (uart_start(&console.uart));
}

int console_stop(void)
{
    return (uart_stop(&console.uart));
}

chan_t *console_get_input_channel(void)
{
    return (&console.uart.chin);
}

chan_t *console_get_output_channel()
{
    return (&console.uart.chout);
}

#elif CONFIG_CONSOLE == CONFIG_CONSOLE_USB_CDC

struct console_t {
    //struct usb_device_class_cdc_driver_t cdc;
};

int console_module_init(void)
{
    return (0);
}

int console_init(void)
{
    return (0);
}

int console_start(void)
{
    return (0);
}

int console_stop(void)
{
    return (0);
}

chan_t *console_get_input_channel(void)
{
    return (NULL);
}

chan_t *console_get_output_channel()
{
    return (NULL);
}

#elif CONFIG_CONSOLE == CONFIG_CONSOLE_NONE
#else
#    error "Bad console."
#endif
