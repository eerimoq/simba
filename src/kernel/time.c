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

#include "time_port.i"

struct module_t {
    struct time_t uptime_offset;
};

static struct module_t module;

static void adjust_result(struct time_t *res_p)
{
    /* abs(nanosecons) must be less than 1000000000. */
    if (res_p->nanoseconds < -999999999L) {
        res_p->seconds--;
        res_p->nanoseconds += 1000000000L;
    } else if (res_p->nanoseconds > 999999999L) {
        res_p->seconds++;
        res_p->nanoseconds -= 1000000000L;
    }

    /* `seconds` and `nanoseconds` must have the same sign. */
    if ((res_p->seconds > 0) && (res_p->nanoseconds < 0)) {
        res_p->seconds--;
        res_p->nanoseconds += 1000000000L;
    } else if ((res_p->seconds < 0) && (res_p->nanoseconds > 0)) {
        res_p->seconds++;
        res_p->nanoseconds -= 1000000000L;
    }
}

int time_get(struct time_t *now_p)
{
    ASSERTN(now_p != NULL, EINVAL);

    if (sys_uptime(now_p) != 0) {
        return (-1);
    }

    return (time_add(now_p, now_p, &module.uptime_offset));
}

int time_set(struct time_t *new_p)
{
    ASSERTN(new_p != NULL, EINVAL);

    struct time_t uptime;

    if (sys_uptime(&uptime) != 0) {
        return (-1);
    }

    return (time_subtract(&module.uptime_offset, new_p, &uptime));
}

int time_add(struct time_t *res_p,
             struct time_t *left_p,
             struct time_t *right_p)
{
    ASSERTN(res_p != NULL, EINVAL);
    ASSERTN(left_p != NULL, EINVAL);
    ASSERTN(right_p != NULL, EINVAL);

    res_p->seconds = (left_p->seconds + right_p->seconds);
    res_p->nanoseconds = (left_p->nanoseconds + right_p->nanoseconds);

    adjust_result(res_p);

    return (0);
}

int time_subtract(struct time_t *res_p,
                  struct time_t *left_p,
                  struct time_t *right_p)
{
    ASSERTN(res_p != NULL, EINVAL);
    ASSERTN(left_p != NULL, EINVAL);
    ASSERTN(right_p != NULL, EINVAL);

    res_p->seconds = (left_p->seconds - right_p->seconds);
    res_p->nanoseconds = (left_p->nanoseconds - right_p->nanoseconds);

    adjust_result(res_p);

    return (0);
}

void time_busy_wait_us(long microseconds)
{
    time_port_busy_wait_us(microseconds);
}

#if CONFIG_TIME_UNIX_TIME_TO_DATE == 1
#    include "time/unix_time_to_date.i"
#endif
