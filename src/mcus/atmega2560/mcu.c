/*
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

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* Port A. 0..7 */
    { .sfr_p = &PINA, .mask = _BV(PINA0) },
    { .sfr_p = &PINA, .mask = _BV(PINA1) },
    { .sfr_p = &PINA, .mask = _BV(PINA2) },
    { .sfr_p = &PINA, .mask = _BV(PINA3) },
    { .sfr_p = &PINA, .mask = _BV(PINA4) },
    { .sfr_p = &PINA, .mask = _BV(PINA5) },
    { .sfr_p = &PINA, .mask = _BV(PINA6) },
    { .sfr_p = &PINA, .mask = _BV(PINA7) },

    /* Port B. 8..15 */
    { .sfr_p = &PINB, .mask = _BV(PINB0) },
    { .sfr_p = &PINB, .mask = _BV(PINB1) },
    { .sfr_p = &PINB, .mask = _BV(PINB2) },
    { .sfr_p = &PINB, .mask = _BV(PINB3) },
    { .sfr_p = &PINB, .mask = _BV(PINB4) },
    { .sfr_p = &PINB, .mask = _BV(PINB5) },
    { .sfr_p = &PINB, .mask = _BV(PINB6) },
    { .sfr_p = &PINB, .mask = _BV(PINB7) },

    /* Port C. 16..23 */
    { .sfr_p = &PINC, .mask = _BV(PINC0) },
    { .sfr_p = &PINC, .mask = _BV(PINC1) },
    { .sfr_p = &PINC, .mask = _BV(PINC2) },
    { .sfr_p = &PINC, .mask = _BV(PINC3) },
    { .sfr_p = &PINC, .mask = _BV(PINC4) },
    { .sfr_p = &PINC, .mask = _BV(PINC5) },
    { .sfr_p = &PINC, .mask = _BV(PINC6) },
    { .sfr_p = &PINC, .mask = _BV(PINC7) },

    /* Port D. 24..31 */
    { .sfr_p = &PIND, .mask = _BV(PIND0) },
    { .sfr_p = &PIND, .mask = _BV(PIND1) },
    { .sfr_p = &PIND, .mask = _BV(PIND2) },
    { .sfr_p = &PIND, .mask = _BV(PIND3) },
    { .sfr_p = &PIND, .mask = _BV(PIND4) },
    { .sfr_p = &PIND, .mask = _BV(PIND5) },
    { .sfr_p = &PIND, .mask = _BV(PIND6) },
    { .sfr_p = &PIND, .mask = _BV(PIND7) },

    /* Port E. 32..39 */
    { .sfr_p = &PINE, .mask = _BV(PINE0) },
    { .sfr_p = &PINE, .mask = _BV(PINE1) },
    { .sfr_p = &PINE, .mask = _BV(PINE2) },
    { .sfr_p = &PINE, .mask = _BV(PINE3) },
    { .sfr_p = &PINE, .mask = _BV(PINE4) },
    { .sfr_p = &PINE, .mask = _BV(PINE5) },
    { .sfr_p = &PINE, .mask = _BV(PINE6) },
    { .sfr_p = &PINE, .mask = _BV(PINE7) },

    /* Port F. 40..47 */
    { .sfr_p = &PINF, .mask = _BV(PINF0) },
    { .sfr_p = &PINF, .mask = _BV(PINF1) },
    { .sfr_p = &PINF, .mask = _BV(PINF2) },
    { .sfr_p = &PINF, .mask = _BV(PINF3) },
    { .sfr_p = &PINF, .mask = _BV(PINF4) },
    { .sfr_p = &PINF, .mask = _BV(PINF5) },
    { .sfr_p = &PINF, .mask = _BV(PINF6) },
    { .sfr_p = &PINF, .mask = _BV(PINF7) },

    /* Port G. 48..53 */
    { .sfr_p = &PING, .mask = _BV(PING0) },
    { .sfr_p = &PING, .mask = _BV(PING1) },
    { .sfr_p = &PING, .mask = _BV(PING2) },
    { .sfr_p = &PING, .mask = _BV(PING3) },
    { .sfr_p = &PING, .mask = _BV(PING4) },
    { .sfr_p = &PING, .mask = _BV(PING5) },

    /* Port H. 54..61 */
    { .sfr_p = &PINH, .mask = _BV(PINH0) },
    { .sfr_p = &PINH, .mask = _BV(PINH1) },
    { .sfr_p = &PINH, .mask = _BV(PINH2) },
    { .sfr_p = &PINH, .mask = _BV(PINH3) },
    { .sfr_p = &PINH, .mask = _BV(PINH4) },
    { .sfr_p = &PINH, .mask = _BV(PINH5) },
    { .sfr_p = &PINH, .mask = _BV(PINH6) },
    { .sfr_p = &PINH, .mask = _BV(PINH7) },

    /* Port I. */

    /* Port J. 62..69 */
    { .sfr_p = &PINJ, .mask = _BV(PINJ0) },
    { .sfr_p = &PINJ, .mask = _BV(PINJ1) },
    { .sfr_p = &PINJ, .mask = _BV(PINJ2) },
    { .sfr_p = &PINJ, .mask = _BV(PINJ3) },
    { .sfr_p = &PINJ, .mask = _BV(PINJ4) },
    { .sfr_p = &PINJ, .mask = _BV(PINJ5) },
    { .sfr_p = &PINJ, .mask = _BV(PINJ6) },
    { .sfr_p = &PINJ, .mask = _BV(PINJ7) },

    /* Port K. 70..77 */
    { .sfr_p = &PINK, .mask = _BV(PINK0) },
    { .sfr_p = &PINK, .mask = _BV(PINK1) },
    { .sfr_p = &PINK, .mask = _BV(PINK2) },
    { .sfr_p = &PINK, .mask = _BV(PINK3) },
    { .sfr_p = &PINK, .mask = _BV(PINK4) },
    { .sfr_p = &PINK, .mask = _BV(PINK5) },
    { .sfr_p = &PINK, .mask = _BV(PINK6) },
    { .sfr_p = &PINK, .mask = _BV(PINK7) },

    /* Port L. 78..85 */
    { .sfr_p = &PINL, .mask = _BV(PINL0) },
    { .sfr_p = &PINL, .mask = _BV(PINL1) },
    { .sfr_p = &PINL, .mask = _BV(PINL2) },
    { .sfr_p = &PINL, .mask = _BV(PINL3) },
    { .sfr_p = &PINL, .mask = _BV(PINL4) },
    { .sfr_p = &PINL, .mask = _BV(PINL5) },
    { .sfr_p = &PINL, .mask = _BV(PINL6) },
    { .sfr_p = &PINL, .mask = _BV(PINL7) },
};

struct exti_device_t exti_device[EXTI_DEVICE_MAX] = {
    { .drv_p = NULL, .pin_p = &pin_device[24], .id = 0 },
    { .drv_p = NULL, .pin_p = &pin_device[25], .id = 1 },
    { .drv_p = NULL, .pin_p = &pin_device[26], .id = 2 },
    { .drv_p = NULL, .pin_p = &pin_device[27], .id = 3 },
    { .drv_p = NULL, .pin_p = &pin_device[36], .id = 4 },
    { .drv_p = NULL, .pin_p = &pin_device[37], .id = 5 }
};

struct pcint_device_t pcint_device[PCINT_DEVICE_MAX] = {
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL },
    { .drv_p = NULL }
};

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .ss_p = &pin_device[8],
        .sck_p = &pin_device[9],
        .mosi_p = &pin_device[10],
        .miso_p = &pin_device[11],
        .mutex = {
            .is_locked = 0,
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
    { .index = 0, .pin_dev_p = &pin_device[36] }, /* timer 3 */
    { .index = 1, .pin_dev_p = &pin_device[37] }, /* timer 3 */
    { .index = 2, .pin_dev_p = &pin_device[35] }, /* timer 3 */
    { .index = 3, .pin_dev_p = &pin_device[57] }, /* timer 4 */
    { .index = 4, .pin_dev_p = &pin_device[58] }, /* timer 4 */
    { .index = 5, .pin_dev_p = &pin_device[59] }, /* timer 4 */
    { .index = 6, .pin_dev_p = &pin_device[60] }, /* timer 2 */
    { .index = 7, .pin_dev_p = &pin_device[12] }, /* timer 2 */
    { .index = 8, .pin_dev_p = &pin_device[15] }, /* timer 0 */
    { .index = 9, .pin_dev_p = &pin_device[53] }, /* timer 0 */
};

struct adc_device_t adc_device[ADC_DEVICE_MAX] = {
    { .jobs = { .head_p = NULL, .tail_p = NULL } },
};

struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
};
