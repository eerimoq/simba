/**
 * @file main.c
 * @version 3.1.0
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

#include "simba.h"

static THRD_STACK(writer_0_stack, 512);
static THRD_STACK(writer_1_stack, 512);
static THRD_STACK(writer_2_stack, 512);
static THRD_STACK(reader_0_stack, 512);
static THRD_STACK(reader_1_stack, 512);

static volatile int count;
struct rwlock_t count_lock;

static void *writer_main(void *arg_p)
{
    int i;

    for (i = 0; i < 10; i++) {
        thrd_sleep_us(15000);
        rwlock_writer_take(&count_lock);
        count++;
        thrd_sleep_us(25);
        rwlock_writer_give(&count_lock);
    }

    return (NULL);
}

static void *reader_main(void *arg_p)
{
    int read_count;

    do {
        thrd_sleep_us(10);
        rwlock_reader_take(&count_lock);
        read_count = count;
        thrd_sleep_us(10);
        rwlock_reader_give(&count_lock);
    } while (read_count < 30);

    return (NULL);
}

static int test_one_thread(struct harness_t *harness_p)
{
    struct rwlock_t foo;

    BTASSERT(rwlock_init(&foo) == 0);

    BTASSERT(rwlock_reader_take(&foo) == 0);
    BTASSERT(rwlock_reader_give(&foo) == 0);

    BTASSERT(rwlock_writer_take(&foo) == 0);
    BTASSERT(rwlock_writer_give(&foo) == 0);

    return (0);
}

static int test_multi_thread(struct harness_t *harness_p)
{
    struct thrd_t *reader_0_p;
    struct thrd_t *reader_1_p;

    count = 0;

    BTASSERT(rwlock_init(&count_lock) == 0);

    BTASSERT((reader_0_p = thrd_spawn(reader_main,
                                      NULL,
                                      0,
                                      reader_0_stack,
                                      sizeof(reader_0_stack))) != NULL);

    BTASSERT((reader_1_p = thrd_spawn(reader_main,
                                      NULL,
                                      0,
                                      reader_1_stack,
                                      sizeof(reader_1_stack))) != NULL);

    BTASSERT(thrd_spawn(writer_main,
                        NULL,
                        0,
                        writer_0_stack,
                        sizeof(writer_0_stack)) != NULL);

    BTASSERT(thrd_spawn(writer_main,
                        NULL,
                        0,
                        writer_1_stack,
                        sizeof(writer_1_stack)) != NULL);

    BTASSERT(thrd_spawn(writer_main,
                        NULL,
                        0,
                        writer_2_stack,
                        sizeof(writer_2_stack)) != NULL);

    /* Wait for both readers to terminate. */
    thrd_join(reader_0_p);
    thrd_join(reader_1_p);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_one_thread, "test_one_thread" },
        { test_multi_thread, "test_multi_thread" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
