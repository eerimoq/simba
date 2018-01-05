/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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
    struct xbee_driver_t xbee;
    struct uart_driver_t uart;
    uint8_t uart_rxbuf[128];
    struct xbee_frame_t frame;
    int res;

    sys_start();

    /* Initialize the UART driver. */
    res = uart_init(&uart,
                    &uart_device[1],
                    9600,
                    &uart_rxbuf[0],
                    sizeof(uart_rxbuf));

    if (res != 0) {
        std_printf(OSTR("Failed to initialize the UART driver.\r\n"));
        return (-1);
    }

    /* Start the UART driver. */
    res = uart_start(&uart);

    if (res != 0) {
        std_printf(OSTR("Failed to start the UART driver.\r\n"));
        return (-1);
    }

    /* Initialize the XBee driver. */
    res = xbee_init(&xbee, &uart, &uart);

    if (res != 0) {
        std_printf(OSTR("Failed to initialize the XBee driver.\r\n"));
        return (-1);
    }

    /* Read frames from the XBee and print them on standard output. */
    while (1) {
        res = xbee_read(&xbee, &frame);

        if (res != 0) {
            std_printf(OSTR("Read failed with %d: %S.\r\n"),
                       res,
                       errno_as_string(res));
            continue;
        }

        xbee_print_frame(sys_get_stdout(), &frame);
    }

    return (0);
}
