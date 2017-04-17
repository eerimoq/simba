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

static struct pin_driver_t pin;
static struct pwm_driver_t pwm;
static struct can_driver_t can;
static uint8_t canrxbuf[128];
static struct uart_driver_t uart;
static uint8_t uartrxbuf[128];

int main()
{
    struct can_frame_t frame;
    uint8_t byte;
    long frequency;
    long duty_cycle;

    /* Start the system. */
    sys_start();

    /* Initialize the pin driver. */
    pin_init(&pin, &pin_device[6], PIN_OUTPUT);
    pin_write(&pin, 0);

    /* Initialize the pwm driver. */
    pwm_init(&pwm, &pwm_device[3], 1, 1);
    pwm_start(&pwm);
    frequency = 100;
    duty_cycle = 0;

    /* Initialize a CAN driver. */
    can_init(&can,
             &can_device[0],
             CAN_SPEED_250KBPS,
             &canrxbuf[0],
             sizeof(canrxbuf));
    can_start(&can);
    frame.id = 5;
    frame.size = 4;
    frame.data.u32[0] = htonl(0x12345678);

    /* Initialize a UART driver. */
    uart_init(&uart,
              &uart_device[3],
              115200,
              &uartrxbuf[0],
              sizeof(uartrxbuf));
    uart_start(&uart);
    byte = 0;

    while (1) {
        /* Wait half a second. */
        thrd_sleep_ms(500);

        /* Toggle the pin. */
        pin_toggle(&pin);

        /* Change PWM output. */
        pwm_set_frequency(&pwm, frequency);
        pwm_set_duty_cycle(&pwm, duty_cycle);
        frequency += 100;
        duty_cycle += 5;
        duty_cycle %= 101;

        /* Write a CAN frame. */
        can_write(&can, &frame, sizeof(frame));
        frame.id++;

        /* Write to the UART. */
        uart_write(&uart, &byte, sizeof(byte));
        byte++;
    }

    return (0);
}
