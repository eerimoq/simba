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

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* PIN B */
    { .sfr_p = &PINB, .mask = _BV(PINB0) },
    { .sfr_p = &PINB, .mask = _BV(PINB1) },
    { .sfr_p = &PINB, .mask = _BV(PINB2) },
    { .sfr_p = &PINB, .mask = _BV(PINB3) },
    { .sfr_p = &PINB, .mask = _BV(PINB4) },
    { .sfr_p = &PINB, .mask = _BV(PINB5) },
    { .sfr_p = &PINB, .mask = _BV(PINB6) },
    { .sfr_p = &PINB, .mask = _BV(PINB7) },

    /* PIN C */
    { .sfr_p = &PINC, .mask = _BV(PINC6) },
    { .sfr_p = &PINC, .mask = _BV(PINC7) },

    /* PIN D */
    { .sfr_p = &PIND, .mask = _BV(PIND0) },
    { .sfr_p = &PIND, .mask = _BV(PIND1) },
    { .sfr_p = &PIND, .mask = _BV(PIND2) },
    { .sfr_p = &PIND, .mask = _BV(PIND3) },
    { .sfr_p = &PIND, .mask = _BV(PIND4) },
    { .sfr_p = &PIND, .mask = _BV(PIND5) },
    { .sfr_p = &PIND, .mask = _BV(PIND6) },
    { .sfr_p = &PIND, .mask = _BV(PIND7) },

    /* PIN E */
    { .sfr_p = &PINE, .mask = _BV(PINE6) },

    /* PIN F */
    { .sfr_p = &PINE, .mask = _BV(PINF0) },
    { .sfr_p = &PINE, .mask = _BV(PINF1) },
    { .sfr_p = &PINE, .mask = _BV(PINF4) },
    { .sfr_p = &PINE, .mask = _BV(PINF5) },
    { .sfr_p = &PINE, .mask = _BV(PINF6) },
    { .sfr_p = &PINE, .mask = _BV(PINF7) }
};

struct exti_device_t exti_device[EXTI_DEVICE_MAX] = {
    { .drv_p = NULL, .pin_p = &pin_device[2], .id = 0 },
    { .drv_p = NULL, .pin_p = &pin_device[3], .id = 1 }
};

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .ss_p = &pin_device[0],
        .sck_p = &pin_device[1],
        .mosi_p = &pin_device[2],
        .miso_p = &pin_device[3],
        .sem = { .count = 0, .count_max = 1, .head_p = NULL }
    }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    { .drv_p = NULL, .sfr_p = &UCSR1A }
};

struct pwm_device_t pwm_device[PWM_DEVICE_MAX] = {
    { .index = 0, .pin_dev_p = &pin_device[9] },  /* timer1 */
    { .index = 1, .pin_dev_p = &pin_device[10] }, /* timer1 */
    { .index = 2, .pin_dev_p = &pin_device[11] }, /* timer2 */
    { .index = 3, .pin_dev_p = &pin_device[3] }   /* timer2 */
};

struct adc_device_t adc_device[ADC_DEVICE_MAX] = {
    { .jobs = { .head_p = NULL, .tail_p = NULL } },
};

struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
    { .drv_p = NULL }
};

struct usb_device_t usb_device[USB_DEVICE_MAX] = {
    { .drv_p = NULL }
};
