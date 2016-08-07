/**
 * @file drivers/ds18b20.h
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_DS18B20_H__
#define __DRIVERS_DS18B20_H__

#include "simba.h"

struct ds18b20_driver_t {
    struct owi_driver_t *owi_p;
    struct ds18b20_driver_t *next_p;
};

/**
 * Initialize the DS18B20 driver module.
 *
 * @return zero(0) or negative error code.
 */
int ds18b20_module_init(void);

/**
 * Initialize given driver object.
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
                            uint8_t *id_p,
                            int *temp_p);

/**
 * Get temperature for given device identity returned formatted as a
 * string.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] id_p Device identity.
 * @param[in] buf_p Buffer.
 *
 * @return Buffer or NULL.
 */
char *ds18b20_get_temperature_str(struct ds18b20_driver_t *self_p,
                                  uint8_t *id_p,
                                  char *buf_p);

#endif
