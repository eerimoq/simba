/**
 * @file main.c
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

#include "simba.h"

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
    thrd_usleep(100000);
    BTASSERT(time_get(&time2) == 0);

    std_printf(FSTR("time2: seconds = %lu, nanoseconds = %lu\r\n"),
               time2.seconds, time2.nanoseconds);

    BTASSERT((time2.seconds > time1.seconds)
             || (time2.nanoseconds > time1.nanoseconds));

    /* Set the time to a high value. */
    time1.seconds = 100;
    time1.nanoseconds = 350000000;
    BTASSERT(time_set(&time1) == 0);

    /* Get the new time. */
    BTASSERT(time_get(&time1) == 0);

    std_printf(FSTR("time1: seconds = %lu, nanoseconds = %lu\r\n"),
               time1.seconds, time1.nanoseconds);

    /* Wait a while and verify that the time has changed. */
    thrd_usleep(100000);
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
    struct time_t start, stop;
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
        std_printf(FSTR("sleep for %ld microseconds\r\n"), times[i]);

        start.seconds = 0;
        start.nanoseconds = 0;
        BTASSERT(time_set(&start) == 0);

        BTASSERT(time_get(&start) == 0);
        
        std_printf(FSTR("  start: seconds = %lu, microseconds = %lu\r\n"),
                   start.seconds, start.nanoseconds / 1000);
        
        time_sleep(times[i]);
        
        BTASSERT(time_get(&stop) == 0);
        
        std_printf(FSTR("  stop: seconds = %lu, microseconds = %lu\r\n"
                        "  diff: seconds = %lu, microseconds = %lu\r\n"),
                   stop.seconds,
                   stop.nanoseconds / 1000,
                   (stop.seconds - start.seconds),
                   (stop.nanoseconds - start.nanoseconds) / 1000);
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
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
