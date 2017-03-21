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

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    { .sfr_p = &PINE, .mask = _BV(PINE0) }, /* D0 */
    { .sfr_p = &PINE, .mask = _BV(PINE1) }, /* D1 */
    { .sfr_p = &PINE, .mask = _BV(PINE4) }, /* D2, PWM0, EXIT4 */
    { .sfr_p = &PINE, .mask = _BV(PINE5) }, /* D3, PWM1, EXIT5 */
    { .sfr_p = &PING, .mask = _BV(PING5) }, /* D4, PWM2 */
    { .sfr_p = &PINE, .mask = _BV(PINE3) }, /* D5, PWM3 */
    { .sfr_p = &PINH, .mask = _BV(PINH3) }, /* D6, PWM4 */
    { .sfr_p = &PINH, .mask = _BV(PINH4) }, /* D7, PWM5 */
    { .sfr_p = &PINH, .mask = _BV(PINH5) }, /* D8, PWM6 */
    { .sfr_p = &PINH, .mask = _BV(PINH6) }, /* D9, PWM7 */
    { .sfr_p = &PINB, .mask = _BV(PINB4) }, /* D10, PWM8 */
    { .sfr_p = &PINB, .mask = _BV(PINB5) }, /* D11, PWM9 */
    { .sfr_p = &PINB, .mask = _BV(PINB6) }, /* D12, PWM10 */
    { .sfr_p = &PINB, .mask = _BV(PINB7) }, /* D13, PWM11 */
    { .sfr_p = &PINJ, .mask = _BV(PINJ1) }, /* D14 */
    { .sfr_p = &PINJ, .mask = _BV(PINJ0) }, /* D15 */
    { .sfr_p = &PINH, .mask = _BV(PINH1) }, /* D16 */
    { .sfr_p = &PINH, .mask = _BV(PINH0) }, /* D17 */
    { .sfr_p = &PIND, .mask = _BV(PIND3) }, /* D18, EXIT3 */
    { .sfr_p = &PIND, .mask = _BV(PIND2) }, /* D19, EXIT2 */
    { .sfr_p = &PIND, .mask = _BV(PIND1) }, /* D20, EXIT1 */
    { .sfr_p = &PIND, .mask = _BV(PIND0) }, /* D21, EXIT0 */
    { .sfr_p = &PINA, .mask = _BV(PINA0) }, /* D22 */
    { .sfr_p = &PINA, .mask = _BV(PINA1) }, /* D23 */
    { .sfr_p = &PINA, .mask = _BV(PINA2) }, /* D24 */
    { .sfr_p = &PINA, .mask = _BV(PINA3) }, /* D25 */
    { .sfr_p = &PINA, .mask = _BV(PINA4) }, /* D26 */
    { .sfr_p = &PINA, .mask = _BV(PINA5) }, /* D27 */
    { .sfr_p = &PINA, .mask = _BV(PINA6) }, /* D28 */
    { .sfr_p = &PINA, .mask = _BV(PINA7) }, /* D29 */
    { .sfr_p = &PINC, .mask = _BV(PINC7) }, /* D30 */
    { .sfr_p = &PINC, .mask = _BV(PINC6) }, /* D31 */
    { .sfr_p = &PINC, .mask = _BV(PINC5) }, /* D32 */
    { .sfr_p = &PINC, .mask = _BV(PINC4) }, /* D33 */
    { .sfr_p = &PINC, .mask = _BV(PINC3) }, /* D34 */
    { .sfr_p = &PINC, .mask = _BV(PINC2) }, /* D35 */
    { .sfr_p = &PINC, .mask = _BV(PINC1) }, /* D36 */
    { .sfr_p = &PINC, .mask = _BV(PINC0) }, /* D37 */
    { .sfr_p = &PIND, .mask = _BV(PIND7) }, /* D38 */
    { .sfr_p = &PING, .mask = _BV(PING2) }, /* D39 */
    { .sfr_p = &PING, .mask = _BV(PING1) }, /* D40 */
    { .sfr_p = &PING, .mask = _BV(PING0) }, /* D41 */
    { .sfr_p = &PINL, .mask = _BV(PINL7) }, /* D42 */
    { .sfr_p = &PINL, .mask = _BV(PINL6) }, /* D43 */
    { .sfr_p = &PINL, .mask = _BV(PINL5) }, /* D44 */
    { .sfr_p = &PINL, .mask = _BV(PINL4) }, /* D45 */
    { .sfr_p = &PINL, .mask = _BV(PINL3) }, /* D46 */
    { .sfr_p = &PINL, .mask = _BV(PINL2) }, /* D47 */
    { .sfr_p = &PINL, .mask = _BV(PINL1) }, /* D48 */
    { .sfr_p = &PINL, .mask = _BV(PINL0) }, /* D49 */
    { .sfr_p = &PINB, .mask = _BV(PINB3) }, /* D50 */
    { .sfr_p = &PINB, .mask = _BV(PINB2) }, /* D51 */
    { .sfr_p = &PINB, .mask = _BV(PINB1) }, /* D52 */
    { .sfr_p = &PINB, .mask = _BV(PINB0) }, /* D53 */
    { .sfr_p = &PINF, .mask = _BV(PINF0) }, /* D54 */
    { .sfr_p = &PINF, .mask = _BV(PINF1) }, /* D55 */
    { .sfr_p = &PINF, .mask = _BV(PINF2) }, /* D56 */
    { .sfr_p = &PINF, .mask = _BV(PINF3) }, /* D57 */
    { .sfr_p = &PINF, .mask = _BV(PINF4) }, /* D58 */
    { .sfr_p = &PINF, .mask = _BV(PINF5) }, /* D59 */
    { .sfr_p = &PINF, .mask = _BV(PINF6) }, /* D60 */
    { .sfr_p = &PINF, .mask = _BV(PINF7) }, /* D61 */
    { .sfr_p = &PINK, .mask = _BV(PINK0) }, /* D62 */
    { .sfr_p = &PINK, .mask = _BV(PINK1) }, /* D63 */
    { .sfr_p = &PINK, .mask = _BV(PINK2) }, /* D64 */
    { .sfr_p = &PINK, .mask = _BV(PINK3) }, /* D65 */
    { .sfr_p = &PINK, .mask = _BV(PINK4) }, /* D66 */
    { .sfr_p = &PINK, .mask = _BV(PINK5) }, /* D67 */
    { .sfr_p = &PINK, .mask = _BV(PINK6) }, /* D68 */
    { .sfr_p = &PINK, .mask = _BV(PINK7) }, /* D69 */
};

struct exti_device_t exti_device[EXTI_DEVICE_MAX] = {
    { .drv_p = NULL, .pin_p = &pin_device[21], .id = 0 },
    { .drv_p = NULL, .pin_p = &pin_device[20], .id = 1 },
    { .drv_p = NULL, .pin_p = &pin_device[19], .id = 2 },
    { .drv_p = NULL, .pin_p = &pin_device[18], .id = 3 },
    { .drv_p = NULL, .pin_p = &pin_device[2],  .id = 4 },
    { .drv_p = NULL, .pin_p = &pin_device[3],  .id = 5 }
};

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .ss_p = &pin_device[53],
        .sck_p = &pin_device[52],
        .mosi_p = &pin_device[51],
        .miso_p = &pin_device[50],
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    { .drv_p = NULL, .sfr_p = &UCSR0A },
    { .drv_p = NULL, .sfr_p = &UCSR1A },
    { .drv_p = NULL, .sfr_p = &UCSR2A },
    { .drv_p = NULL, .sfr_p = &UCSR3A }
};

struct pwm_device_t pwm_device[PWM_DEVICE_MAX] = {
    { .index = 0, .pin_dev_p = &pin_device[2] }, /* timer 3 */
    { .index = 1, .pin_dev_p = &pin_device[3] }, /* timer 3 */
    { .index = 2, .pin_dev_p = &pin_device[5] }, /* timer 3 */
    { .index = 3, .pin_dev_p = &pin_device[6] }, /* timer 4 */
    { .index = 4, .pin_dev_p = &pin_device[7] }, /* timer 4 */
    { .index = 5, .pin_dev_p = &pin_device[8] }, /* timer 4 */
    { .index = 6, .pin_dev_p = &pin_device[9] }, /* timer 2 */
    { .index = 7, .pin_dev_p = &pin_device[10] }, /* timer 2 */
    { .index = 8, .pin_dev_p = &pin_device[11] }, /* timer 1 */
    { .index = 9, .pin_dev_p = &pin_device[12] }, /* timer 1 */
};

struct adc_device_t adc_device[ADC_DEVICE_MAX] = {
    { .jobs = { .head_p = NULL, .tail_p = NULL } },
};

struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
};
