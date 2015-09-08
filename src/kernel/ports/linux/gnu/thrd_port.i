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

struct thrd_port_idle_t {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static struct thrd_t main_thrd;

static struct thrd_port_idle_t idle = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER
};

static void *thrd_port_entry(void *arg)
{
    struct thrd_port_t *port;

    port = arg;
    pthread_cond_wait(&port->cond, &port->mutex);
    pthread_mutex_unlock(&port->mutex);
    sys_unlock(&irq);
    port->entry(port->arg);

    return (NULL);
}

static void thrd_port_swap(struct thrd_t *in,
                           struct thrd_t *out)
{
    /* Signal 'out' thrd and enter wait.*/
    pthread_mutex_lock(&out->port.mutex);
    pthread_mutex_lock(&in->port.mutex);
    pthread_cond_signal(&in->port.cond);
    pthread_mutex_unlock(&in->port.mutex);
    pthread_cond_wait(&out->port.cond, &out->port.mutex);
    pthread_mutex_unlock(&out->port.mutex);
}

static void thrd_port_init_main(struct thrd_port_t *port)
{
    port->entry = NULL;
    port->arg = NULL;
    pthread_mutex_init(&port->mutex, NULL);
    pthread_cond_init (&port->cond, NULL);
}

static int thrd_port_spawn(struct thrd_t *thrd,
                           void *(*entry)(void *),
                           void *arg,
                           void *stack,
                           size_t stack_size)
{
    struct thrd_port_t *port;

    /* Initialize thrd port.*/
    port = &thrd->port;
    port->entry = entry;
    port->arg = arg;
    pthread_mutex_init(&port->mutex, NULL);
    pthread_cond_init (&port->cond, NULL);
    pthread_mutex_lock(&port->mutex);

    if (pthread_create(&port->thrd, NULL, thrd_port_entry, port)) {
        fprintf(stderr, "Error creating thrd\n");
        return (1);
    }

    return (0);
}

static void thrd_port_kill(struct thrd_t *thrd)
{
    
}

static void thrd_port_idle_wait(void)
{
    pthread_mutex_lock(&idle.mutex);
    pthread_cond_wait(&idle.cond, &idle.mutex);
    pthread_mutex_unlock(&idle.mutex);
}

static void thrd_port_suspend_timer_callback(void *arg)
{
    struct thrd_t *thrd = arg;

    thrd->state = THRD_STATE_READY;
    sys_lock(&irq);
    scheduler_ready_push(thrd);
    sys_unlock(&irq);

    /* Signal idle thrd.*/
    pthread_mutex_lock(&idle.mutex);
    pthread_cond_signal(&idle.cond);
    pthread_mutex_unlock(&idle.mutex);    
}

static void thrd_port_tick(void)
{
    /* Signal idle thrd.*/
    pthread_mutex_lock(&idle.mutex);
    pthread_cond_signal(&idle.cond);
    pthread_mutex_unlock(&idle.mutex);        
}
