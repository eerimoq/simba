/**
 * @file linux/gnu/thrd_port.i
 * @version 3.0.0
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

#include <pthread.h>

static pthread_mutex_t mutex;

struct sys_port_t {
    pthread_t thrd;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static struct sys_port_t sys_port;

static void *sys_port_ticker(void *arg)
{
    struct timespec abstimeout;
    struct timespec now;

    pthread_mutex_init(&sys_port.mutex, NULL);
    pthread_cond_init (&sys_port.cond, NULL);

    pthread_mutex_lock(&sys_port.mutex);
    while (1) {
        clock_gettime(CLOCK_REALTIME, &now);
        abstimeout.tv_sec = now.tv_sec;
        if (now.tv_nsec > 990000000L) {
            abstimeout.tv_sec++;
        }
        abstimeout.tv_nsec = ((now.tv_nsec + 10000000L) % 1000000000L);
        pthread_cond_timedwait(&sys_port.cond, &sys_port.mutex, &abstimeout);
        sys_tick();
    }

    return (NULL);
}

static void sys_port_lock(void)
{
    pthread_mutex_lock(&mutex);
}

static void sys_port_unlock(void)
{
    pthread_mutex_unlock(&mutex);
}

static void sys_port_lock_isr(void)
{
    pthread_mutex_lock(&mutex);
}

static void sys_port_unlock_isr(void)
{
    pthread_mutex_unlock(&mutex);
}

int sys_port_module_init(void)
{
    pthread_mutex_init(&mutex, NULL);
    
    /* Start sys tick thrd.*/
    if (pthread_create(&sys_port.thrd, NULL, sys_port_ticker, NULL)) {
        fprintf(stderr, "Error creating ticker thrd\n");
        exit(4);
    }

    return (0);
}

void sys_stop(int error)
{
    return (exit(error));
}

static float sys_port_interrupt_cpu_usage_get(void)
{
    return (0.0f);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}
