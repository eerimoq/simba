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

/**
 * `time_unix_time_to_date()` is based on the musl project code.
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

/* 2000-03-01 (mod 400 year, immediately after feb29 */
#define LEAPOCH (946684800LL + 86400 * (31 + 29))

#define DAYS_PER_400Y (365L * 400L + 97L)
#define DAYS_PER_100Y (365L * 100L + 24L)
#define DAYS_PER_4Y   (365L * 4L   + 1L)

int time_unix_time_to_date(struct date_t *date_p,
                           struct time_t *time_p)
{
    ASSERTN(date_p != NULL, EINVAL);
    ASSERTN(time_p != NULL, EINVAL);

    long long t = time_p->seconds;
    long long weeks, days, secs;
    int32_t remdays, remsecs, remyears;
    int32_t qc_cycles, c_cycles, q_cycles;
    int32_t years, months;
    int32_t wday, yday, leap;
    static const char days_in_month[] = {
        31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 29
    };

    /* Reject time_t values whose year would overflow int32_t */
    if ((t < INT32_MIN * 31622400LL) || (t > INT32_MAX * 31622400LL)) {
        return (-1);
    }

    secs = (t - LEAPOCH);
    days = (secs / 86400);
    remsecs = (secs - (86400 * days));

    if (remsecs < 0) {
        remsecs += 86400;
        days--;
    }

    weeks = ((3 + days) / 7);
    wday = ((3 + days) - (7 * weeks));

    if (wday < 0) {
        wday += 7;
    }

    qc_cycles = (days / DAYS_PER_400Y);
    remdays = (days - (qc_cycles * DAYS_PER_400Y));

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

    if ((years + 100 > INT32_MAX) || (years + 100 < INT32_MIN)) {
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

    date_p->hour = (remsecs / 3600);
    date_p->minute = (remsecs / 60 % 60);
    date_p->second = (remsecs % 60);

    return (0);
}
