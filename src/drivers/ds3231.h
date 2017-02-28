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
