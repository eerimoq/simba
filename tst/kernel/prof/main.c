/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015, Erik Moqvist
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

int test_memory_usage(struct harness_t *harness_p)
{
    /* chan */
    std_printf(FSTR("sizeof(struct chan_list_t) = %d\n"), (int)sizeof(struct chan_list_t));
    std_printf(FSTR("sizeof(struct chan_t) = %d\n"), (int)sizeof(struct chan_t));

    /* fs */
    std_printf(FSTR("sizeof(struct fs_counter_t) = %d\n"), (int)sizeof(struct fs_counter_t));
    std_printf(FSTR("sizeof(struct fs_node_t) = %d\n"), (int)sizeof(struct fs_node_t));

    /* list */
    std_printf(FSTR("sizeof(struct list_singly_linked_t) = %d\n"), (int)sizeof(struct list_singly_linked_t));

    /* queue */
    std_printf(FSTR("sizeof(struct queue_t) = %d\n"), (int)sizeof(struct queue_t));

    /* sem */
    std_printf(FSTR("sizeof(struct sem_t) = %d\n"), (int)sizeof(struct sem_t));

    /* shell */
    std_printf(FSTR("sizeof(struct shell_args_t) = %d\n"), (int)sizeof(struct shell_args_t));

    /* sys */
    std_printf(FSTR("sizeof(struct sys_t) = %d\n"), (int)sizeof(struct sys_t));

    /* time */
    std_printf(FSTR("sizeof(struct time_t) = %d\n"), (int)sizeof(struct time_t));

    /* timer */
    std_printf(FSTR("sizeof(struct timer_t) = %d\n"), (int)sizeof(struct timer_t));

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_memory_usage, "test_memory_usage" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
