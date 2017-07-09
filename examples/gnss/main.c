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

static struct gnss_driver_t gnss;
static struct uart_driver_t uart;
static uint8_t uart_rxbuf[128];

int main()
{
    int res;
    struct time_t timeout;

    sys_start();

    /* Initialization of the UART (device index 1) and GNSS
       drivers. */
    gnss_module_init();
    uart_init(&uart,
              &uart_device[1],
              9600,
              &uart_rxbuf[0],
              sizeof(uart_rxbuf));
    chan_control(&uart, CHAN_CONTROL_NON_BLOCKING_READ);
    uart_start(&uart);
    gnss_init(&gnss, &uart);

    timeout.seconds = 1;
    timeout.nanoseconds = 0;

    std_printf(OSTR("Waiting for GNSS data...\r\n"));

    while (1) {
        /* Read and decode GNSS sentences. */
        if (chan_poll(&uart, &timeout) != NULL) {
            res = gnss_read(&gnss);

            /* Non-blocking channels returns -EAGAIN when a read would
               block. */
            if (res == -EAGAIN) {
                continue;
            }

            if (res != 0) {
                std_printf(OSTR("gnss_read() failed with %d: %S.\r\n\r\n"),
                           res,
                           errno_as_string(res));
            }
        }

        /* Print GNSS information on standard output. */
        gnss_print(&gnss, sys_get_stdout());
        std_printf(OSTR("\r\n"));
    }

    return (0);
}
