/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
static struct i2c_driver_t i2c;

int main()
{
    struct can_frame_t frame;
    long frequency;
    long duty_cycle;
    uint8_t uart_byte;
    uint8_t i2c_byte;
    int i2c_slave_address;

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

    /* Initialize the CAN driver. */
    can_init(&can,
             &can_device[0],
             CAN_SPEED_250KBPS,
             &canrxbuf[0],
             sizeof(canrxbuf));
    can_start(&can);
    frame.id = 5;
    frame.size = 4;
    frame.data.u32[0] = htonl(0x12345678);

    /* Initialize the UART driver. */
    uart_init(&uart,
              &uart_device[3],
              115200,
              &uartrxbuf[0],
              sizeof(uartrxbuf));
    uart_start(&uart);
    uart_byte = 0;

    /* Initialize the I2C driver. */
    i2c_init(&i2c,
             &i2c_device[2],
             I2C_BAUDRATE_3_2MBPS,
             -1);
    i2c_start(&i2c);
    i2c_slave_address = 0;
    i2c_byte = 0;

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
        uart_write(&uart, &uart_byte, sizeof(uart_byte));
        uart_byte++;

        /* Write to the I2C. */
        i2c_write(&i2c, i2c_slave_address, &i2c_byte, sizeof(i2c_byte));
        i2c_slave_address += 2;
        i2c_slave_address %= 128;
        i2c_byte++;
    }

    return (0);
}
