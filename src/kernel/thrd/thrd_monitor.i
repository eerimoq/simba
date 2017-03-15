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

#if !defined(THRD_MONITOR_PRIO)
#    define THRD_MONITOR_PRIO -80
#endif

struct monitor_t {
    long period_us;
    int print;
};

static struct monitor_t monitor = {
    .period_us = CONFIG_MONITOR_THREAD_PERIOD_US,
    .print = 0
};

/* Stacks. */
static THRD_STACK(monitor_thrd_stack, CONFIG_THRD_MONITOR_STACK_SIZE);

static int cmd_monitor_set_period_ms_cb(int argc,
                                        const char *argv[],
                                        void *out_p,
                                        void *in_p,
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
                                    void *out_p,
                                    void *in_p,
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
static int update_cpu_usage(int print)
{
    struct thrd_t *thrd_p;

    thrd_p = module.threads_p;

    while (thrd_p != NULL) {
        /* Get and reset the thread cpu usage. */
        thrd_p->statistics.cpu.usage = thrd_port_cpu_usage_get(thrd_p);
        thrd_port_cpu_usage_reset(thrd_p);

        if (print == 1) {
#if CONFIG_FLOAT == 1
            std_printf(FSTR("%20s %10f%%\r\n"),
                       thrd_p->name_p,
                       (float)thrd_p->statistics.cpu.usage);
#else
            std_printf(FSTR("%20s %10d%%\r\n"),
                       thrd_p->name_p,
                       (int)thrd_p->statistics.cpu.usage);
#endif
        }

        thrd_p = thrd_p->next_p;
    }

    return (0);
}

/**
 * The monitor thread monitors the cpu usage of all threads.
 */
static void *monitor_thrd(void *arg_p)
{
    int print;
    cpu_usage_t irq_usage;

    thrd_set_name("monitor");

    while (1) {
        thrd_sleep_us(monitor.period_us);
        print = monitor.print;

        /* Get and reset the interrupt CPU usage. */
        irq_usage = sys_interrupt_cpu_usage_get();
        sys_interrupt_cpu_usage_reset();

        if (print == 1) {
#if CONFIG_FLOAT == 1
            std_printf(FSTR("\r\n                NAME         CPU\r\n"
                            "                 irq %10f%%\r\n"),
                       (float)irq_usage);
#else
            std_printf(FSTR("\r\n                NAME         CPU\r\n"
                            "                 irq %10d%%\r\n"),
                       (int)irq_usage);
#endif
        }

        update_cpu_usage(print);
    }

    return (NULL);
}
