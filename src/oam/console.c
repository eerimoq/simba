/**
 * @file console.c
 * @version 4.1.0
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
                      &uart_device[CONFIG_CONSOLE_DEVICE_INDEX],
                      CONFIG_CONSOLE_UART_BAUDRATE,
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

int console_set_input_channel(chan_t *chan_p)
{
    return (-1);
}

chan_t *console_get_input_channel(void)
{
    return (&console.uart.chin);
}

chan_t *console_set_output_channel(chan_t *chan_p)
{
    return (NULL);
}

chan_t *console_get_output_channel()
{
    return (&console.uart.chout);
}

#elif CONFIG_CONSOLE == CONFIG_CONSOLE_USB_CDC

struct console_t {
    struct usb_device_driver_t usb;
    struct usb_device_driver_base_t *drivers[1];
    struct usb_device_class_cdc_driver_t cdc;
    uint8_t rxbuf[32];
};

static struct console_t console;

int console_module_init(void)
{
    usb_device_module_init();
    usb_device_class_cdc_module_init();

    return (0);
}

int console_init(void)
{
    /* Initialize the CDC driver object. */
    usb_device_class_cdc_init(&console.cdc,
                              0,
                              2,
                              3,
                              console.rxbuf,
                              sizeof(console.rxbuf));
    console.drivers[0] = &console.cdc.base;

    /* Initialize the USB device driver. */
    usb_device_init(&console.usb,
                    &usb_device[0],
                    console.drivers,
                    membersof(console.drivers),
                    usb_device_descriptors);

    return (0);
}

int console_start(void)
{
    usb_device_start(&console.usb);

#if CONFIG_CONSOLE_USB_CDC_WAIT_FOR_CONNETION == 1

    /* Wait for the host to connect. */
    while (usb_device_class_cdc_is_connected(&console.cdc) == 0) {
        thrd_sleep_us(100000);
    }

    thrd_sleep_us(100000);

#endif

    return (0);
}

int console_stop(void)
{
    return (usb_device_stop(&console.usb));
}

int console_set_input_channel(chan_t *chan_p)
{
    return (-1);
}

chan_t *console_get_input_channel(void)
{
    return (&console.cdc.chin);
}

chan_t *console_set_output_channel(chan_t *chan_p)
{
    return (NULL);
}

chan_t *console_get_output_channel(void)
{
    return (&console.cdc.chout);
}

#elif CONFIG_CONSOLE == CONFIG_CONSOLE_NONE

struct console_t {
    chan_t *chin_p;
    chan_t *chout_p;
};

static struct console_t console;

int console_module_init(void)
{
    console.chin_p = NULL;
    console.chout_p = NULL;

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

int console_set_input_channel(chan_t *chan_p)
{
    console.chin_p = chan_p;

    return (0);
}

chan_t *console_get_input_channel(void)
{
    return (console.chin_p);
}

chan_t *console_set_output_channel(chan_t *chan_p)
{
    console.chout_p = chan_p;

    return (0);
}

chan_t *console_get_output_channel(void)
{
    return (console.chout_p);
}

#else
#    error "Bad console."
#endif
