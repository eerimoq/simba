/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
