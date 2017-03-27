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

#ifndef __DRIVERS_DS18B20_H__
#define __DRIVERS_DS18B20_H__

#include "simba.h"

/* DS18B20 one wire family code. */
#define DS18B20_FAMILY_CODE 0x28

struct ds18b20_driver_t {
    struct owi_driver_t *owi_p;
    struct ds18b20_driver_t *next_p;
};

/**
 * Initialize the DS18B20 driver module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_module_init(void);

/**
 * Initialize given driver object. The driver object will communicate
 * with all DS18B20 devices on given OWI bus.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] owi_p One-Wire (OWI) driver.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_init(struct ds18b20_driver_t *self_p,
                 struct owi_driver_t *owi_p);

/**
 * Start temperature convertion on all sensors.
 *
 * @param[in] self_p Driver object to be initialized.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_convert(struct ds18b20_driver_t *self_p);

/**
 * Get the temperature for given device identity.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] id_p Device identity.
 * @param[out] temp_p Measured temperature in Q4.4 to Q8.4 depending
 *                    on resolution.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_get_temperature(struct ds18b20_driver_t *self_p,
                            const uint8_t *id_p,
                            int *temp_p);

/**
 * Get temperature for given device identity formatted as a string.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] id_p Device identity.
 * @param[out] temp_p Measured formatted temperature.
 *
 * @return ``temp_p`` on success, NULL otherwise.
 */
char *ds18b20_get_temperature_str(struct ds18b20_driver_t *self_p,
                                  const uint8_t *id_p,
                                  char *temp_p);

#endif
