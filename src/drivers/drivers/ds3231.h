/**
 * @file drivers/ds3231.h
 * @version 0.1.0
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

#ifndef __DRIVERS_DS3231_H__
#define __DRIVERS_DS3231_H__

#include "simba.h"

struct ds3231_driver_t {
    //struct i2c_driver_t *owi2c_p;
};

/**
 * Initialize driver object.
 *
 * @param[in,out] self_p Driver object to be initialized.
 *
 * @return zero(0) or negative error code.
 */
int ds3231_init(struct ds3231_driver_t *self_p);

int ds3231_set_date(struct ds3231_driver_t *self_p, struct date_t *date_p);

int ds3231_get_date(struct ds3231_driver_t *self_p, struct date_t *date_p);

#endif
