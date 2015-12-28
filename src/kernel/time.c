/**
 * @file time.c
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

/**
 * `time_time_to_date()` is based on the musl project code.
 *
 * musl as a whole is licensed under the following standard MIT license:
 * 
 * Copyright © 2005-2013 Rich Felker
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "simba.h"
#include <limits.h>

#include "time_port.i"

/* 2000-03-01 (mod 400 year, immediately after feb29 */
#define LEAPOCH (946684800LL + 86400*(31+29))

#define DAYS_PER_400Y (365L*400L + 97L)
#define DAYS_PER_100Y (365L*100L + 24L)
#define DAYS_PER_4Y   (365L*4L   + 1L)

struct state_t {
    uint64_t tick; /* Current tick. 64 bits so it does not wrap around
                      during the system's uptime. */
    struct time_t now;
};

static struct state_t state = {
    .tick = 0,
    .now = { .seconds = 0, .nanoseconds = 0 }
};

static inline void tick_to_time(uint64_t tick,
                                struct time_t *time_p)
{
    time_p->seconds = (tick / SYS_TICK_FREQUENCY);
    time_p->nanoseconds =
        ((1000000000ULL * (tick % SYS_TICK_FREQUENCY))
         / SYS_TICK_FREQUENCY);
}

/**
 * Update the current time every system tick.
 */
void time_tick(void)
{
    state.tick += 1;
    tick_to_time(state.tick, &state.now);
}

int time_get(struct time_t *now_p)
{
    sys_lock();
    /* ToDo: Add the time since latest system tick by reading the
       system tick counter register. */
    *now_p = state.now;
    sys_unlock();

    return (0);
}

int time_set(struct time_t *new_p)
{
    sys_lock();
    state.tick = ((new_p->seconds * SYS_TICK_FREQUENCY) +
                  DIV_CEIL((DIV_CEIL(new_p->nanoseconds, 1000)
                            * SYS_TICK_FREQUENCY), 1000000));
    tick_to_time(state.tick, &state.now);
    sys_unlock();

    return (0);
}

void time_sleep(int us)
{
    time_port_sleep(us);
}

int time_time_to_date(struct date_t *date_p,
                      struct time_t *time_p)
{
    long long t = time_p->seconds;
    long long days, secs;
    int remdays, remsecs, remyears;
    int qc_cycles, c_cycles, q_cycles;
    int years, months;
    int wday, yday, leap;
    static const char days_in_month[] = {31,30,31,30,31,31,30,31,30,31,31,29};

    /* Reject time_t values whose year would overflow int */
    if ((t < INT_MIN * 31622400LL) || (t > INT_MAX * 31622400LL)) {
        return (-1);
    }

    secs = (t - LEAPOCH);
    days = (secs / 86400);
    remsecs = (secs % 86400);

    if (remsecs < 0) {
        remsecs += 86400;
        days--;
    }

    wday = ((3 + days) % 7);

    if (wday < 0) {
        wday += 7;
    }

    qc_cycles = (days / DAYS_PER_400Y);
    remdays = (days % DAYS_PER_400Y);

    if (remdays < 0) {
        remdays += DAYS_PER_400Y;
        qc_cycles--;
    }

    c_cycles = (remdays / DAYS_PER_100Y);

    if (c_cycles == 4) {
        c_cycles--;
    }

    remdays -= (c_cycles * DAYS_PER_100Y);

    q_cycles = (remdays / DAYS_PER_4Y);

    if (q_cycles == 25) {
        q_cycles--;
    }

    remdays -= (q_cycles * DAYS_PER_4Y);

    remyears = (remdays / 365);

    if (remyears == 4) {
        remyears--;
    }

    remdays -= (remyears * 365);

    leap = (!remyears && (q_cycles || !c_cycles));
    yday = (remdays + 31 + 28 + leap);

    if (yday >= (365 + leap)) {
        yday -= (365 + leap);
    }

    years = (remyears + 4 * q_cycles + 100 * c_cycles + 400 * qc_cycles);

    for (months = 0; days_in_month[months] <= remdays; months++) {
        remdays -= days_in_month[months];
    }

    if ((years + 100 > INT_MAX) || (years + 100 < INT_MIN)) {
        return (-1);
    }

    date_p->year = (years + 100);
    date_p->year += 1900;
    date_p->month = (months + 2);

    if (date_p->month >= 12) {
        date_p->month -= 12;
        date_p->year++;
    }

    date_p->month++;
    date_p->date = (remdays + 1);
    date_p->day = wday;
    //date_p->yday = yday;

    date_p->hour = (remsecs / 3600);
    date_p->minute = (remsecs / 60 % 60);
    date_p->second = (remsecs % 60);

    return (0);
}
