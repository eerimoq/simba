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
 * with all DS18B20 sensors on given OWI bus.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] owi_p One-Wire (OWI) driver.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_init(struct ds18b20_driver_t *self_p,
                 struct owi_driver_t *owi_p);

/**
 * Start a temperature convertion on all sensors. The converted
 * temperature can later be read with ``ds18b20_read*()``.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_convert(struct ds18b20_driver_t *self_p);

/**
 * Read the most recently converted temperature from given sensor.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] id_p Sensor identity.
 * @param[out] temperature_p Measured temperature.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_read(struct ds18b20_driver_t *self_p,
                 const uint8_t *id_p,
                 float *temperature_p);

/**
 * Read the most recently converted temperature from given sensor as a
 * fixed point number.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] id_p Sensor identity.
 * @param[out] temperature_p Measured temperature in Q4 fixed point
 *                           format, or unit 0.0625 degrees Celsius
 *                           (the raw value read from the sensor).
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_read_fixed_point(struct ds18b20_driver_t *self_p,
                             const uint8_t *id_p,
                             int *temperature_p);

/**
 * Read the most recently converted temperature from given sensor as a
 * string.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] id_p Sensor identity.
 * @param[out] temperature_p Measured temperature as a string.
 *
 * @return ``temperature_p`` on success, NULL otherwise.
 */
char *ds18b20_read_string(struct ds18b20_driver_t *self_p,
                          const uint8_t *id_p,
                          char *temperature_p);

/**
 * Read the most recently converted temperature from given
 * sensor. Call ``ds18b20_convert()`` to read the temperature from the
 * sensor and update the cached value.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] id_p Sensor identity.
 * @param[out] temperature_p Measured temperature in 0.0625 degrees
 *                           Celsius (the raw value read from the
 *                           sensor).
 *
 * @return zero(0) or negative error code.
 */
__attribute__ ((deprecated))
int ds18b20_get_temperature(struct ds18b20_driver_t *self_p,
                            const uint8_t *id_p,
                            int *temperature_p);

/**
 * Get temperature for given sensor identity formatted as a string.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] id_p Sensor identity.
 * @param[out] temperature_p Measured formatted temperature.
 *
 * @return ``temperature_p`` on success, NULL otherwise.
 */
__attribute__ ((deprecated))
char *ds18b20_get_temperature_str(struct ds18b20_driver_t *self_p,
                                  const uint8_t *id_p,
                                  char *temperature_p);

#endif
