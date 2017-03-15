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

#ifndef __DRIVERS_DS3231_H__
#define __DRIVERS_DS3231_H__

#include "simba.h"

struct ds3231_driver_t {
    struct i2c_driver_t *i2c_p;
};

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] i2c_p I2C driver to use.
 *
 * @return zero(0) or negative error code.
 */
int ds3231_init(struct ds3231_driver_t *self_p,
                struct i2c_driver_t *i2c_p);

/**
 * Set date in the DS3231 device.
 *
 * @param[in] self_p Driver object.
 * @param[in] date_p Date to set in the device.
 *
 * @return zero(0) or negative error code.
 */
int ds3231_set_date(struct ds3231_driver_t *self_p,
                    struct date_t *date_p);

/**
 * Get date from the DS3231 device.
 *
 * @param[in] self_p Driver object.
 * @param[out] date_p Date read from the device.
 *
 * @return zero(0) or negative error code.
 */
int ds3231_get_date(struct ds3231_driver_t *self_p,
                    struct date_t *date_p);

#endif
