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

#include "simba.h"

struct test_time_t {
    struct time_t time_in;
    sys_tick_t tick;
    struct time_t time_out;
};

static void on_fatal(int error)
{
    std_printf(FSTR("on_fatal: error: %d\r\n"), error);
}

static int test_set_on_fatal_callback(struct harness_t *harness_p)
{
    sys_set_on_fatal_callback(on_fatal);
    ASSERT(0 == 1, "intentional fatal assert");

    return (0);
}

static int test_info(struct harness_t *harness_p)
{
    std_printf(sys_get_info());

#if CONFIG_FS_CMD_SYS_INFO == 1

    char buf[32];

    strcpy(buf, "/kernel/sys/info");
    BTASSERT(fs_call(buf, chan_null(), sys_get_stdout(), NULL) == 0);

#endif

    return (0);
}

static int test_config(struct harness_t *harness_p)
{
    std_printf(sys_get_config());

#if CONFIG_FS_CMD_SYS_CONFIG == 1

    char buf[32];

    strcpy(buf, "/kernel/sys/config");
    BTASSERT(fs_call(buf, chan_null(), sys_get_stdout(), NULL) == 0);

#endif

    return (0);
}

static int test_uptime(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SYS_UPTIME == 1

    char buf[32];

    strcpy(buf, "/kernel/sys/uptime");
    BTASSERT(fs_call(buf, chan_null(), sys_get_stdout(), NULL) == 0);

    return (0);

#else

    return (1);

#endif
}

static int test_time(struct harness_t *harness_p)
{
    int i;
    struct time_t time;
    sys_tick_t tick;

    static struct test_time_t test_times[] = {
        {
            .time_in = { .seconds = 1, .nanoseconds = 0 },
            .tick = 100,
            .time_out = { .seconds = 1, .nanoseconds = 0 }
        },

        {
            .time_in = { .seconds = 2, .nanoseconds = 0 },
            .tick = 200,
            .time_out = { .seconds = 2, .nanoseconds = 0 }
        },

        {
            .time_in = { .seconds = 2, .nanoseconds = 10000000 },
            .tick = 201,
            .time_out = { .seconds = 2, .nanoseconds = 10000000 }
        },

        /* nanoseconds rounded up. */
        {
            .time_in = { .seconds = 0, .nanoseconds = 15000000 },
            .tick = 2,
            .time_out = { .seconds = 0, .nanoseconds = 20000000 }
        },

        /* All zeroes. */
        {
            .time_in = { .seconds = 0, .nanoseconds = 0 },
            .tick = 0,
            .time_out = { .seconds = 0, .nanoseconds = 0 }
        },

        /* All zeroes. */
        {
            .time_in = { .seconds = 4325, .nanoseconds = 735000000 },
            .tick = 432574,
            .time_out = { .seconds = 4325, .nanoseconds = 740000000 }
        },

        /* Max time. */
        {
            .time_in = { .seconds = 4294967295 / 2, .nanoseconds = 0 },
            .tick = 214748364700,
            .time_out = { .seconds = 4294967295 / 2, .nanoseconds = 0 }
        },

        /* Max tick. */
        {
            .time_in = { .seconds = 42949672, .nanoseconds = 949999999 },
            .tick = 4294967295,
            .time_out = { .seconds = 42949672, .nanoseconds = 950000000 }
        }
    };

    for (i = 0; i < membersof(test_times); i++) {
        /* Convertion from the time struct to ticks. */
        tick = t2st(&test_times[i].time_in);
        BTASSERT(tick == test_times[i].tick);

        /* Convertion from ticks to the time struct. */
        st2t(tick, &time);
        BTASSERT(time.seconds == test_times[i].time_out.seconds)
        BTASSERT(time.nanoseconds == test_times[i].time_out.nanoseconds)
    }

    return (0);
}

static int test_stdin(struct harness_t *harness_p)
{
    void *original_stdin_p;
    void *stdin_p;
    struct chan_t chan;

    original_stdin_p = sys_get_stdin();

    sys_set_stdin(&chan);
    stdin_p = sys_get_stdin();

    sys_set_stdin(original_stdin_p);

    BTASSERT(stdin_p == &chan);

    return (0);
}

static int test_stdout(struct harness_t *harness_p)
{
    void *original_stdout_p;
    void *stdout_p;
    struct chan_t chan;

    original_stdout_p = sys_get_stdout();

    sys_set_stdout(&chan);
    stdout_p = sys_get_stdout();

    sys_set_stdout(original_stdout_p);

    BTASSERT(stdout_p == &chan);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_set_on_fatal_callback, "test_set_on_fatal_callback" },
        { test_info, "test_info" },
        { test_config, "test_config" },
        { test_uptime, "test_uptime" },
        { test_time, "test_time" },
        { test_stdin, "test_stdin" },
        { test_stdout, "test_stdout" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);
    
    return (0);
}
