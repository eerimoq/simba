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
    /** Second. 0-59 */
    int second;
    /** Minute. 0-59 */
    int minute;
    /** Hour. 0-23 */
    int hour;
    /** Weekday. 1-7 */
    int day;
    /** Day in month. 1-31 */
    int date;
    /** Month. 1-12 */
    int month;
    /** Year. 1970- */
    int year;
};

/**
 * Get current time in seconds and nanoseconds. The resolution of the
 * time is implementation specific.
 *
 * @param[out] now_p Current time.
 *
 * @return zero(0) or negative error code.
 */
int time_get(struct time_t *now_p);

/**
 * Set current time in seconds and nanoseconds.
 *
 * @param[in] new_p New current time.
 *
 * @return zero(0) or negative error code.
 */
int time_set(struct time_t *new_p);

/**
 * Sleep (busy wait) for given number of microseconds.
 *
 * @param[in] usec Microseconds to sleep.
 *
 * @return void
 */
void time_sleep(int usec);

/**
 * Convert given time to a date.
 *
 * @param[out] date_p Converted time.
 * @param[in] time_p Time to convert.
 *
 * @return zero(0) or negative error code.
 */
int time_time_to_date(struct date_t *date_p,
                      struct time_t *time_p);

#endif
