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

struct test_time_t {
    struct time_t time_in;
    sys_tick_t tick;
    struct time_t time_out;
};

void on_fatal(int error)
{
    std_printf(FSTR("on_fatal: error: %d\r\n"), error);
}

int call_assert(void)
{
    ASSERT(0 == 1, "intentional fatal assert");

    return (0);
}

int test_set_on_fatal_callback(void)
{
    sys_set_on_fatal_callback(on_fatal);

    return (0);
}

int test_non_fatal_assert(void)
{
    BTASSERT(call_assert() == -EASSERT);

    return (0);
}

int test_info(void)
{
    std_printf(sys_get_info());

#if CONFIG_SYS_FS_COMMAND_INFO == 1
    char buf[32];

    strcpy(buf, "/kernel/sys/info");
    BTASSERT(fs_call(buf, chan_null(), sys_get_stdout(), NULL) == 0);
#endif

    return (0);
}

int test_config(void)
{
    std_printf(sys_get_config());

#if CONFIG_SYS_FS_COMMAND_CONFIG == 1
    char buf[32];

    strcpy(buf, "/kernel/sys/config");
    BTASSERT(fs_call(buf, chan_null(), sys_get_stdout(), NULL) == 0);
#endif

    return (0);
}

int test_uptime(void)
{
    int res;
    struct time_t uptime;

    BTASSERT(sys_uptime(&uptime) == 0);
    std_printf(OSTR("seconds: %ld, nanosecons: %ld\r\n"),
               uptime.seconds,
               uptime.nanoseconds);

    sys_lock();
    res = sys_uptime_isr(&uptime);
    sys_unlock();

    std_printf(OSTR("seconds: %ld, nanosecons: %ld\r\n"),
               uptime.seconds,
               uptime.nanoseconds);

    BTASSERT(res == 0);

#if CONFIG_SYS_FS_COMMAND_UPTIME == 1
    char buf[32];

    strcpy(buf, "/kernel/sys/uptime");
    BTASSERT(fs_call(buf, chan_null(), sys_get_stdout(), NULL) == 0);
#endif

    return (0);
}

int test_repeated_uptime(void)
{
    struct time_t uptime[150];
    struct time_t diff;
    int i;
    int backwards;
    int failed;

    for (i = 0; i < membersof(uptime); i++) {
        BTASSERT(sys_uptime(&uptime[i]) == 0);
    }

    failed = 0;

    for (i = 1; i < membersof(uptime); i++) {
        time_subtract(&diff, &uptime[i], &uptime[i - 1]);
        backwards = ((diff.seconds < 0) || (diff.nanoseconds < 0));

        if (backwards == 1) {
            failed = -1;
        }

        std_printf(OSTR("seconds: %ld, nanoseconds: %ld%s\r\n"),
                   uptime[i].seconds,
                   uptime[i].nanoseconds,
                   (backwards ? " !" : ""));
    }

    BTASSERTI(failed, ==, 0);

    return (0);
}

int test_time(void)
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

int test_stdin(void)
{
    void *original_stdin_p;
    void *stdin_p;
    struct chan_t chan;

    original_stdin_p = sys_get_stdin();

    sys_set_stdin(&chan);
    stdin_p = sys_get_stdin();
    BTASSERT(stdin_p == &chan);

    sys_set_stdin(NULL);
    BTASSERT(sys_get_stdin() == chan_null());

    sys_set_stdin(original_stdin_p);

    return (0);
}

int test_stdout(void)
{
    void *original_stdout_p;
    void *stdout_p;
    struct chan_t chan;

    original_stdout_p = sys_get_stdout();

    sys_set_stdout(&chan);
    stdout_p = sys_get_stdout();
    BTASSERT(stdout_p == &chan);

    sys_set_stdout(NULL);
    BTASSERT(sys_get_stdout() == chan_null());

    sys_set_stdout(original_stdout_p);

    return (0);
}

int test_backtrace(void)
{
#if defined(ARCH_PPC)
    void *backtrace[4];
    char buf[32];

    BTASSERT(sys_backtrace(backtrace, sizeof(backtrace)) == 2);

    std_printf(FSTR("Backtrace:\r\n"));
    std_printf(OSTR("0x%08x:0x%08x 0x%08x:0x%08x\r\n"),
               backtrace[0],
               backtrace[1],
               backtrace[2],
               backtrace[3]);

    std_printf(FSTR("/kernel/sys/backtrace:\r\n"));
    strcpy(buf, "/kernel/sys/backtrace");
    BTASSERT(fs_call(buf, chan_null(), sys_get_stdout(), NULL) == 0);

    return (0);
#else
    return (1);
#endif
}

int test_div_ceil(void)
{
    BTASSERT(DIV_CEIL(0, 1) == 0);
    BTASSERT(DIV_CEIL(11, 5) == 3);
    BTASSERT(DIV_CEIL(11, 6) == 2);

    return (0);
}

int test_div_round(void)
{
    BTASSERT(DIV_ROUND(11, 1) == 11);
    BTASSERT(DIV_ROUND(11, 2) == 6);
    BTASSERT(DIV_ROUND(11, 3) == 4);
    BTASSERT(DIV_ROUND(11, 4) == 3);
    BTASSERT(DIV_ROUND(11, 5) == 2);
    BTASSERT(DIV_ROUND(11, 6) == 2);
    BTASSERT(DIV_ROUND(11, 7) == 2);
    BTASSERT(DIV_ROUND(11, 8) == 1);
    BTASSERT(DIV_ROUND(11, 9) == 1);
    BTASSERT(DIV_ROUND(11, 10) == 1);
    BTASSERT(DIV_ROUND(11, 11) == 1);

    return (0);
}

int test_reset_cause(void)
{
    char buf[32];
    enum sys_reset_cause_t reset_cause;

    reset_cause = (enum sys_reset_cause_t)(sys_reset_cause_max_t - 1);
    BTASSERT(sys_reset_cause_as_string(reset_cause) != NULL);

    reset_cause = sys_reset_cause();

    BTASSERT(reset_cause >= sys_reset_cause_unknown_t);
    BTASSERT(reset_cause < sys_reset_cause_max_t);

    std_printf(FSTR("Reset cause: %S (%d)\r\n"),
               sys_reset_cause_as_string(reset_cause),
               reset_cause);

    strcpy(&buf[0], "/kernel/sys/reset_cause");
    BTASSERT(fs_call(&buf[0], chan_null(), sys_get_stdout(), NULL) == 0);

    return (0);
}

int test_errno(void)
{
    BTASSERT(std_strcmp("Operation not permitted",
                        errno_as_string(EPERM)) == 0);
    BTASSERT(std_strcmp("Key was rejected by service",
                        errno_as_string(EKEYREJECTED)) == 0);

    BTASSERT(errno_as_string(EPERM - 1) == NULL);
    BTASSERT(errno_as_string(EKEYREJECTED + 1) == NULL);

    BTASSERT(std_strcmp("Stack corrupt",
                        errno_as_string(ESTACK)) == 0);
    BTASSERT(std_strcmp("Command not found",
                        errno_as_string(ENOCOMMAND)) == 0);

    BTASSERT(errno_as_string(ESTACK - 1) == NULL);
    BTASSERT(errno_as_string(ENOCOMMAND + 1) == NULL);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_set_on_fatal_callback, "test_set_on_fatal_callback" },
        { test_non_fatal_assert, "test_non_fatal_assert" },
        { test_info, "test_info" },
        { test_config, "test_config" },
        { test_uptime, "test_uptime" },
#if !defined(BOARD_ARDUINO_NANO) && !defined(BOARD_ARDUINO_UNO) && !defined(BOARD_ARDUINO_PRO_MICRO)
        { test_repeated_uptime, "test_repeated_uptime" },
        { test_time, "test_time" },
#endif
        { test_stdin, "test_stdin" },
        { test_stdout, "test_stdout" },
        { test_backtrace, "test_backtrace" },
        { test_div_ceil, "test_div_ceil" },
        { test_div_round, "test_div_round" },
        { test_reset_cause, "test_reset_cause" },
#if !defined(BOARD_ARDUINO_NANO) && !defined(BOARD_ARDUINO_UNO) && !defined(BOARD_ARDUINO_PRO_MICRO)
        { test_errno, "test_errno" },
#endif
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
