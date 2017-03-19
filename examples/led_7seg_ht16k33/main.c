/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Google Inc.
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

struct led_7seg_ht16k33_driver_t led;
struct i2c_soft_driver_t i2c;

void counting(void)
{
    int i, mult, dot;

    for (mult = 1, dot = 3; mult <= 0x1000; mult *= 0x10, dot--) {
        led_7seg_ht16k33_show_dot(&led, dot, true);
	for (i = 0; i <= 0xf; i++) {
	    led_7seg_ht16k33_set_num(&led, i * mult, 16);
	    led_7seg_ht16k33_show_colon(&led, i % 2);
	    led_7seg_ht16k33_display(&led);
	    thrd_sleep_ms(500);
	}
        led_7seg_ht16k33_show_dot(&led, dot, false);
    }
}

void single_cycle(void)
{
    led_7seg_ht16k33_set_num(&led, 42, 10);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_set_num(&led, 1234, 10);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_show_colon(&led, true);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_show_colon(&led, false);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    for (int i = 0; i < 4; i++) {
        led_7seg_ht16k33_show_dot(&led, i, true);
        led_7seg_ht16k33_display(&led);
        thrd_sleep(1);
        led_7seg_ht16k33_show_dot(&led, i, false);
    }

    for (int i = LED_7SEG_HT16K33_BRIGHTNESS_MIN;
	 i <= LED_7SEG_HT16K33_BRIGHTNESS_MAX;
	 i++) {
        led_7seg_ht16k33_brightness(&led, i);
        thrd_sleep_ms(250);
    }

    led_7seg_ht16k33_clear(&led);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_show_colon(&led, true);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_show_colon(&led, false);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_set_num(&led, 1, 10);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_set_num(&led, 10, 10);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_set_num(&led, 100, 10);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_set_num(&led, 1000, 10);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_set_num(&led, 4321, 10);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);

    led_7seg_ht16k33_set_num(&led, 0xfefe, 16);
    led_7seg_ht16k33_display(&led);
    thrd_sleep(1);
}

int main()
{

    /* Initialization. */
    sys_start();
    led_7seg_ht16k33_module_init();
    i2c_soft_init(&i2c, &pin_d5_dev, &pin_d6_dev, 50000, 1000000, 1000);
    led_7seg_ht16k33_init(&led, &i2c, LED_7SEG_HT16K33_DEFAULT_I2C_ADDR);
    led_7seg_ht16k33_start(&led);

    std_printf("\n\n");
    std_printf(sys_get_info());
    while (1) {
	counting();
        single_cycle();
    }

    return (0);
}
