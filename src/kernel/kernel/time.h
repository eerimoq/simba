/**
 * @file kernel/time.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#ifndef __KERNEL_TIME_H__
#define __KERNEL_TIME_H__

#include "simba.h"

struct time_t {
    uint32_t seconds;
    uint32_t nanoseconds;
};

struct date_t {
    int second;
    int minute;
    int hour;
    int day;
    int date;
    int month;
    int year;
};

/**
 * Get current time.
 * @param[out] now_p Current time.
 * @return zero(0) or negative error code.
 */
int time_get(struct time_t *now_p);

/**
 * Set current time.
 * @param[in] new_p New current time.
 * @return zero(0) or negative error code.
 */
int time_set(struct time_t *new_p);

/**
 * Sleep for given number of microseconds.
 * @param[in] usec Microseconds to sleep.
 */
void time_sleep(int us);

#endif
