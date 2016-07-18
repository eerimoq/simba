/**
 * @file thrd_monitor.i
 * @version 2.0.0
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

#if !defined(THRD_MONITOR_PRIO)
#    define THRD_MONITOR_PRIO -80
#endif

struct monitor_t {
    long period_us;
    int print;
};

static struct fs_command_t cmd_monitor_set_period_ms;
static struct fs_command_t cmd_monitor_set_print;

static struct monitor_t monitor = {
    .period_us = 2000000,
    .print = 0
};

/* Stacks. */
static THRD_STACK(monitor_thrd_stack, THRD_MONITOR_STACK_MAX);
 
static int cmd_monitor_set_period_ms_cb(int argc,
                                        const char *argv[],
                                        chan_t *out_p,
                                        chan_t *in_p,
                                        void *arg_p,
                                        void *call_arg_p)
{
    long ms;

    if (argc != 2) {
        std_fprintf(out_p, FSTR("Usage: set_period_ms <milliseconds>\r\n"));

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &ms) == NULL) {
        return (-EINVAL);
    }

    monitor.period_us = (1000 * ms);

    return (0);
}

static int cmd_monitor_set_print_cb(int argc,
                                    const char *argv[],
                                    chan_t *out_p,
                                    chan_t *in_p,
                                    void *arg_p,
                                    void *call_arg_p)
{
    long print;

    if (argc != 2) {
        goto err_inval;
    }

    if (std_strtol(argv[1], &print) == NULL) {
        goto err_inval;
    }

    if ((print != 0) && (print != 1)) {
        goto err_inval;
    }

    monitor.print = print;

    return (0);

err_inval:
    std_fprintf(out_p, FSTR("Usage: set_print <1/0>\r\n"));

    return (-EINVAL);
}

/**
 * Called for each thread when iterating over all threads.
 */
static int update_cpu_usage(void *arg_p,
                            struct thrd_t *thrd_p)
{
    int *print_p = arg_p;

    /* Get and reset the thread cpu usage. */
    thrd_p->cpu.usage = thrd_port_cpu_usage_get(thrd_p);
    thrd_port_cpu_usage_reset(thrd_p);

    if (*print_p == 1) {
        std_printf(FSTR("%20s %10f%%\r\n"), thrd_p->name_p, thrd_p->cpu.usage);
    }

    return (0);
}

/**
 * The monitor thread monitors the cpu usage of all threads.
 */
static void *monitor_thrd(void *arg_p)
{
    int print;
    float irq_usage;

    thrd_set_name("monitor");

    while (1) {
        thrd_usleep(monitor.period_us);
        print = monitor.print;

        /* Get and reset the interrupt CPU usage. */
        irq_usage = sys_interrupt_cpu_usage_get();
        sys_interrupt_cpu_usage_reset();

        if (print == 1) {
            std_printf(FSTR("\r\n                NAME         CPU\r\n"
                            "                 irq %10f%%\r\n"),
                       irq_usage);
        }

        iterate_threads(update_cpu_usage, &print);
    }

    return (NULL);
}
