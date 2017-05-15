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

#define pin_scl_dev pin_d5_dev
#define pin_sca_dev pin_d6_dev

int test_led_brightness(struct harness_t *harness_p)
{
    struct led_7seg_ht16k33_driver_t led;
    struct i2c_soft_driver_t i2c;

    BTASSERT(i2c_soft_init(&i2c, &pin_scl_dev, &pin_sca_dev, 50000, 1000000,
			   1000) == 0);
    BTASSERT(led_7seg_ht16k33_init(&led, &i2c,
				   LED_7SEG_HT16K33_DEFAULT_I2C_ADDR) == 0);
    BTASSERT(led_7seg_ht16k33_start(&led) == 0);

    // This isn't pretty, but otherwise we get very long lines.
    BTASSERT(LED_7SEG_HT16K33_BRIGHTNESS_MIN == 0);
    BTASSERT(LED_7SEG_HT16K33_BRIGHTNESS_MAX == 0xf);

    std_printf("NOTE: ASSERT errors are expected at this point\r\n");
    BTASSERT(led_7seg_ht16k33_brightness(&led, -1) == -EINVAL);
    BTASSERT(led_7seg_ht16k33_brightness(&led, 0) == 0);
    BTASSERT(led_7seg_ht16k33_brightness(&led, 0xf) == 0);
    BTASSERT(led_7seg_ht16k33_brightness(&led, 0x10) == -EINVAL);

    return (0);
}

int test_led_dot(struct harness_t *harness_p)
{
    struct led_7seg_ht16k33_driver_t led;
    struct i2c_soft_driver_t i2c;
    uint8_t digit;

    BTASSERT(i2c_soft_init(&i2c, &pin_scl_dev, &pin_sca_dev, 50000, 1000000,
			   1000) == 0);
    BTASSERT(led_7seg_ht16k33_init(&led, &i2c,
				   LED_7SEG_HT16K33_DEFAULT_I2C_ADDR) == 0);
    BTASSERT(led_7seg_ht16k33_start(&led) == 0);

    // We set number before adding / removing dots to verify we will
    // not nuke them.
    BTASSERT(led_7seg_ht16k33_set_num(&led, 4444, 10) == 0);
    digit = led.buf[0];
    BTASSERT(digit != 0);
    BTASSERT(led.buf[0] == digit);
    BTASSERT(led.buf[1] == digit);
    BTASSERT(led.buf[3] == digit);
    BTASSERT(led.buf[4] == digit);

    BTASSERT(led_7seg_ht16k33_show_dot(&led, 0, true) == 0);
    BTASSERT(led.buf[0] != digit);
    BTASSERT(led.buf[1] == digit);
    BTASSERT(led.buf[3] == digit);
    BTASSERT(led.buf[4] == digit);
    BTASSERT(led_7seg_ht16k33_show_dot(&led, 0, false) == 0);
    BTASSERT(led.buf[0] == digit);
    BTASSERT(led.buf[1] == digit);
    BTASSERT(led.buf[3] == digit);
    BTASSERT(led.buf[4] == digit);

    BTASSERT(led_7seg_ht16k33_show_dot(&led, 3, true) == 0);
    BTASSERT(led.buf[0] == digit);
    BTASSERT(led.buf[1] == digit);
    BTASSERT(led.buf[3] == digit);
    BTASSERT(led.buf[4] != digit);
    BTASSERT(led_7seg_ht16k33_show_dot(&led, 3, false) == 0);
    BTASSERT(led.buf[0] == digit);
    BTASSERT(led.buf[1] == digit);
    BTASSERT(led.buf[3] == digit);
    BTASSERT(led.buf[4] == digit);

    std_printf("NOTE: ASSERT errors are expected at this point\r\n");
    BTASSERT(led_7seg_ht16k33_show_dot(&led, -1, true) == -EINVAL);
    BTASSERT(led_7seg_ht16k33_show_dot(&led, 0, true) == 0);
    BTASSERT(led_7seg_ht16k33_show_dot(&led, 1, true) == 0);
    BTASSERT(led_7seg_ht16k33_show_dot(&led, 2, true) == 0);
    BTASSERT(led_7seg_ht16k33_show_dot(&led, 3, true) == 0);
    BTASSERT(led_7seg_ht16k33_show_dot(&led, 4, true) == -EINVAL);

    return (0);
}

int test_led_colon(struct harness_t *harness_p)
{
    struct led_7seg_ht16k33_driver_t led;
    struct i2c_soft_driver_t i2c;

    BTASSERT(i2c_soft_init(&i2c, &pin_scl_dev, &pin_sca_dev, 50000, 1000000,
			   1000) == 0);
    BTASSERT(led_7seg_ht16k33_init(&led, &i2c,
				   LED_7SEG_HT16K33_DEFAULT_I2C_ADDR) == 0);
    BTASSERT(led_7seg_ht16k33_start(&led) == 0);

    BTASSERT(led.buf[0] == 0);

    BTASSERT(led_7seg_ht16k33_show_colon(&led, true) == 0);
    BTASSERT(led.buf[2] != 0);

    BTASSERT(led_7seg_ht16k33_show_colon(&led, false) == 0);
    BTASSERT(led.buf[2] == 0);

    return (0);
}

int test_led_send_nums(struct harness_t *harness_p)
{
    struct led_7seg_ht16k33_driver_t led;
    struct i2c_soft_driver_t i2c;

    BTASSERT(i2c_soft_init(&i2c, &pin_scl_dev, &pin_sca_dev, 50000, 1000000,
			   1000) == 0);
    BTASSERT(led_7seg_ht16k33_init(&led, &i2c,
				   LED_7SEG_HT16K33_DEFAULT_I2C_ADDR) == 0);
    BTASSERT(led_7seg_ht16k33_start(&led) == 0);

    // Make sure buffer is empty from start.
    for (int i = 0; i < sizeof(led.buf); i++) {
	BTASSERT(led.buf[i] == 0);
    }

    BTASSERT(led_7seg_ht16k33_set_num(&led, 42, 10) == 0);
    BTASSERT(led_7seg_ht16k33_display(&led) == 0);
    time_busy_wait_us(500000); // 0.5s

    BTASSERT(led_7seg_ht16k33_set_num(&led, 4242, 10) == 0);
    BTASSERT(led_7seg_ht16k33_display(&led) == 0);
    time_busy_wait_us(500000); // 0.5s

    BTASSERT(led_7seg_ht16k33_set_num(&led, 10000, 10) == -ERANGE);
    BTASSERT(led_7seg_ht16k33_set_num(&led, 10000, 16) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_led_send_nums, "led_send_nums" },
        { test_led_brightness, "test_led_brightness" },
        { test_led_dot, "test_led_dot" },
        { test_led_colon, "test_led_colon" },
        { NULL, NULL }
    };

    sys_start();
    led_7seg_ht16k33_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
