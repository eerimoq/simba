/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Google Inc.
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
#include "led_7seg_ht16k33_mock.h"

int mock_write_led_7seg_ht16k33_module_init(int res)
{
    harness_mock_write("led_7seg_ht16k33_module_init()",
                       NULL,
                       0);

    harness_mock_write("led_7seg_ht16k33_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_module_init)()
{
    int res;

    harness_mock_assert("led_7seg_ht16k33_module_init()",
                        NULL,
                        0);

    harness_mock_read("led_7seg_ht16k33_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_init(struct i2c_soft_driver_t *i2c_p,
                                     int i2c_addr,
                                     int res)
{
    harness_mock_write("led_7seg_ht16k33_init(i2c_p)",
                       i2c_p,
                       sizeof(*i2c_p));

    harness_mock_write("led_7seg_ht16k33_init(i2c_addr)",
                       &i2c_addr,
                       sizeof(i2c_addr));

    harness_mock_write("led_7seg_ht16k33_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_init)(struct led_7seg_ht16k33_driver_t *self_p,
                                                       struct i2c_soft_driver_t *i2c_p,
                                                       int i2c_addr)
{
    int res;

    harness_mock_assert("led_7seg_ht16k33_init(i2c_p)",
                        i2c_p,
                        sizeof(*i2c_p));

    harness_mock_assert("led_7seg_ht16k33_init(i2c_addr)",
                        &i2c_addr,
                        sizeof(i2c_addr));

    harness_mock_read("led_7seg_ht16k33_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_start(int res)
{
    harness_mock_write("led_7seg_ht16k33_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_start)(struct led_7seg_ht16k33_driver_t *self_p)
{
    int res;

    harness_mock_read("led_7seg_ht16k33_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_display(int res)
{
    harness_mock_write("led_7seg_ht16k33_display(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_display)(struct led_7seg_ht16k33_driver_t *self_p)
{
    int res;

    harness_mock_read("led_7seg_ht16k33_display(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_clear(int res)
{
    harness_mock_write("led_7seg_ht16k33_clear(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_clear)(struct led_7seg_ht16k33_driver_t *self_p)
{
    int res;

    harness_mock_read("led_7seg_ht16k33_clear(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_brightness(int brightness,
                                           int res)
{
    harness_mock_write("led_7seg_ht16k33_brightness(brightness)",
                       &brightness,
                       sizeof(brightness));

    harness_mock_write("led_7seg_ht16k33_brightness(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_brightness)(struct led_7seg_ht16k33_driver_t *self_p,
                                                             int brightness)
{
    int res;

    harness_mock_assert("led_7seg_ht16k33_brightness(brightness)",
                        &brightness,
                        sizeof(brightness));

    harness_mock_read("led_7seg_ht16k33_brightness(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_set_num(int num,
                                        int base,
                                        int res)
{
    harness_mock_write("led_7seg_ht16k33_set_num(num)",
                       &num,
                       sizeof(num));

    harness_mock_write("led_7seg_ht16k33_set_num(base)",
                       &base,
                       sizeof(base));

    harness_mock_write("led_7seg_ht16k33_set_num(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_set_num)(struct led_7seg_ht16k33_driver_t *self_p,
                                                          int num,
                                                          int base)
{
    int res;

    harness_mock_assert("led_7seg_ht16k33_set_num(num)",
                        &num,
                        sizeof(num));

    harness_mock_assert("led_7seg_ht16k33_set_num(base)",
                        &base,
                        sizeof(base));

    harness_mock_read("led_7seg_ht16k33_set_num(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_show_colon(int show_colon,
                                           int res)
{
    harness_mock_write("led_7seg_ht16k33_show_colon(show_colon)",
                       &show_colon,
                       sizeof(show_colon));

    harness_mock_write("led_7seg_ht16k33_show_colon(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_show_colon)(struct led_7seg_ht16k33_driver_t *self_p,
                                                             int show_colon)
{
    int res;

    harness_mock_assert("led_7seg_ht16k33_show_colon(show_colon)",
                        &show_colon,
                        sizeof(show_colon));

    harness_mock_read("led_7seg_ht16k33_show_colon(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_led_7seg_ht16k33_show_dot(int position,
                                         int show_colon,
                                         int res)
{
    harness_mock_write("led_7seg_ht16k33_show_dot(position)",
                       &position,
                       sizeof(position));

    harness_mock_write("led_7seg_ht16k33_show_dot(show_colon)",
                       &show_colon,
                       sizeof(show_colon));

    harness_mock_write("led_7seg_ht16k33_show_dot(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(led_7seg_ht16k33_show_dot)(struct led_7seg_ht16k33_driver_t *self_p,
                                                           int position,
                                                           int show_colon)
{
    int res;

    harness_mock_assert("led_7seg_ht16k33_show_dot(position)",
                        &position,
                        sizeof(position));

    harness_mock_assert("led_7seg_ht16k33_show_dot(show_colon)",
                        &show_colon,
                        sizeof(show_colon));

    harness_mock_read("led_7seg_ht16k33_show_dot(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
