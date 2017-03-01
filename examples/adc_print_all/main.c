/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

struct analog_pin_t {
    struct adc_device_t *adc_dev_p;
    struct pin_device_t *pin_dev_p;
    const char *name_p;
};

#if defined(BOARD_NANO32)

struct analog_pin_t analog_pins[] = {
    /* ADC1 pins. */
    { &adc_0_dev, &pin_gpio36_dev, "gpio36" },
    { &adc_0_dev, &pin_gpio39_dev, "gpio39" },
    { &adc_0_dev, &pin_gpio32_dev, "gpio32" },
    { &adc_0_dev, &pin_gpio33_dev, "gpio33" },
    { &adc_0_dev, &pin_gpio34_dev, "gpio34" },
    { &adc_0_dev, &pin_gpio35_dev, "gpio35" },
    /* { &adc_1_dev, &pin_gpio04_dev, "gpio04" }, */
    /* { &adc_1_dev, &pin_gpio00_dev, "gpio00" }, */
    /* { &adc_1_dev, &pin_gpio02_dev, "gpio02" }, */
    /* { &adc_1_dev, &pin_gpio15_dev, "gpio15" }, */
    /* { &adc_1_dev, &pin_gpio13_dev, "gpio13" }, */
    /* { &adc_1_dev, &pin_gpio12_dev, "gpio12" }, */
    /* { &adc_1_dev, &pin_gpio14_dev, "gpio14" }, */
    /* { &adc_1_dev, &pin_gpio27_dev, "gpio27" }, */
    /* { &adc_1_dev, &pin_gpio25_dev, "gpio25" }, */
    /* { &adc_1_dev, &pin_gpio26_dev, "gpio26" } */
};

#elif defined(BOARD_ESP12E)

struct analog_pin_t analog_pins[] = {
    { &adc_0_dev,  &pin_a0_dev,  "a0" }
};

#else
#    error "Board not supported."
#endif

struct adc_driver_t adc[membersof(analog_pins)];

static void print_header()
{
    int i;

    std_printf(FSTR("+"));

    for (i = 0; i < membersof(adc); i++) {
        std_printf(FSTR("--------+"));
    }

    std_printf(FSTR("\r\n"
                    "|"));

    for (i = 0; i < membersof(adc); i++) {
        std_printf(FSTR(" %6s |"), analog_pins[i].name_p);
    }

    std_printf(FSTR("\r\n"
                    "+"));

    for (i = 0; i < membersof(adc); i++) {
        std_printf(FSTR("--------+"));
    }

    std_printf(FSTR("\r\n"));
}

int main()
{
    int i, j;
    uint16_t samples[membersof(analog_pins)];

    sys_start();
    adc_module_init();

    for (i = 0; i < membersof(adc); i++) {
        adc_init(&adc[i],
                 analog_pins[i].adc_dev_p,
                 analog_pins[i].pin_dev_p,
                 ADC_REFERENCE_VCC,
                 1);
    }

    i = 0;

    while (1) {
        if ((i % 10) == 0) {
            print_header();
        }

        std_printf(FSTR("|"));

        for (j = 0; j < membersof(adc); j++) {
            samples[j] = 0xffff;
            adc_convert(&adc[j], &samples[j], 1);
        }

        for (j = 0; j < membersof(adc); j++) {
            std_printf(FSTR(" %6d |"), (int)samples[j]);
        }

        std_printf(FSTR("\r\n"));
        thrd_sleep_ms(500);
        i++;
    }

    return (0);
}
