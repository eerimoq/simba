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

static struct sem_t sem;
static struct mutex_t mutex;
static int sem_counter = 0;
static int mutex_counter = 0;
static THRD_STACK(worker_0_stack, 1024);
static THRD_STACK(worker_1_stack, 1024);
static THRD_STACK(worker_2_stack, 1024);

struct worker_t {
    int sem_counter;
    int mutex_counter;
    char *name_p;
};

static struct worker_t workers[3] = {
    {
        .sem_counter = 0,
        .mutex_counter = 0,
        .name_p = "worker_0"
    },
    {
        .sem_counter = 0,
        .mutex_counter = 0,
        .name_p = "worker_1"
    },
    {
        .sem_counter = 0,
        .mutex_counter = 0,
        .name_p = "worker_2"
    }
};

static void *worker_main(void *arg_p)
{
    int sem_count;
    int mutex_count;
    struct time_t diff, prev, now;
    struct worker_t *worker_p;

    worker_p = arg_p;
    thrd_set_name(worker_p->name_p);
    time_get(&prev);

    while (1) {
        /* Semaphore. */
        sem_take(&sem, NULL);
        sem_count = sem_counter;
        sem_count++;

        if (sem_count % 200 == 0) {
            thrd_sleep_us(10);
        }

        sem_counter = sem_count;
        worker_p->sem_counter++;
        thrd_yield();
        sem_give(&sem, 1);

        /* Mutex. */
        mutex_lock(&mutex);
        mutex_count = mutex_counter;
        mutex_count++;

        if (mutex_count % 200 == 0) {
            thrd_sleep_us(10);
        }

        mutex_counter = mutex_count;
        worker_p->mutex_counter++;
        mutex_unlock(&mutex);

        time_get(&now);
        time_subtract(&diff, &now, &prev);

        if (diff.seconds >= 1) {
            prev = now;
            log_object_print(NULL,
                             LOG_ERROR,
                             OSTR("Sem count: %d, Mutex count: %d\r\n"),
                             sem_count,
                             mutex_count);
        }

        thrd_yield();
    }

    return (NULL);
}

static int test_all(struct harness_t *harness_p)
{
    int i;
    int global_sem_counter;
    int local_sem_counter;
    int global_mutex_counter;
    int local_mutex_counter;

    sem_init(&sem, 0, 1);
    mutex_init(&mutex);

    thrd_spawn(worker_main,
               &workers[0],
               90,
               worker_0_stack,
               sizeof(worker_0_stack));

    thrd_spawn(worker_main,
               &workers[1],
               90,
               worker_1_stack,
               sizeof(worker_1_stack));

    thrd_spawn(worker_main,
               &workers[2],
               90,
               worker_2_stack,
               sizeof(worker_2_stack));

    thrd_sleep_ms(5500);

    /* Copy worker counters to the stack. */
    sem_take(&sem, NULL);
    mutex_lock(&mutex);

    global_sem_counter = sem_counter;
    local_sem_counter = 0;
    global_mutex_counter = mutex_counter;
    local_mutex_counter = 0;

    for (i = 0; i < membersof(workers); i++) {
        local_sem_counter += workers[i].sem_counter;
        local_mutex_counter += workers[i].mutex_counter;
    }

    mutex_unlock(&mutex);
    sem_give(&sem, 1);

    std_printf(FSTR("global_sem_counter: %d\r\n"
                    "local_sem_counter: %d\r\n"
                    "global_mutex_counter: %d\r\n"
                    "local_mutex_counter: %d\r\n"),
               global_sem_counter,
               local_sem_counter,
               global_mutex_counter,
               local_mutex_counter);

    BTASSERTI(global_sem_counter, >, 10000);
    BTASSERTI(global_mutex_counter, >, 10000);
    BTASSERTI(global_sem_counter, ==, local_sem_counter);
    BTASSERTI(global_mutex_counter, ==, local_mutex_counter);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_all, "test_all" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
