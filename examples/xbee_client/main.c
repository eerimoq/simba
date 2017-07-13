/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

static struct xbee_client_t client;
static uint8_t frame_rxbuf[256];
static THRD_STACK(client_stack, 512);
static struct uart_driver_t uart;
static uint8_t uart_rxbuf[128];

static int init()
{
    int res;

    /* Initialize the UART driver. */
    res = uart_init(&uart,
                    &uart_device[1],
                    9600,
                    &uart_rxbuf[0],
                    sizeof(uart_rxbuf));

    if (res != 0) {
        std_printf(OSTR("Failed to initialize the UART driver.\r\n"));
        return (res);
    }

    /* Start the UART driver. */
    res = uart_start(&uart);

    if (res != 0) {
        std_printf(OSTR("Failed to start the UART driver.\r\n"));
        return (res);
    }

    /* Initialize the XBee client. */
    res = xbee_client_init(&client,
                           &uart.chin,
                           &uart.chout,
                           &frame_rxbuf[0],
                           sizeof(frame_rxbuf),
                           XBEE_CLIENT_NON_BLOCKING_READ);

    if (res != 0) {
        std_printf(OSTR("Failed to initialize the XBee client.\r\n"));
        return (res);
    }

    thrd_spawn(xbee_client_main,
               &client,
               0,
               &client_stack[0],
               sizeof(client_stack));

    return (res);
}

int main()
{
    int res;
    ssize_t size;
    uint8_t buf[XBEE_DATA_MAX];
    struct xbee_client_address_t sender;

    sys_start();

    res = init();

    if (res != 0) {
        return (res);
    }
    
    /* Configure the two pins.*/
    xbee_client_pin_set_mode(&client,
                             XBEE_PIN_DIO0,
                             XBEE_CLIENT_PIN_OUTPUT);
    xbee_client_pin_set_mode(&client,
                             XBEE_PIN_AD1,
                             XBEE_CLIENT_PIN_ADC);

    /* Toggle one pin, convert the other, and try to read a data
       packet. */
    while (1) {
        res = xbee_client_pin_toggle(&client, XBEE_PIN_DIO0);

        if (res != 0) {
            std_printf(OSTR("Toggle failed with %d: %S\r\n"),
                       res,
                       errno_as_string(res));
        }

        res = xbee_client_pin_convert(&client, XBEE_PIN_AD1);

        if (res >= 0) {
            std_printf(OSTR("ADC sample: %d\r\n"), res);
        } else {
            std_printf(OSTR("Convert failed with %d: %S\r\n"),
                       res,
                       errno_as_string(res));
        }

        size = xbee_client_read_from(&client,
                                     &buf[0],
                                     sizeof(buf),
                                     &sender);

        if (size > 0) {
            xbee_client_print_address(sys_get_stdout(), &sender);
            std_hexdump(sys_get_stdout(), &buf[0], size);
        } else {
            std_printf(OSTR("Read failed with %d: %S\r\n"),
                       size,
                       errno_as_string(size));
        }

        std_printf(OSTR("\r\n"));

        thrd_sleep(0.5);
    }

    return (0);
}
