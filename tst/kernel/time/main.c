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

#include "simba.h"
#include <limits.h>

struct test_date_t {
    struct time_t time;
    struct date_t date;
};

static int test_get_set(struct harness_t *harness)
{
    struct time_t time1;
    struct time_t time2;

    /* Start by getting the current time. */
    BTASSERT(time_get(&time1) == 0);

    std_printf(FSTR("time1: seconds = %lu, nanoseconds = %lu\r\n"),
               time1.seconds, time1.nanoseconds);

    /* Wait a while and verify that the time has changed. */
    thrd_sleep_us(100000);
    BTASSERT(time_get(&time2) == 0);

    std_printf(FSTR("time2: seconds = %lu, nanoseconds = %lu\r\n"),
               time2.seconds, time2.nanoseconds);

    BTASSERT((time2.seconds > time1.seconds)
             || (time2.nanoseconds > time1.nanoseconds));

    /* Set the time just before a MSB increment, to verify that
       increment. */
    time1.seconds = (INT_MAX / CONFIG_SYSTEM_TICK_FREQUENCY - 1);
    time1.nanoseconds = 999000000;
    BTASSERT(time_set(&time1) == 0);

    /* Get the new time. */
    BTASSERT(time_get(&time1) == 0);
    BTASSERT(time1.seconds >= (INT_MAX / CONFIG_SYSTEM_TICK_FREQUENCY - 1));

    std_printf(FSTR("time1: seconds = %lu, nanoseconds = %lu\r\n"),
               time1.seconds, time1.nanoseconds);

    /* Wait a while and verify that the time has changed. */
    thrd_sleep_us(100000);
    BTASSERT(time_get(&time2) == 0);

    std_printf(FSTR("time2: seconds = %lu, nanoseconds = %lu\r\n"),
               time2.seconds, time2.nanoseconds);

    BTASSERT((time2.seconds > time1.seconds)
             || (time2.nanoseconds > time1.nanoseconds));

    return (0);
}

static int test_date(struct harness_t *harness)
{
    int i;
    struct date_t date;
    struct test_date_t times[] = {
        /* Unix time start date: Thursday, 1 January 1970 */
        {
            .time = { .seconds = 0, .nanoseconds = 0 },
            .date = { .second = 0, .minute = 0, .hour = 0,
                      .day = 4, .date = 1, .month = 1, .year = 1970 }
        },

        /* Max hour, minute and second. */
        {
            .time = { .seconds = 1451347199, .nanoseconds = 0 },
            .date = { .second = 59, .minute = 59, .hour = 23,
                      .day = 1, .date = 28, .month = 12, .year = 2015 }
        },

        /* 1999-01-01 00:00:00 */
        {
            .time = { .seconds = 915148800, .nanoseconds = 0 },
            .date = { .second = 0, .minute = 0, .hour = 0,
                      .day = 5, .date = 1, .month = 1, .year = 1999 }
        }
    };

    for (i = 0; i < membersof(times); i++) {
        std_printf(FSTR("date index %d\r\n"), i);

        BTASSERT(time_unix_time_to_date(&date, &times[i].time) == 0);

        /* Verify the converted time. */
        BTASSERT(date.second == times[i].date.second);
        BTASSERT(date.minute == times[i].date.minute);
        BTASSERT(date.hour == times[i].date.hour);
        BTASSERT(date.day == times[i].date.day);
        BTASSERT(date.date == times[i].date.date);
        BTASSERT(date.month == times[i].date.month);
        BTASSERT(date.year == times[i].date.year);
    }

    return (0);
}

static int test_sleep(struct harness_t *harness)
{
    struct time_t start, stop, diff;
    int i;

    long times[] = {
        1,
        10,
        100,
        1000,
        10000,
        100000,
        1000000, /* 1 second. */
        2000000, /* 2 seconds. */
    };

    for (i = 0; i < membersof(times); i++) {
        std_printf(FSTR("busy-wait for %ld microseconds\r\n"), times[i]);

        BTASSERT(time_get(&start) == 0);
        time_busy_wait_us(times[i]);
        BTASSERT(time_get(&stop) == 0);

        time_subtract(&diff, &stop, &start);

        std_printf(FSTR("  start: seconds = %ld, microseconds = %ld\r\n"
                        "  stop: seconds = %ld, microseconds = %ld\r\n"
                        "  diff: seconds = %ld, microseconds = %ld\r\n"),
                   start.seconds,
                   start.nanoseconds / 1000,
                   stop.seconds,
                   stop.nanoseconds / 1000,
                   diff.seconds,
                   diff.nanoseconds / 1000);
    }

    return (0);
}

static int test_add(struct harness_t *harness)
{
    struct time_t *left_p, *right_p, res;
    float fleft, fright, fres;
    int i;

    /* left, right, res. */
    struct time_t times[][3] = {
        { {  0,          0 }, {  0,          0 }, {   0,           0 } },
        { {  1,          0 }, {  0,  100000000 }, {   1,   100000000 } },
        { { -1,          0 }, {  0, -100000000 }, {  -1,  -100000000 } },
        { {  1,          0 }, {  2,          0 }, {   3,           0 } },
        { {  0,          0 }, {  0,  100000000 }, {   0,   100000000 } },
        { {  1,          0 }, {  0, -100000000 }, {   0,   900000000 } },
        { { 10,  100000000 }, {  2,  700000000 }, {  12,   800000000 } },
        { { 10,  100000000 }, { 20,  700000000 }, {  30,   800000000 } },
        { {  0, -600000000 }, {  0, -700000000 }, {  -1,  -300000000 } },
        { {  0,  600000000 }, {  0,  700000000 }, {   1,   300000000 } },
    };

    for (i = 0; i < membersof(times); i++) {
        left_p = &times[i][0];
        right_p = &times[i][1];

        time_add(&res, left_p, right_p);

        fleft = (left_p->seconds + left_p->nanoseconds / 1000000000.0);
        fright = (right_p->seconds + right_p->nanoseconds / 1000000000.0);
        fres = (fleft - fright);

        std_printf(FSTR("%ld;%ld - %ld;%ld = %ld;%ld (%f - %f = %f)\r\n"),
                   (long)left_p->seconds,
                   (long)left_p->nanoseconds / 100000000,
                   (long)right_p->seconds,
                   (long)right_p->nanoseconds / 100000000,
                   (long)res.seconds,
                   (long)res.nanoseconds / 100000000,
                   fleft,
                   fright,
                   fres);

        BTASSERT(res.seconds == times[i][2].seconds);
        BTASSERT(res.nanoseconds == times[i][2].nanoseconds);
    }

    return (0);
}

static int test_subtract(struct harness_t *harness)
{
    struct time_t *left_p, *right_p, res;
    float fleft, fright, fres;
    int i;

    /* left, right, res. */
    struct time_t times[][3] = {
        { {  0,          0 }, {  0,          0 }, {   0,           0 } },
        { {  1,          0 }, {  0,  100000000 }, {   0,   900000000 } },
        { { -1,          0 }, {  0, -100000000 }, {   0,  -900000000 } },
        { {  1,          0 }, {  2,          0 }, {  -1,           0 } },
        { {  0,          0 }, {  0,  100000000 }, {   0,  -100000000 } },
        { {  1,          0 }, {  0, -100000000 }, {   1,   100000000 } },
        { { 10,  100000000 }, {  2,  700000000 }, {   7,   400000000 } },
        { { 10,  100000000 }, { 20,  700000000 }, { -10,  -600000000 } },
        { {  0, -600000000 }, {  0,  700000000 }, {  -1,  -300000000 } },
        { {  0,  600000000 }, {  0, -700000000 }, {   1,   300000000 } },
    };

    for (i = 0; i < membersof(times); i++) {
        left_p = &times[i][0];
        right_p = &times[i][1];

        time_subtract(&res, left_p, right_p);

        fleft = (left_p->seconds + left_p->nanoseconds / 1000000000.0);
        fright = (right_p->seconds + right_p->nanoseconds / 1000000000.0);
        fres = (fleft - fright);

        std_printf(FSTR("%ld;%ld - %ld;%ld = %ld;%ld (%f - %f = %f)\r\n"),
                   (long)left_p->seconds,
                   (long)left_p->nanoseconds / 100000000,
                   (long)right_p->seconds,
                   (long)right_p->nanoseconds / 100000000,
                   (long)res.seconds,
                   (long)res.nanoseconds / 100000000,
                   fleft,
                   fright,
                   fres);

        BTASSERT(res.seconds == times[i][2].seconds);
        BTASSERT(res.nanoseconds == times[i][2].nanoseconds);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_get_set, "test_get_set" },
        { test_date, "test_date" },
        { test_sleep, "test_sleep" },
        { test_add, "test_add" },
        { test_subtract, "test_subtract" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
