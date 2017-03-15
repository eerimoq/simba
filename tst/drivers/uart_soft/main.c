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

struct uart_soft_driver_t uart_soft;
static char rxbuf[32];

static int test_write(struct harness_t *harness_p)
{
    uint8_t i;
    int baudrate;

    for (baudrate = 9600; baudrate <= 230400; baudrate += 9600) {
        BTASSERT(uart_soft_init(&uart_soft,
                                &pin_d1_dev,
                                &pin_d2_dev,
                                &exti_device[2],
                                baudrate,
                                rxbuf,
                                sizeof(rxbuf)) == 0);

        for (i = 0; i < 50; i++) {
            BTASSERT(uart_soft_write(&uart_soft, &i, 1) == 1);
        }

        thrd_sleep_ms(50);
    }

    return (0);
}

static int test_echo(struct harness_t *harness_p)
{
    uint8_t i;

    BTASSERT(uart_soft_init(&uart_soft,
                            &pin_d1_dev,
                            &pin_d2_dev,
                            &exti_device[2],
                            9600,
                            rxbuf,
                            sizeof(rxbuf)) == 0);

    std_printf(FSTR("Reading one byte from the software UART.\r\n"));
    
    BTASSERT(uart_soft_read(&uart_soft, &i, 1) == 1);
    BTASSERT(uart_soft_write(&uart_soft, &i, 1) == 1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_write, "test_write" },
        { test_echo, "test_echo" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
