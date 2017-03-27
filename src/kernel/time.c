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

#include "time_port.i"

#define SECONDS_PER_MSB (INT_MAX / CONFIG_SYSTEM_TICK_FREQUENCY)
#define TICKS_PER_MSB   (SECONDS_PER_MSB * CONFIG_SYSTEM_TICK_FREQUENCY)

/* 64 bits so it does not wrap around during the system's uptime. */
struct tick_t {
    uint32_t msb;
    uint32_t lsb;
};

struct module_t {
    struct tick_t tick;
};

static struct module_t module = {
    .tick = {
        .msb = 0,
        .lsb = 0
    }
};

static void tick_to_time(struct time_t *time_p,
                         struct tick_t *tick_p)
{
    uint32_t lsb_seconds;
    uint32_t lsb_ticks;

    lsb_seconds = (tick_p->lsb / CONFIG_SYSTEM_TICK_FREQUENCY);
    lsb_ticks = (tick_p->lsb - (CONFIG_SYSTEM_TICK_FREQUENCY * lsb_seconds));

    time_p->seconds = (tick_p->msb * SECONDS_PER_MSB + lsb_seconds);
    time_p->nanoseconds = (1000 * ((1000000UL * lsb_ticks)
                                   / CONFIG_SYSTEM_TICK_FREQUENCY));
}

static void time_to_tick(struct tick_t *tick_p,
                         struct time_t *time_p)
{
    uint32_t lsb_seconds;
    uint32_t lsb_ticks;

    tick_p->msb = (time_p->seconds / SECONDS_PER_MSB);
    lsb_seconds = (time_p->seconds - (tick_p->msb * SECONDS_PER_MSB));
    lsb_ticks = (((time_p->nanoseconds / 1000)
                  * CONFIG_SYSTEM_TICK_FREQUENCY) / 1000000);
    tick_p->lsb = ((lsb_seconds * CONFIG_SYSTEM_TICK_FREQUENCY) + lsb_ticks);
}

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

/**
 * Update the current time every system tick.
 */
void RAM_CODE time_tick_isr(void)
{
    module.tick.lsb++;

    if (module.tick.lsb == TICKS_PER_MSB) {
        module.tick.msb++;
        module.tick.lsb = 0;
    }
}

int time_get(struct time_t *now_p)
{
    ASSERTN(now_p != NULL, EINVAL);

    struct tick_t tick;
    struct time_t offset;

    offset.seconds = 0;

    sys_lock();
    tick = module.tick;
    offset.nanoseconds = time_port_get_time_into_tick();
    sys_unlock();

    tick_to_time(now_p, &tick);
    time_add(now_p, now_p, &offset);

    return (0);
}

int time_set(struct time_t *new_p)
{
    ASSERTN(new_p != NULL, EINVAL);

    struct tick_t tick;

    time_to_tick(&tick, new_p);

    sys_lock();
    module.tick = tick;
    sys_unlock();

    return (0);
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
