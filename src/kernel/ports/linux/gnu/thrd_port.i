/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

struct thrd_port_idle_t {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static struct thrd_t main_thrd;
extern char __main_stack_end;

static struct thrd_t *thrd_port_get_main_thrd(void)
{
    return (&main_thrd);
}

static char *thrd_port_get_main_thrd_stack_top(void)
{
    return (NULL);
}

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

static cpu_usage_t thrd_port_cpu_usage_get(struct thrd_t *thrd_p)
{
    return (0);
}

static void thrd_port_cpu_usage_reset(struct thrd_t *thrd_p)
{
}

#endif

static const void *thrd_port_get_bottom_of_stack(struct thrd_t *thrd_p)
{
    char dummy;
    const void *bottom_p = (const void *)&dummy;

    if (thrd_p == thrd_self()) {
        return (bottom_p);
    }

    return (NULL);
}

static const void *thrd_port_get_top_of_stack(struct thrd_t *thrd_p)
{
    return (NULL);
}
