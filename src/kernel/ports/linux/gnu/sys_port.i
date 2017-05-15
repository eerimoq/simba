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
        sys_tick_isr();
    }

    return (NULL);
}

__attribute__ ((noreturn))
static void sys_port_stop(int error)
{
    exit(error);
}

static void sys_port_panic_putc(char c)
{
    fputc(c, stderr);
}

__attribute__ ((noreturn))
static void sys_port_reboot()
{
    exit(1);
}

static int sys_port_backtrace(void **buf_pp, size_t size)
{
    return (0);
}

static int sys_port_get_time_into_tick()
{
    return (0);
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

static cpu_usage_t sys_port_interrupt_cpu_usage_get(void)
{
    return (0);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}
