/**
 * @file linux/gnu/thrd_port.i
 * @version 0.5.0
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

#define THRD_IDLE_STACK_MAX 1024
#define THRD_MONITOR_STACK_MAX 1024

struct thrd_port_idle_t {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static struct thrd_t main_thrd;

static struct thrd_port_idle_t idle = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER
};

static void *thrd_port_main(void *arg)
{
    struct thrd_port_t *port;

    port = arg;
    pthread_cond_wait(&port->cond, &port->mutex);
    pthread_mutex_unlock(&port->mutex);
    sys_unlock();
    port->main(port->arg);

    /* Thread termination. */
    terminate();

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
    port->main = NULL;
    port->arg = NULL;
    pthread_mutex_init(&port->mutex, NULL);
    pthread_cond_init (&port->cond, NULL);
}

static int thrd_port_spawn(struct thrd_t *thrd_p,
                           void *(*main)(void *),
                           void *arg,
                           void *stack,
                           size_t stack_size)
{
    struct thrd_port_t *port;

    /* Initialize thrd port.*/
    port = &thrd_p->port;
    port->main = main;
    port->arg = arg;
    pthread_mutex_init(&port->mutex, NULL);
    pthread_cond_init (&port->cond, NULL);
    pthread_mutex_lock(&port->mutex);

    if (pthread_create(&port->thrd, NULL, thrd_port_main, port)) {
        fprintf(stderr, "Error creating thrd\n");
        return (1);
    }

    return (0);
}

static void thrd_port_idle_wait(struct thrd_t *thrd_p)
{
    pthread_mutex_lock(&idle.mutex);
    pthread_cond_wait(&idle.cond, &idle.mutex);
    pthread_mutex_unlock(&idle.mutex);

    /* Add this thread to the ready list and reschedule. */
    sys_lock();
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);
    thrd_reschedule();
    sys_unlock();
}

static void thrd_port_suspend_timer_callback(void *arg)
{
    struct thrd_t *thrd_p = arg;

    thrd_p->err = -ETIMEDOUT;
    thrd_p->state = THRD_STATE_READY;
    scheduler_ready_push(thrd_p);

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

static void thrd_port_cpu_usage_start(struct thrd_t *thrd_p)
{
}

static void thrd_port_cpu_usage_stop(struct thrd_t *thrd_p)
{
}

#if CONFIG_MONITOR_THREAD == 1

static float thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    return (0.0);
}

static void thrd_port_cpu_usage_reset(struct thrd_t *thrd_p)
{
}

#endif
