/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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

#if CONFIG_START_CONSOLE == CONFIG_START_CONSOLE_UART

struct module_t {
    int initialized;
    struct {
        struct uart_driver_t uart;
        char rxbuf[CONFIG_START_CONSOLE_UART_RX_BUFFER_SIZE];
    } console;
};

static struct module_t module;

int console_module_init(void)
{
    return (uart_module_init());
}

int console_init(void)
{
    return (uart_init(&module.console.uart,
                      &uart_device[CONFIG_START_CONSOLE_DEVICE_INDEX],
                      CONFIG_START_CONSOLE_UART_BAUDRATE,
                      module.console.rxbuf,
                      sizeof(module.console.rxbuf)));
}

int console_start(void)
{
    return (uart_start(&module.console.uart));
}

int console_stop(void)
{
    return (uart_stop(&module.console.uart));
}

int console_set_input_channel(void *chan_p)
{
    return (-1);
}

void *console_get_input_channel(void)
{
    return (&module.console.uart.chin);
}

void *console_set_output_channel(void *chan_p)
{
    return (NULL);
}

void *console_get_output_channel()
{
    return (&module.console.uart.chout);
}

#elif CONFIG_START_CONSOLE == CONFIG_START_CONSOLE_USB_CDC

struct module_t {
    int initialized;
    struct {
        struct usb_device_driver_t usb;
        struct usb_device_driver_base_t *drivers[1];
        struct usb_device_class_cdc_driver_t cdc;
        uint8_t rxbuf[32];
    } console;
};

static struct module_t module;

int console_module_init(void)
{
    usb_device_module_init();
    usb_device_class_cdc_module_init();

    return (0);
}

int console_init(void)
{
    /* Initialize the CDC driver object. */
    usb_device_class_cdc_init(&module.console.cdc,
                              0,
                              2,
                              3,
                              module.console.rxbuf,
                              sizeof(module.console.rxbuf));
    module.console.drivers[0] = &module.console.cdc.base;

    /* Initialize the USB device driver. */
    usb_device_init(&module.console.usb,
                    &usb_device[0],
                    module.console.drivers,
                    membersof(module.console.drivers),
                    usb_device_descriptors);

    return (0);
}

int console_start(void)
{
    usb_device_start(&module.console.usb);

#if CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION == 1

    /* Wait for the host to connect. */
    while (usb_device_class_cdc_is_connected(&module.console.cdc) == 0) {
        thrd_sleep_us(100000);
    }

    thrd_sleep_us(100000);

#endif

    return (0);
}

int console_stop(void)
{
    return (usb_device_stop(&module.console.usb));
}

int console_set_input_channel(void *chan_p)
{
    return (-1);
}

void *console_get_input_channel(void)
{
    return (&module.console.cdc.chin);
}

void *console_set_output_channel(void *chan_p)
{
    return (NULL);
}

void *console_get_output_channel(void)
{
    return (&module.console.cdc.chout);
}

#elif CONFIG_START_CONSOLE == CONFIG_START_CONSOLE_NONE

struct module_t {
    int initialized;
    struct {
        void *chin_p;
        void *chout_p;
    } console;
};

static struct module_t module;

int console_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;
    module.console.chin_p = chan_null();
    module.console.chout_p = chan_null();

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

int console_set_input_channel(void *chan_p)
{
    module.console.chin_p = chan_p;

    return (0);
}

void *console_get_input_channel(void)
{
    return (module.console.chin_p);
}

void *console_set_output_channel(void *chan_p)
{
    module.console.chout_p = chan_p;

    return (0);
}

void *console_get_output_channel(void)
{
    return (module.console.chout_p);
}

#else
#    error "Bad console."
#endif
