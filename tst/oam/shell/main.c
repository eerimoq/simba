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

static struct fs_command_t cmd_tmp_foo;
static struct fs_command_t cmd_tmp_bar;

static struct fs_counter_t foo;
static struct fs_counter_t bar;
static struct fs_counter_t fie;

static int tmp_fie_value = 57;
static struct fs_parameter_t tmp_fie;

static int tmp_foo(int argc,
                   const char *argv[],
                   void *out_p,
                   void *in_p,
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
                   void *out_p,
                   void *in_p,
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
    /* Bad username. */
    BTASSERT(harness_expect(&qout, "username: ", NULL) > 0);
    chan_write(&qin, "bad\r\n", 5);
    BTASSERT(harness_expect(&qout, "bad\r\n", NULL) > 0);

    /* Ok password. */
    BTASSERT(harness_expect(&qout, "password: ", NULL) > 0);
    chan_write(&qin, "pannkaka\n", 9);
    BTASSERT(harness_expect(&qout,
                            "********\nauthentication failure\r\n",
                            NULL) > 0);

    /* Ok username. */
    BTASSERT(harness_expect(&qout, "username: ", NULL) > 0);
    chan_write(&qin, "erik\r\n", 6);
    BTASSERT(harness_expect(&qout, "erik\r\n", NULL) > 0);

    /* Bad password. */
    BTASSERT(harness_expect(&qout, "password: ", NULL) > 0);
    chan_write(&qin, "bad\n", 4);
    BTASSERT(harness_expect(&qout,
                            "***\nauthentication failure\r\n",
                            NULL) > 0);

    /* Misspell the username ... */
    BTASSERT(harness_expect(&qout, "username: ", NULL) > 0);
    chan_write(&qin, "eril", 4);
    BTASSERT(harness_expect(&qout, "eril", NULL) > 0);

    /* ... and correct it using backspace. */
    chan_write(&qin, "\x7fk\r\n", 4);
    BTASSERT(harness_expect(&qout, "\x08 \x08k\r\n", NULL) > 0);

    /* OK password. */
    BTASSERT(harness_expect(&qout, "password: ", NULL) > 0);
    chan_write(&qin, "pannkaka\n", 9);
    BTASSERT(harness_expect(&qout, "********\n$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_empty_line(struct harness_t *harness_p)
{
    /* Empty line. */
    chan_write(&qin, "\n", sizeof("\n") - 1);
    BTASSERT(harness_expect(&qout, "\n$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_command_not_found(struct harness_t *harness_p)
{
    /* Command not found. */
    chan_write(&qin, "/1/2/3\r\n", sizeof("/1/2/3\r\n") - 1);
    BTASSERT(harness_expect(&qout,
                            "/1/2/3\r\n"
                            "/1/2/3: command not found\r\n"
                            "ERROR(-1003)\r\n"
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_command_too_long(struct harness_t *harness_p)
{
    /* Command too long. */
    chan_write(&qin,
               "This command is too long for the shell to handle. "
               "The maximum command length is probably 64 characters.\n",
               membersof("This command is too long for the shell to "
                         "handle. The maximum command length is probably "
                         "64 characters.\n") - 1);
    BTASSERT(harness_expect(&qout,
                            "This command is too long for the shell to "
                            "handle. The maximum c\n"
                            "This: command not found\r\n"
                            "ERROR(-1003)\r\n"
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_auto_completion(struct harness_t *harness_p)
{
    /* Auto completion. */
    chan_write(&qin, "\t", 1);
    BTASSERT(harness_expect(&qout,
                            "\r\n"
                            "bar\r\n"
#if !defined(ARCH_LINUX) && !defined(ARCH_PPC)
                            "drivers/\r\n"
#endif
                            "fie\r\n"
                            "filesystems/\r\n"
                            "foo\r\n"
                            "help\r\n"
                            "history\r\n"
                            "kernel/\r\n"
                            "logout\r\n"
                            "tmp/\r\n"
                            "$ ",
                            NULL) > 0);

    /* Auto completion. */
    chan_write(&qin, "ba\t\r\n", 5);
    BTASSERT(harness_expect(&qout,
                            "bar \r\n"
                            "0000000000000000\r\n"
                            "OK\r\n"
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_commands_foo_and_bar(struct harness_t *harness_p)
{
    /* Test command foo. */
    chan_write(&qin, "/tmp/foo 3 2 1\n", 15);
    BTASSERT(harness_expect(&qout,
                            "/tmp/foo 3 2 1\n"
                            "argc = 4, argv[0] = /tmp/foo, argv[1] = 3, argv[2] = 2\n"
                            "OK\r\n"
                            "$ ",
                            NULL) > 0);

    /* Test command bar. */
    chan_write(&qin, "/tmp/bar 3\n", 11);
    BTASSERT(harness_expect(&qout,
                            "/tmp/bar 3\n"
                            "bar 6\n"
                            "OK\r\n"
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_parameters(struct harness_t *harness_p)
{
    /* Get parameter value. */
    chan_write(&qin, "/tmp/fie\n", 9);
    BTASSERT(harness_expect(&qout,
                            "/tmp/fie\n"
                            "57\r\n"
                            "OK\r\n"
                            "$ ",
                            NULL) > 0);

    /* Set parameter value. */
    chan_write(&qin, "/tmp/fie 58\n", 12);
    BTASSERT(harness_expect(&qout,
                            "/tmp/fie 58\n"
                            "OK\r\n"
                            "$ ",
                            NULL) > 0);

    /* Get modified parameter value. */
    chan_write(&qin, "/tmp/fie\n", 9);
    BTASSERT(harness_expect(&qout,
                            "/tmp/fie\n"
                            "58\r\n"
                            "OK\r\n"
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_set_log_mask(struct harness_t *harness_p)
{
    /* Set main thread log mask. */
    chan_write(&qin, "/kernel/thrd/set_log_mask shell 0xff\r\n", 38);
    BTASSERT(harness_expect(&qout,
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_thrd_list(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)
    /* List thrds. */
    chan_write(&qin, "/kernel/thrd/list\r\n", 19);
    /* Read until a prompt is found to empty the buffer. */
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

#endif

    return (0);
}

static int test_fs_counters_list(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    /* List counters. */
    fs_counter_increment(&foo, 2);
    fs_counter_increment(&foo, 2);
    fs_counter_increment(&bar, 339283982393);
    fs_counter_increment(&fie, 1);
    chan_write(&qin, "/filesystems/fs/counters/list\r\n", 31);
    BTASSERT(harness_expect(&qout,
                            "/filesystems/fs/counters/list\r\n"
                            "NAME                                                 VALUE\r\n"
                            "/foo                                                 "
                            "0000000000000004\r\n"
                            "/bar                                                 "
                            "0000004efee6b839\r\n"
                             "/fie                                                 "
                            "0000000000000001\r\n"
                            "OK\r\n"
                            "$ ",
                            NULL) > 0);

#endif

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_cursor_movement(struct harness_t *harness_p)
{
    /* Write '123' to the shell. */
    chan_write(&qin, "123", 3);
    BTASSERT(harness_expect(&qout, "123", NULL) > 0);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    BTASSERT(harness_expect(&qout, "\x1b[3D", NULL) > 0);

    /* Move the cursor one step to the right (right arrow).*/
    chan_write(&qin, "\x1b[C", 3);
    BTASSERT(harness_expect(&qout, "\x1b[1C", NULL) > 0);

    /* Move the cursor to the end of the line (Ctrl+E).*/
    chan_write(&qin, "\x05", 1);
    BTASSERT(harness_expect(&qout, "\x1b[2C", NULL) > 0);

    /* Move the cursor to the beginning of the line (HOME). */
    chan_write(&qin, "\x1bOH", 3);
    BTASSERT(harness_expect(&qout, "\x1b[3D", NULL) > 0);

    /* Move the cursor to the end of the line (END).*/
    chan_write(&qin, "\x1bOF", 3);
    BTASSERT(harness_expect(&qout, "\x1b[3C", NULL) > 0);

    /* Move the cursor one step to the right (right arrow). This
       should not output anything since the cursor is already at the
       end of the line. */
    chan_write(&qin, "\x1b[C", 3);

    /* Move one left and then right. */
    chan_write(&qin, "\x1b[D", 3);
    chan_write(&qin, "\x1b[C", 3);
    BTASSERT(harness_expect(&qout, "\x1b[1D\x1b[1C", NULL) > 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_cursor_insert(struct harness_t *harness_p)
{
    /* Write 'ep' to the shell. */
    chan_write(&qin, "ep", 2);
    BTASSERT(harness_expect(&qout, "ep", NULL) > 0);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    BTASSERT(harness_expect(&qout, "\x1b[2D", NULL) > 0);

    /* Write 'h' at the beginning of a line. The cursor should be
       advanced by one (in this case moving 2 steps to the left after
       hep has been printed).*/
    chan_write(&qin, "h", 1);
    BTASSERT(harness_expect(&qout, "\x1b[Khep\x1b[2D", NULL) > 0);

    /* Move the cursor right once to insert 'l' to complete the word
       'help'. */
    chan_write(&qin, "\x1b[C", 3);
    BTASSERT(harness_expect(&qout, "\x1b[1C", NULL) > 0);

    /* Write 'l'.*/
    chan_write(&qin, "l", 1);
    BTASSERT(harness_expect(&qout, "\x1b[2D\x1b[Khelp", NULL) > 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    BTASSERT(harness_expect(&qout, "Cursor movement", NULL) > 0);
    BTASSERT(harness_expect(&qout, "Edit", NULL) > 0);
    BTASSERT(harness_expect(&qout, "History", NULL) > 0);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    /* Write 'ff'.*/
    chan_write(&qin, "fff", 3);
    BTASSERT(harness_expect(&qout, "fff", NULL) > 0);

    /* Delete a character at the end of the line. Nothing should
       happen (Ctrl+D). */
    chan_write(&qin, "\x04", 1);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    BTASSERT(harness_expect(&qout, "\x1b[3D", NULL) > 0);

    /* Delete the first 'f' (Ctrl+D). */
    chan_write(&qin, "\x04", 1);
    BTASSERT(harness_expect(&qout, "\x1b[Kff", NULL) > 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_cursor_cut(struct harness_t *harness_p)
{
    /* Write 'foo' to the shell. */
    chan_write(&qin, "foo", 3);
    BTASSERT(harness_expect(&qout, "foo", NULL) > 0);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    BTASSERT(harness_expect(&qout, "\x1b[3D", NULL) > 0);

    /* Cut the whole line (Ctrl+K). */
    chan_write(&qin, "\x0b", 1);
    BTASSERT(harness_expect(&qout, "\x08 \x08\x08 \x08\x08 \x08", NULL) > 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_swap_characters(struct harness_t *harness_p)
{
    /* Write 'b' to the shell. */
    chan_write(&qin, "b", 1);
    BTASSERT(harness_expect(&qout, "b", NULL) > 0);

    /* Cannot swap one character. At least two are needed. (Ctrl+T). */
    chan_write(&qin, "\x14", 1);

    /* Write 'ra' to the shell. */
    chan_write(&qin, "ra", 2);
    BTASSERT(harness_expect(&qout, "ra", NULL) > 0);

    /* Move the cursor to the beginning of the line (Ctrl+A). */
    chan_write(&qin, "\x01", 1);
    BTASSERT(harness_expect(&qout, "\x1b[3D", NULL) > 0);

    /* Cannot swap at the beginning of the line (Ctrl+T). */
    chan_write(&qin, "\x14", 1);

    /* Move the cursor to the end of the line (Ctrl+E).*/
    chan_write(&qin, "\x05", 1);
    BTASSERT(harness_expect(&qout, "\x1b[3C", NULL) > 0);

    /* Swap 'r' and 'a' to get "bar" (Ctrl+T). */
    chan_write(&qin, "\x14", 1);
    BTASSERT(harness_expect(&qout, "\x1b[3D\x1b[Kbar", NULL) > 0);

    /* Move the cursor one step to the left. */
    chan_write(&qin, "\x1b[D", 3);
    BTASSERT(harness_expect(&qout, "\x1b[1D", NULL) > 0);

    /* Swap 'a' and 'r' to get "bra" (Ctrl+T).*/
    chan_write(&qin, "\x14", 1);
    BTASSERT(harness_expect(&qout, "\x1b[2D\x1b[Kbra", NULL) > 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_backspace(struct harness_t *harness_p)
{
    /* Delete from empty line. */
    chan_write(&qin, "\x7f", 1);

    /* Write '1' to the shell. */
    chan_write(&qin, "1", 1);
    BTASSERT(harness_expect(&qout, "1", NULL) > 0);

    /* Delete the '1'. */
    chan_write(&qin, "\x7f", 1);
    BTASSERT(harness_expect(&qout, "\x08 \x08", NULL) > 0);

    /* Write '123' to the shell. */
    chan_write(&qin, "123", 3);
    BTASSERT(harness_expect(&qout, "123", NULL) > 0);

    /* Move the cursor one step to the left. */
    chan_write(&qin, "\x1b[D", 3);
    BTASSERT(harness_expect(&qout, "\x1b[1D", NULL) > 0);

    /* Delete the '2' with the backspace key. */
    chan_write(&qin, "\x7f", 1);
    BTASSERT(harness_expect(&qout, "\x1b[2D\x1b[K13\x1b[1D", NULL) > 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    BTASSERT(harness_expect(&qout,
                            "\r\n13: command not found\r\n"
                            "ERROR(-1003)\r\n"
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_delete_word(struct harness_t *harness_p)
{
    int i;

    /* Delete from empty line. */
    chan_write(&qin, "\x1b""d", 2);

    /* Write '1' to the shell. */
    chan_write(&qin, "123 456 789", 11);
    BTASSERT(harness_expect(&qout, "123 456 789", NULL) > 0);

    /* Move the cursor eight step to the left. */
    for (i = 0; i < 8; i++) {
        chan_write(&qin, "\x1b[D", 3);
        BTASSERT(harness_expect(&qout, "\x1b[1D", NULL) > 0);
    }

    /* Delete the word ' 456'. */
    chan_write(&qin, "\x1b""d", 2);
    BTASSERT(harness_expect(&qout, "\x1b[3D\x1b[K123 789\x1b[4D", NULL) > 0);

    /* Delete the word ' 789'. */
    chan_write(&qin, "\x1b""d", 2);
    BTASSERT(harness_expect(&qout,
                            "    \x08 \x08\x08 \x08\x08 \x08\x08 \x08",
                            NULL) > 0);

    /* Read until a new prompt to clean up. */
    chan_write(&qin, "\r\n", 2);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history_clear(struct harness_t *harness_p)
{
    /* Clear the history. */
    chan_write(&qin, "history -c\n", 11);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history(struct harness_t *harness_p)
{
    /* Print the history. */
    chan_write(&qin, "history\n", 8);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    /* Print the history. */
    chan_write(&qin, "/history\n", 9);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history_up_down(struct harness_t *harness_p)
{
    /* Clear the history. */
    chan_write(&qin, "history -c\n", 11);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    /* Print the history. */
    chan_write(&qin, "history\n", 8);
    BTASSERT(harness_expect(&qout, "$ ", NULL) > 0);

    /* Go to the previous command. */
    chan_write(&qin, "\x1b[A", 3);
    chan_write(&qin, "\x1b[A", 3);
    BTASSERT(harness_expect(&qout, "history", NULL) > 0);

    /* Go to the next command (there is none). */
    chan_write(&qin, "\x1b[B", 3);
    chan_write(&qin, "\x1b[B", 3);
    BTASSERT(harness_expect(&qout,
                            "\x08 \x08\x08 \x08\x08 \x08\x08 \x08"
                            "\x08 \x08\x08 \x08\x08 \x08",
                            NULL) > 0);

    /* Start typing a command but do not hit enter. */
    chan_write(&qin, "hello", 5);
    BTASSERT(harness_expect(&qout, "hello", NULL) > 0);

    /* Go to the previous command. */
    chan_write(&qin, "\x1b[A", 3);
    BTASSERT(harness_expect(&qout, "\x1b[5D\x1b[Khistory", NULL) > 0);

    /* Go to the next command. The text 'hello' should be printed. */
    chan_write(&qin, "\x1b[B", 3);
    BTASSERT(harness_expect(&qout, "\x1b[7D\x1b[Khello", NULL) > 0);

    /* Hit enter to execute 'hello'. */
    chan_write(&qin, "\n", 1);
    BTASSERT(harness_expect(&qout,
                            "\nhello: command not found\r\n"
                            "ERROR(-1003)\r\n"
                            "$ ",
                            NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_history_search(struct harness_t *harness_p)
{
    /* Send Ctrl+R and search for the 'hello' command. */
    chan_write(&qin, "\x12", 1);
    BTASSERT(harness_expect(&qout,
                            "\x1b[K(history-search)`': \x1b[3D",
                            NULL) > 0)

    chan_write(&qin, "h", 1);
    BTASSERT(harness_expect(&qout, "\x1b[Kh': hello\x1b[8D", NULL) > 0);

    /* Hit enter to execute 'hello'. */
    chan_write(&qin, "\n", 1);
    BTASSERT(harness_expect(&qout,
                            "\x1b[18D\x1b[Khello\n"
                            "hello: command not found\r\n"
                            "ERROR(-1003)\r\n"
                            "$ ",
                            NULL) > 0);

    /* Send Ctrl+R and search for the 'history' command. */
    chan_write(&qin, "\x12", 1);
    BTASSERT(harness_expect(&qout,
                            "\x1b[K(history-search)`': \x1b[3D",
                            NULL) > 0);

    chan_write(&qin, "h", 1);
    BTASSERT(harness_expect(&qout, "\x1b[Kh': hello\x1b[8D", NULL) > 0);

    chan_write(&qin, "i", 1);
    BTASSERT(harness_expect(&qout,
                            "\x1b[K"
                            "i': "
                            "history\x1b[10D",
                            NULL) > 0);

    /* Delete the 'i' to find hello again. */
    chan_write(&qin, "\x7f", 1);
    BTASSERT(harness_expect(&qout,
                            "\x1b[1D"
                            "\x1b[K"
                            "': "
                            "hello\x1b[8D",
                            NULL) > 0);

    /* Ctrl+G to abort the search. */
    chan_write(&qin, "\x07", 1);
    BTASSERT(harness_expect(&qout, "\x1b[18D\x1b[K", NULL) > 0);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_comment(struct harness_t *harness_p)
{
    /* Logout. */
    chan_write(&qin, " # this is a comment\n", 21);
    BTASSERTI(harness_expect(&qout, " # this is a comment\n$ ", NULL), ==, 23);

    std_printf(FSTR("\r\n"));

    return (0);
}

static int test_logout(struct harness_t *harness_p)
{
    /* Logout. */
    chan_write(&qin, "logout\n", 7);
    BTASSERT(harness_expect(&qout, "logout\nusername: ", NULL) > 0);

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
        { test_cursor_cut, "test_cursor_cut" },
        { test_swap_characters, "test_swap_characters" },
        { test_backspace, "test_backspace" },
        { test_delete_word, "test_delete_word" },
        { test_history_clear, "test_history_clear" },
        { test_history, "test_history" },
        { test_history_up_down, "test_history_up_down" },
        { test_history_search, "test_history_search" },
        { test_comment, "test_comment" },
        { test_logout, "test_logout" },
        { NULL, NULL }
    };

    sys_start();

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
                      fs_parameter_int_set,
                      fs_parameter_int_print,
                      &tmp_fie_value);
    fs_parameter_register(&tmp_fie);

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
