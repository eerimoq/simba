/**
 * @file main.c
 * @version 0.3.0
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

static struct fs_command_t cmd_tmp_foo;
static struct fs_command_t cmd_tmp_bar;

static struct fs_counter_t foo;
static struct fs_counter_t bar;
static struct fs_counter_t fie;

static int tmp_fie_value = 57;
static struct fs_parameter_t tmp_fie;

static int tmp_foo(int argc,
                   const char *argv[],
                   chan_t *out_p,
                   chan_t *in_p,
                   void *arg_p,
                   void *call_arg_p)
{
    BTASSERT(argc == 4);
    std_fprintf(out_p,
                FSTR("argc = %d, argv[0] = %s, argv[1] = %s, argv[2] = %s\n"),
                argc,
                argv[0],
                argv[1],
                argv[2]);

    return (0);
}

static int tmp_bar(int argc,
                   const char *argv[],
                   chan_t *out_p,
                   chan_t *in_p,
                   void *arg_p,
                   void *call_arg_p)
{
    int i;

    BTASSERT(argc == 2);
    i = (int)(*argv[1]) - '0';
    std_fprintf(out_p, FSTR("bar %d\n"), 2 * i);

    return (0);
}

static char qinbuf[8];
static QUEUE_INIT_DECL(qin, qinbuf, sizeof(qinbuf));
static char qoutbuf[256];
static QUEUE_INIT_DECL(qout, qoutbuf, sizeof(qoutbuf));

static THRD_STACK(shell_stack, 1024);
static struct shell_args_t shell_args = {
    .chin_p = &qin,
    .chout_p = &qout,
    .username_p = "erik",
    .password_p = "pannkaka"
};

#define BUFFER_SIZE 512

static int chout_read_until(char *buf_p,
                            const char *pattern)
{
    char c;
    size_t length = 0;
    size_t pattern_length = strlen(pattern);

    while (length < BUFFER_SIZE - 1) {
        chan_read(&qout, &c, sizeof(c));

        std_printf(FSTR("%c"), c);

        *buf_p++ = c;
        length++;
        *buf_p = '\0';

        /* Compare to pattern. */
        if (length >= pattern_length) {
            if (!strcmp(&buf_p[-pattern_length], pattern)) {
                return (1);
            }
        }
    }

    return (0);
}

static int chout_read_until_prompt(char *buf_p)
{
    return (chout_read_until(buf_p, "$ "));
}

static int test_all(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    thrd_spawn(shell_main,
               &shell_args,
               0,
               shell_stack,
               sizeof(shell_stack));

    /* Login. */
    /* Username. */
    chout_read_until(buf, "username: ");
    BTASSERT(std_strcmp(buf, FSTR("username: ")) == 0, "%s\n", buf);
    chan_write(&qin, "erik\n", sizeof("erik\n") - 1);
    chout_read_until(buf, "erik\n");
    BTASSERT(std_strcmp(buf, FSTR("erik\n")) == 0, "%s\n", buf);

    /* Password. */
    chout_read_until(buf, "password: ");
    BTASSERT(std_strcmp(buf, FSTR("password: ")) == 0, "%s\n", buf);
    chan_write(&qin, "pannkaka\n", sizeof("pannkaka\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf, FSTR("********\n$ ")) == 0, "%s\n", buf);

    /* Empty line. */
    chan_write(&qin, "\n", sizeof("\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf, FSTR("\n$ ")) == 0, "%s\n", buf);

    /* Command not found. */
    chan_write(&qin, "/1/2/3\n", sizeof("/1/2/3\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("/1/2/3\n"
                             "/1/2/3: command not found\r\n"
                             "$ ")) == 0, "%s\n", buf);

    /* Command too long. */
    chan_write(&qin,
               "This command is too long for the shell to handle. "
               "The maximum command length is probably 64 characters.\n",
               membersof("This command is too long for the shell to handle. "
                         "The maximum command length is probably 64 characters.\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("This command is too long for the shell to handle. "
                             "The maximum co\r\n"
                             "shell: error: command too long\r\n"
                             "$ ")) == 0, "%s\n", buf);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("mmand length is probably 64 characters.\n"
                             "mmand: command not found\r\n"
                             "$ ")) == 0, "%s\n", buf);

    /* Auto completion. */
    chan_write(&qin, "\t", membersof("\t") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("\r\n"
                             "bar\r\n"
#if !defined(ARCH_LINUX) && !defined(ARCH_ESP)
                             "drivers/\r\n"
#endif
                             "fie\r\n"
                             "foo\r\n"
                             "kernel/\r\n"
                             "logout\r\n"
                             "tmp/\r\n"
                             "$ ")) == 0, "%s\n", buf);

    /* Test command foo. */
    chan_write(&qin, "/tmp/foo 3 2 1\n", membersof("/tmp/foo 3 2 1\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("/tmp/foo 3 2 1\n"
                             "argc = 4, argv[0] = /tmp/foo, argv[1] = 3, argv[2] = 2\n"
                             "$ ")) == 0, "%s\n", buf);

    /* Test command bar. */
    chan_write(&qin, "/tmp/bar 3\n", membersof("/tmp/bar 3\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("/tmp/bar 3\n"
                             "bar 6\n"
                             "$ ")) == 0, "%s\n", buf);

    /* Get parameter value. */
    chan_write(&qin, "/tmp/fie\n", membersof("/tmp/fie\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("/tmp/fie\n"
                             "57\r\n"
                             "$ ")) == 0, "%s\n", buf);

    /* Set parameter value. */
    chan_write(&qin, "/tmp/fie 58\n", membersof("/tmp/fie 58\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("/tmp/fie 58\n$ ")) == 0, "%s\n", buf);

    /* Get modified parameter value. */
    chan_write(&qin, "/tmp/fie\n", membersof("/tmp/fie\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("/tmp/fie\n"
                             "58\r\n"
                             "$ ")) == 0, "%s\n", buf);

    /* Set main thread log mask. */
    chan_write(&qin, "/kernel/thrd/set_log_mask shell 0xff\r\n",
               membersof("/kernel/thrd/set_log_mask shell 0xff\r\n") - 1);
    chout_read_until_prompt(buf);

#if defined(ARCH_LINUX) || defined(ARCH_ESP)
    /* List thrds. */
    chan_write(&qin, "/kernel/thrd/list\r\n", membersof("/kernel/thrd/list\r\n") - 1);
    /* Read until a prompt is found to empty the buffer. */
    BTASSERT(chout_read_until_prompt(buf) == 1);

    /* List counters. */
    fs_counter_increment(&foo, 2);
    fs_counter_increment(&foo, 2);
    fs_counter_increment(&bar, 339283982393);
    fs_counter_increment(&fie, 1);
    chan_write(&qin, "/kernel/fs/counters_list\r\n", membersof("/kernel/fs/counters_list\r\n") - 1);
    BTASSERT(chout_read_until_prompt(buf) == 1);
    BTASSERT(std_strcmp(buf,
                        FSTR("/kernel/fs/counters_list\r\n"
                             "NAME                                                 VALUE\r\n"
                             "/foo                                                 "
                             "0000000000000004\r\n"
                             "/bar                                                 "
                             "0000004efee6b839\r\n"
                             "/fie                                                 "
                             "0000000000000001\r\n"
                             "$ ")) == 0, "%s", buf);
#endif

    /* Logout. */
    chan_write(&qin, "logout\n", sizeof("logout\n") - 1);
    chout_read_until(buf, "username: ");
    BTASSERT(std_strcmp(buf, FSTR("logout\nusername: ")) == 0, "%s\n", buf);

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
    uart_module_init();

    /* Setup the commands. */
    fs_command_init(&cmd_tmp_foo, FSTR("/tmp/foo"), tmp_foo, NULL);
    fs_command_register(&cmd_tmp_foo);
    fs_command_init(&cmd_tmp_bar, FSTR("/tmp/bar"), tmp_bar, NULL);
    fs_command_register(&cmd_tmp_bar);

    /* Setup the counters. */
    fs_counter_init(&fie, FSTR("/fie"), 0);
    fs_counter_register(&fie);
    fs_counter_init(&bar, FSTR("/bar"), 0);
    fs_counter_register(&bar);
    fs_counter_init(&foo, FSTR("/foo"), 0);
    fs_counter_register(&foo);

    /* Setup the parameter. */
    fs_parameter_init(&tmp_fie,
                      FSTR("/tmp/fie"),
                      fs_cmd_parameter_int,
                      &tmp_fie_value);
    fs_parameter_register(&tmp_fie);

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
