/**
 * @file main.c
 * @version 1.0.0
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
static struct shell_t shell;

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

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(shell_init(&shell,
                        &qin,
                        &qout,
                        NULL,
                        NULL,
                        "erik",
                        "pannkaka") == 0);

    BTASSERT(thrd_spawn(shell_main,
                        &shell,
                        0,
                        shell_stack,
                        sizeof(shell_stack)) != NULL);

    return (0);
}

static int test_login(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

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

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_empty_line(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Empty line. */
    chan_write(&qin, "\n", sizeof("\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf, FSTR("\n$ ")) == 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_command_not_found(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Command not found. */
    chan_write(&qin, "/1/2/3\r\n", sizeof("/1/2/3\r\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("/1/2/3\r\n"
                             "/1/2/3: command not found\r\n"
                             "$ ")) == 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_command_too_long(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Command too long. */
    chan_write(&qin,
               "This command is too long for the shell to handle. "
               "The maximum command length is probably 64 characters.\n",
               membersof("This command is too long for the shell to "
                         "handle. The maximum command length is probably "
                         "64 characters.\n") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("This command is too long for the shell to "
                             "handle. The maximum c\n"
                             "This: command not found\r\n"
                             "$ ")) == 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_auto_completion(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Auto completion. */
    chan_write(&qin, "\t", membersof("\t") - 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("\r\n"
                             "bar\r\n"
#if !defined(ARCH_LINUX)
                             "drivers/\r\n"
#endif
                             "fie\r\n"
                             "foo\r\n"
                             "help\r\n"
                             "history\r\n"
                             "kernel/\r\n"
                             "logout\r\n"
                             "tmp/\r\n"
                             "$ ")) == 0, "%s\n", buf);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_commands_foo_and_bar(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

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

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_parameters(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

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

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_set_log_mask(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Set main thread log mask. */
    chan_write(&qin, "/kernel/thrd/set_log_mask shell 0xff\r\n",
               membersof("/kernel/thrd/set_log_mask shell 0xff\r\n") - 1);
    chout_read_until_prompt(buf);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_thrd_list(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)
    char buf[BUFFER_SIZE];

    /* List thrds. */
    chan_write(&qin, "/kernel/thrd/list\r\n", membersof("/kernel/thrd/list\r\n") - 1);
    /* Read until a prompt is found to empty the buffer. */
    BTASSERT(chout_read_until_prompt(buf) == 1);

    std_printf(FSTR("\r\n"));

#endif

    return (0);
}

static int test_fs_counters_list(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)
    char buf[BUFFER_SIZE];

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

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_cursor_movement(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Write '123' to the shell. */
    chan_write(&qin, "123", sizeof("123") - 1);
    chout_read_until(buf, "123");
    BTASSERT(std_strcmp(buf, FSTR("123")) == 0);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    chout_read_until(buf, "\x1b[3D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[3D")) == 0);

    /* Move the cursor one step to the right (right arrow).*/
    chan_write(&qin, "\x1b[C", 3);
    chout_read_until(buf, "\x1b[1C");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[1C")) == 0);

    /* Move the cursor to the end of the line (Ctrl+E).*/
    chan_write(&qin, "\x05", 1);
    chout_read_until(buf, "\x1b[2C");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[2C")) == 0);

    /* Move the cursor one step to the right (right arrow). This
       should not output anything since the cursor is already at the
       end of the line. */
    chan_write(&qin, "\x1b[C", 3);

    /* Move one left and then right. */
    chan_write(&qin, "\x1b[D", 3);
    chan_write(&qin, "\x1b[C", 3);
    chout_read_until(buf, "\x1b[1D\x1b[1C");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[1D\x1b[1C")) == 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    chout_read_until_prompt(buf);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_cursor_insert(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Write 'ep' to the shell. */
    chan_write(&qin, "ep", sizeof("ep") - 1);
    chout_read_until(buf, "ep");
    BTASSERT(std_strcmp(buf, FSTR("ep")) == 0);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    chout_read_until(buf, "\x1b[2D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[2D")) == 0);

    /* Write 'h' at the beginning of a line. The cursor should be
       advanced by one (in this case moving 2 steps to the left after
       hep has been printed).*/
    chan_write(&qin, "h", 1);
    chout_read_until(buf, "hep\x1b[2D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[Khep\x1b[2D")) == 0);

    /* Move the cursor right once to insert 'l' to complete the word
       'help'. */
    chan_write(&qin, "\x1b[C", 3);
    chout_read_until(buf, "\x1b[1C");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[1C")) == 0);

    /* Write 'l'.*/
    chan_write(&qin, "l", 1);
    chout_read_until(buf, "help");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[2D\x1b[Khelp")) == 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    chout_read_until(buf, "Cursor movement");
    chout_read_until(buf, "Edit");
    chout_read_until(buf, "History");
    chout_read_until_prompt(buf);

    /* Write 'ff'.*/
    chan_write(&qin, "fff", 3);
    chout_read_until(buf, "fff");
    BTASSERT(std_strcmp(buf, FSTR("fff")) == 0);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    chout_read_until(buf, "\x1b[3D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[3D")) == 0);

    /* Delete the first 'f' (Ctrl+D). */
    chan_write(&qin, "\x04", 1);
    chout_read_until(buf, "ff");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[Kff")) == 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    chout_read_until_prompt(buf);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_backspace(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Delete from empty line. */
    chan_write(&qin, "\x7f", 1);

    /* Write '1' to the shell. */
    chan_write(&qin, "1", sizeof("1") - 1);
    chout_read_until(buf, "1");
    BTASSERT(std_strcmp(buf, FSTR("1")) == 0);

    /* Delete the '1'. */
    chan_write(&qin, "\x7f", 1);
    chout_read_until(buf, "\x08 \x08");
    BTASSERT(std_strcmp(buf, FSTR("\x08 \x08")) == 0);

    /* Write '123' to the shell. */
    chan_write(&qin, "123", sizeof("123") - 1);
    chout_read_until(buf, "123");
    BTASSERT(std_strcmp(buf, FSTR("123")) == 0);

    /* Move the cursor one step to the left. */
    chan_write(&qin, "\x1b[D", 3);
    chout_read_until(buf, "\x1b[1D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[1D")) == 0);

    /* Delete the '2' with the backspace key. */
    chan_write(&qin, "\x7f", 1);
    chout_read_until(buf, "13\x1b[1D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[2D\x1b[K13\x1b[1D")) == 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf, FSTR("\r\n13: command not found\r\n$ ")) == 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history_clear(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Clear the history. */
    chan_write(&qin, "history -c\n", sizeof("history -c\n") - 1);
    chout_read_until_prompt(buf);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Print the history. */
    chan_write(&qin, "history\n", sizeof("history\n") - 1);
    chout_read_until_prompt(buf);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history_up_down(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Go to the previous command. */
    chan_write(&qin, "\x1b[A", 3);
    chan_write(&qin, "\x1b[A", 3);
    chout_read_until(buf, "history");
    BTASSERT(std_strcmp(buf, FSTR("history")) == 0);

    /* Go to the next command (there is none). */
    chan_write(&qin, "\x1b[B", 3);
    chan_write(&qin, "\x1b[B", 3);
    chout_read_until(buf, "\x08 \x08\x08 \x08\x08 \x08\x08 \x08"
                     "\x08 \x08\x08 \x08\x08 \x08");
    BTASSERT(std_strcmp(buf, FSTR("\x08 \x08\x08 \x08\x08 \x08\x08 \x08"
                                  "\x08 \x08\x08 \x08\x08 \x08")) == 0);

    /* Start typing a command but do not hit enter. */
    chan_write(&qin, "hello", 5);
    chout_read_until(buf, "hello");
    BTASSERT(std_strcmp(buf, FSTR("hello")) == 0);

    /* Go to the previous command. */
    chan_write(&qin, "\x1b[A", 3);
    chout_read_until(buf, "history");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[5D\x1b[Khistory")) == 0);

    /* Go to the next command. The text 'hello' should be printed. */
    chan_write(&qin, "\x1b[B", 3);
    chout_read_until(buf, "hello");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[7D\x1b[Khello")) == 0);

    /* Hit enter to execute 'hello'. */
    chan_write(&qin, "\n", 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("\nhello: command not found\r\n$ ")) == 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history_search(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Send Ctrl+R and search for the 'hello' command. */
    chan_write(&qin, "\x12", 1);
    chout_read_until(buf, "(history-search)`': \x1b[3D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[K"
                                  "(history-search)`': "
                                  "\x1b[3D")) == 0);

    chan_write(&qin, "h", 1);
    chout_read_until(buf, "hello\x1b[8D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[K"
                                  "h': "
                                  "hello"
                                  "\x1b[8D")) == 0);

    /* Hit enter to execute 'hello'. */
    chan_write(&qin, "\n", 1);
    chout_read_until_prompt(buf);
    BTASSERT(std_strcmp(buf,
                        FSTR("\x1b[18D\x1b[Khello\n"
                             "hello: command not found\r\n$ ")) == 0);

    /* Send Ctrl+R and search for the 'history' command. */
    chan_write(&qin, "\x12", 1);
    chout_read_until(buf, "(history-search)`': \x1b[3D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[K"
                                  "(history-search)`': "
                                  "\x1b[3D")) == 0);

    chan_write(&qin, "h", 1);
    chout_read_until(buf, "hello\x1b[8D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[K"
                                  "h': "
                                  "hello"
                                  "\x1b[8D")) == 0);

    chan_write(&qin, "i", 1);
    chout_read_until(buf, "history\x1b[10D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[K"
                                  "i': "
                                  "history"
                                  "\x1b[10D")) == 0);

    /* Delete the 'i' to find hello again. */
    chan_write(&qin, "\x7f", 1);
    chout_read_until(buf, "hello\x1b[8D");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[1D"
                                  "\x1b[K"
                                  "': "
                                  "hello"
                                  "\x1b[8D")) == 0);

    /* Ctrl+G to abort the search. */
    chan_write(&qin, "\x07", 1);
    chout_read_until(buf, "\x1b[K");
    BTASSERT(std_strcmp(buf, FSTR("\x1b[18D"
                                  "\x1b[K")) == 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_logout(struct harness_t *harness_p)
{
    char buf[BUFFER_SIZE];

    /* Logout. */
    chan_write(&qin, "logout\n", sizeof("logout\n") - 1);
    chout_read_until(buf, "username: ");
    BTASSERT(std_strcmp(buf, FSTR("logout\nusername: ")) == 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_login, "test_login" },
        { test_empty_line, "test_empty_line" },
        { test_command_not_found, "test_command_not_found" },
        { test_command_too_long, "test_command_too_long" },
        { test_auto_completion, "test_auto_completion" },
        { test_commands_foo_and_bar, "test_commands_foo_and_bar" },
        { test_parameters, "test_parameters" },
        { test_set_log_mask, "test_set_log_mask" },
        { test_thrd_list, "test_thrd_list" },
        { test_fs_counters_list, "test_fs_counters_list" },
        { test_cursor_movement, "test_cursor_movement" },
        { test_cursor_insert, "test_cursor_insert" },
        { test_backspace, "test_backspace" },
        { test_history_clear, "test_history_clear" },
        { test_history, "test_history" },
        { test_history_up_down, "test_history_up_down" },
        { test_history_search, "test_history_search" },
        { test_logout, "test_logout" },
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
