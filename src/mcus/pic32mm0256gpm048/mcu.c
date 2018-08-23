/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* Port A. */
    { .regs_p = PIC32MM_PORTA, .mask = BIT(0) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(1) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(2) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(3) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(4) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(6) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(7) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(8) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(9) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(10) },
    { .regs_p = PIC32MM_PORTA, .mask = BIT(15) },

    /* Port B. */
    { .regs_p = PIC32MM_PORTB, .mask = BIT(0) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(1) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(2) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(3) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(4) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(5) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(6) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(7) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(8) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(9) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(10) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(11) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(12) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(13) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(14) },
    { .regs_p = PIC32MM_PORTB, .mask = BIT(15) },

    /* Port C. */
    { .regs_p = PIC32MM_PORTC, .mask = BIT(0) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(1) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(2) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(3) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(4) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(6) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(7) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(8) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(9) },
    { .regs_p = PIC32MM_PORTC, .mask = BIT(12) },

    /* Port D. */
    { .regs_p = PIC32MM_PORTD, .mask = BIT(0) }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    { .drv_p = NULL, .regs_p = PIC32MM_UART1 },
    { .drv_p = NULL, .regs_p = PIC32MM_UART2 },
    { .drv_p = NULL, .regs_p = PIC32MM_UART3 }
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX] = {
    { .regs_p = PIC32MM_FLASH }
};

struct i2c_device_t i2c_device[I2C_DEVICE_MAX];
