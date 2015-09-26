/**
 * @file linux/gnu/thrd_port.i
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

#define THRD_IDLE_STACK_MAX 1024

static struct thrd_t main_thrd;

static void thrd_port_swap(struct thrd_t *in,
                           struct thrd_t *out)
{
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
    port->entry = NULL;
    port->arg = NULL;
}

static int thrd_port_spawn(struct thrd_t *thrd,
                           void *(*entry)(void *),
                           void *arg,
                           void *stack,
                           size_t stack_size)
{
    return (0);
}

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
}

static void thrd_port_suspend_timer_callback(void *arg)
{
}

static void thrd_port_tick(void)
{
}
