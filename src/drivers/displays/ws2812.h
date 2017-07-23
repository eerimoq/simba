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

#ifndef __DRIVERS_WS2812_H__
#define __DRIVERS_WS2812_H__

#include "simba.h"

/**
 * Maximum number of pin devices a driver object can handle.
 */
#define WS2812_PIN_DEVICES_MAX                              8

struct ws2812_driver_t {
    struct pin_device_t **pins_pp;
    int number_of_pins;
    uint32_t mask;
};

/**
 * Initialize the WS2812 driver module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int ws2812_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] pin_devices_pp An array of pin device(s) to use. The
 *                           maximum length of the array is defined as
 *                           ``WS2812_PIN_DEVICES_MAX``.
 * @param[in] number_of_pin_devices Number of pin devices in the pin
 *                                  devices array.
 *
 * @return zero(0) or negative error code.
 */
int ws2812_init(struct ws2812_driver_t *self_p,
                struct pin_device_t **pin_devices_pp,
                int number_of_pin_devices);

/**
 * Write given RGB colors to the NeoPixels.
 *
 * CAUTION: Interrupts are disabled during the write to meet the
 *          strict timing requirements on the pulse train. It takes
 *          ~30 us to write to one pixel, that is, interrupts are
 *          disabled for ~30 * number_of_pixles us. Long pixel chains
 *          may cause the rest of the system to misbehave.
 *
 * @param[in] self_p Driver object.
 * @param[in] colors_p An array of GRB colors to write to the
 *                     NeoPixels. All pin devices green component
 *                     first, then all red, and last all blue,
 *                     repeated for all NeoPixels. For example, when a
 *                     single pin device is configured the array is
 *                     G0, R0, B0, G1, R1, B1, ...
 * @param[in] number_of_pixles Number of GRB colors per pin device
 *                             in `colors_p`.
 *
 * @return zero(0) or negative error code.
 */
int ws2812_write(struct ws2812_driver_t *self_p,
                 const uint8_t *colors_p,
                 int number_of_pixles);

#endif
