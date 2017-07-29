/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

#ifndef __DRIVERS_HX711_H__
#define __DRIVERS_HX711_H__

#include "simba.h"

enum hx711_channel_gain_t {
    hx711_channel_gain_a_128_t = 1,
    hx711_channel_gain_b_32_t,
    hx711_channel_gain_a_64_t
};

/* The HX711 driver. */
struct hx711_driver_t {
    struct pin_device_t *pd_sck_p;
    struct pin_device_t *dout_p;
    float scale;
    float offset;
};

/**
 * Initialize the hx711 module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int hx711_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[in,out] self_p Driver object to initialize.
 * @param[in] pd_sck_p PD_SCK pin device.
 * @param[in] dout_p DOUT pin device.
 * @param[in] scale Scale value to multiply with read samples after
 *                  the offset has been added.
 * @param[in] offset Offset value to add to read samples before they
 *                   are scaled.
 *
 * @return zero(0) or negative error code.
 */
int hx711_init(struct hx711_driver_t *self_p,
               struct pin_device_t *pd_sck_p,
               struct pin_device_t *dout_p,
               float scale,
               float offset);

/**
 * Start the driver by configuring the pins and resetting the HX711.
 *
 * @param[in] self_p Driver object to start.
 *
 * @return zero(0) or negative error code.
 */
int hx711_start(struct hx711_driver_t *self_p);

/**
 * Stop given driver by setting pins as input.
 *
 * @param[in] self_p Driver object to stop.
 *
 * @return zero(0) or negative error code.
 */
int hx711_stop(struct hx711_driver_t *self_p);

/**
 * Read a offsetted and scaled weight from given channel and gain
 * combination.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] weight_p Measured offsetted and scaled weight.
 * @param[in] channel_gain Channel and gain combination.
 *
 * @return zero(0) or negative error code.
 */
int hx711_read(struct hx711_driver_t *self_p,
               float *weight_p,
               enum hx711_channel_gain_t channel_gain);

/**
 * Read a sample from given channel and gain combination and output
 * the sign extended raw read value. No offsetting or scaling is
 * performed.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] sample_p Sign extended read sample.
 * @param[in] channel_gain Channel and gain combination.
 *
 * @return zero(0) or negative error code.
 */
int hx711_read_raw(struct hx711_driver_t *self_p,
                   int32_t *sample_p,
                   enum hx711_channel_gain_t channel_gain);

/**
 * Set the scale value.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] scale Scale value to multiply with read samples after
 *                  the offset has been added.
 *
 * @return zero(0) or negative error code.
 */
int hx711_set_scale(struct hx711_driver_t *self_p,
                    float scale);

/**
 * Set the offset value.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] offset Offset value to add to read samples before they
 *                   are scaled.
 *
 * @return zero(0) or negative error code.
 */
int hx711_set_offset(struct hx711_driver_t *self_p,
                     float offset);

#endif
