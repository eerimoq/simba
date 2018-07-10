/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

#ifndef __DRIVERS_DHT_H__
#define __DRIVERS_DHT_H__

#include "simba.h"

/**
 * The DHT driver struct.
 */
struct dht_driver_t {
    struct pin_device_t *pin_p;
#if CONFIG_DHT_DEBUG_LOG_MASK > -1
    struct log_object_t log;
#endif
};

/**
 * Initialize the dht module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int dht_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] pin_p Data line pin device.
 *
 * @return zero(0) or negative error code.
 */
int dht_init(struct dht_driver_t *self_p,
             struct pin_device_t *pin_p);

/**
 * Read temperature and humidity from the DHT21/22 device.
 *
 * CAUTION: This function disables interrupts for up to 5 ms, which
 *          may cause problems for other timing critical
 *          functionality.
 *
 * @param[in] self_p Driver object.
 * @param[out] temperature_p Temperature in degrees Celsius, or NULL.
 * @param[out] humidity_p Humidity in relative humidty %RH, or NULL.
 *
 * @return zero(0) or negative error code.
 */
int dht_read(struct dht_driver_t *self_p,
             float *temperature_p,
             float *humidty_p);

/**
 * Read temperature and humidity from the DHT11 device.
 *
 * CAUTION: This function disables interrupts for up to 5 ms, which
 *          may cause problems for other timing critical
 *          functionality.
 *
 * @param[in] self_p Driver object.
 * @param[out] temperature_p Temperature in degrees Celsius, or NULL.
 * @param[out] humidity_p Humidity in relative humidty %RH, or NULL.
 *
 * @return zero(0) or negative error code.
 */
int dht_read_11(struct dht_driver_t *self_p,
                float *temperature_p,
                float *humidty_p);
#endif
