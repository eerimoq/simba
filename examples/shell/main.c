/**
 * @file main.c
 * @version 0.1.0
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

FS_COMMAND_DEFINE("/tmp/foo", tmp_foo);
FS_COMMAND_DEFINE("/worker/set", worker_set);

COUNTER_DEFINE("/foo", foo);
COUNTER_DEFINE("/bar", bar);
COUNTER_DEFINE("/fie", fie);

static long worker_cpu_usage = 10;

int worker_set(int argc,
               const char *argv[],
               void *out_p,
               void *in_p)
{
    if (argc != 2) {
        std_fprintf(out_p, FSTR("Usage: %s <worker cpu usage>\r\n"), argv[0]);

        return (1);
    }

    std_strtol(argv[1], &worker_cpu_usage);

    return (0);
}

int tmp_foo(int argc,
            const char *argv[],
            void *out_p,
            void *in_p)
{
    UNUSED(in_p);

    if (argc != 4) {
        std_fprintf(out_p, FSTR("3 arguments required.\r\n"));
        return (1);
    }

    std_fprintf(out_p,
                FSTR("argc = %d, argv[0] = %s, argv[1] = %s, argv[2] = %s\r\n"),
                argc,
                argv[0],
                argv[1],
                argv[2]);

    return (0);
}

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_args_t shell_args;

static THRD_STACK(worker_stack, 1024);
static void *worker_thrd(void *arg_p)
{
    struct time_t start, now;

    thrd_set_name("worker");

    while (1) {
        thrd_usleep(10000 * (100 - worker_cpu_usage));

        time_get(&start);

        do {
            time_get(&now);
        } while ((now.seconds - start.seconds) < worker_cpu_usage);
    }

    return (NULL);
}

int main()
{
    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 115200, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    sys_set_stdout(&uart.chout);

    COUNTER_INC(foo, 0xfffd);
    COUNTER_INC(foo, 2);
    COUNTER_INC(bar, 339283982393);
    COUNTER_INC(fie, 1);

    std_printf(sys_get_info());

    thrd_spawn(worker_thrd, NULL, 20, worker_stack, sizeof(worker_stack));

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_main(&shell_args);

    return (0);
}
