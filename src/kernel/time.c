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

#include "simba.h"

#include "time_port.i"

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
