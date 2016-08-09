/**
 * @file ds3231.c
 * @version 6.0.0
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

#include "simba.h"

#define DS3231_I2C_ADDRESS     0x68

static inline uint8_t encode(int value)
{
    int low;

    low = (value % 10);

    return ((((value - low) / 10) << 4) | low);
}

static inline int decode(uint8_t value)
{
    return (10 * (value >> 4) + (value & 0x0f));
}

int ds3231_init(struct ds3231_driver_t *self_p,
                struct i2c_driver_t *i2c_p)
{
    self_p->i2c_p = i2c_p;

    return (0);
}

int ds3231_set_date(struct ds3231_driver_t *self_p,
                    struct date_t *date_p)
{
    uint8_t date[8];

    date[0] = 0x00;
    date[1] = encode(date_p->second);
    date[2] = encode(date_p->minute);
    date[3] = (((date_p->hour > 19) << 5)
               + encode(date_p->hour % 20));
    date[4] = date_p->day;
    date[5] = encode(date_p->date);
    date[6] = encode(date_p->month);
    date[7] = encode(date_p->year % 100);

    if (i2c_write(self_p->i2c_p,
                  DS3231_I2C_ADDRESS,
                  date,
                  sizeof(date)) != sizeof(date)) {
        return (-1);
    }

    return (0);
}

int ds3231_get_date(struct ds3231_driver_t *self_p,
                    struct date_t *date_p)
{
    uint8_t date[7];
    uint8_t address;

    address = 0x0;

    if (i2c_write(self_p->i2c_p,
                  DS3231_I2C_ADDRESS,
                  &address,
                  sizeof(address)) != sizeof(address)) {
        return (-1);
    }

    if (i2c_read(self_p->i2c_p,
                 DS3231_I2C_ADDRESS,
                 date,
                 sizeof(date)) != sizeof(date)) {
        return (-1);
    }

    date_p->second = decode(date[0]);
    date_p->minute = decode(date[1]);
    date_p->hour = (20 * ((date[2] >> 5) & 0x01)
                    + decode(date[2] & 0x1f));
    date_p->day = date[3];
    date_p->date = decode(date[4]);
    date_p->month = decode(date[5] & 0x7f);
    date_p->year = decode(date[6]);

    return (0);
}
