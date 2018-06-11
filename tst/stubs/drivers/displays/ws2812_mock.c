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
#include "ws2812_mock.h"

int mock_write_ws2812_module_init(int res)
{
    harness_mock_write("ws2812_module_init()",
                       NULL,
                       0);

    harness_mock_write("ws2812_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ws2812_module_init)()
{
    int res;

    harness_mock_assert("ws2812_module_init()",
                        NULL,
                        0);

    harness_mock_read("ws2812_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ws2812_init(struct pin_device_t **pin_devices_pp,
                           int number_of_pin_devices,
                           int res)
{
    harness_mock_write("ws2812_init(pin_devices_pp)",
                       pin_devices_pp,
                       sizeof(*pin_devices_pp));

    harness_mock_write("ws2812_init(number_of_pin_devices)",
                       &number_of_pin_devices,
                       sizeof(number_of_pin_devices));

    harness_mock_write("ws2812_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ws2812_init)(struct ws2812_driver_t *self_p,
                                             struct pin_device_t **pin_devices_pp,
                                             int number_of_pin_devices)
{
    int res;

    harness_mock_assert("ws2812_init(pin_devices_pp)",
                        pin_devices_pp,
                        sizeof(*pin_devices_pp));

    harness_mock_assert("ws2812_init(number_of_pin_devices)",
                        &number_of_pin_devices,
                        sizeof(number_of_pin_devices));

    harness_mock_read("ws2812_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ws2812_write(const uint8_t *colors_p,
                            int number_of_pixles,
                            int res)
{
    harness_mock_write("ws2812_write(colors_p)",
                       colors_p,
                       sizeof(*colors_p));

    harness_mock_write("ws2812_write(number_of_pixles)",
                       &number_of_pixles,
                       sizeof(number_of_pixles));

    harness_mock_write("ws2812_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ws2812_write)(struct ws2812_driver_t *self_p,
                                              const uint8_t *colors_p,
                                              int number_of_pixles)
{
    int res;

    harness_mock_assert("ws2812_write(colors_p)",
                        colors_p,
                        sizeof(*colors_p));

    harness_mock_assert("ws2812_write(number_of_pixles)",
                        &number_of_pixles,
                        sizeof(number_of_pixles));

    harness_mock_read("ws2812_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
