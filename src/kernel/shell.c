/**
 * @file shell.c
 * @version 1.0
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

#define SHELL_COMMAND_MAX 64
#define SHELL_PROMPT "$ "

FS_COMMAND_DEFINE("/logout", shell_cmd_logout);

/**
 * Unused command callback. Logout handling in shell_entry().
 */
int shell_cmd_logout(int argc,
                     const char *argv[],
                     chan_t *chout_p,
                     chan_t *chin_p)
{
    return (-1);
}

static int shell_read_line(char *buf_p,
                           chan_t *chin_p,
                           chan_t *chout_p,
                           int sensitive)
{
    char c, *b_p = buf_p;
    int newline_found = 0;

    while (newline_found == 0) {
        chan_read(chin_p, &c, sizeof(c));

        if (c == '\n') {
            newline_found = 1;
        } else if (c == '\r') {
        } else if (c == 127) {
            /* Delete character. */
            if (b_p > buf_p) {
                b_p--;
                std_fprintf(chout_p, FSTR("\x08 \x08"));
                continue;
            }
        } else {
            *b_p++ = c;

            if (sensitive != 0) {
                c = '*';
            }
        }

        chan_write(chout_p, &c, sizeof(c));
    }

    return (0);
}

static int shell_login(char *buf_p,
                       chan_t *chin_p,
                       chan_t *chout_p,
                       const char *username_p,
                       const char *password_p)
{
    int correct_username = 0;
    int correct_password = 0;

    while (!(correct_username && correct_password)) {
        /* Read the username. */
        std_fprintf(chout_p, FSTR("username: "));
        shell_read_line(buf_p, chin_p, chout_p, 0);
        correct_username = !strcmp(username_p, buf_p);

        /* Read the  password. */
        std_fprintf(chout_p, FSTR("password: "));
        shell_read_line(buf_p, chin_p, chout_p, 1);
        correct_password = !strcmp(password_p, buf_p);
    }

    /* Write a prompt on successful login. */
    std_fprintf(chout_p, FSTR(SHELL_PROMPT));

    return (0);
}

static int shell_read_command(char *buf_p,
                              chan_t *chin_p,
                              chan_t *chout_p)
{
    char c, *b_p = buf_p, *filter_p, *path_p;
    int err;

    while (b_p < &buf_p[SHELL_COMMAND_MAX]) {
        chan_read(chin_p, &c, sizeof(c));

        if (c == '\t') {
            /* Auto-completion. */
            *b_p = '\0';
            err = fs_auto_complete(buf_p, chout_p);

            if (err > 0) {
                b_p += err;
            } else if (err == 0) {
                fs_split(buf_p, &path_p, &filter_p);
                std_fprintf(chout_p, FSTR("\r\n"));
                fs_list(path_p, filter_p, chout_p);
                fs_merge(path_p, filter_p);
                std_fprintf(chout_p, FSTR(SHELL_PROMPT "%s"), buf_p);
            }
        } else if (c == '\n') {
            /* Comamnd termination. */
            if (((b_p > buf_p) > 0) && (b_p[-1] == '\r')) {
                b_p--;
            }

            chan_write(chout_p, &c, sizeof(c));
            *b_p = '\0';

            return (b_p - buf_p);
        } else if (c == 127) {
            /* Delete character. */
            if (b_p > buf_p) {
                b_p--;
                std_fprintf(chout_p, FSTR("\x08 \x08"));
            }
        } else {
            /* Echo other characters. */
            chan_write(chout_p, &c, sizeof(c));
            *b_p++ = c;
        }
    }

    std_fprintf(chout_p, FSTR("\r\nshell: error: command too long\r\n"));

    return (-E2BIG);
}

void *shell_entry(void *arg_p)
{
    chan_t *chin_p, *chout_p;
    const char *username_p, *password_p;
    struct shell_args_t *sarg_p;
    char buf[SHELL_COMMAND_MAX];
    int authorized = 0;

    thrd_set_name("shell");

    sarg_p = arg_p;
    chin_p = sarg_p->chin_p;
    chout_p = sarg_p->chout_p;
    username_p = sarg_p->username_p;
    password_p = sarg_p->password_p;

    /* Always authorized if no login is required. */
    if (username_p == NULL) {
        authorized = 1;
    }

    while (1) {
        /* Authorization. */
        if (authorized == 0) {
            shell_login(buf, chin_p, chout_p, username_p, password_p);
            authorized = 1;
        }

        /* Read command.*/
        if (shell_read_command(buf, chin_p, chout_p) > 0) {
            /* Logout handling. */
            if (!std_strcmp(FSTR("logout"), buf)
                || !std_strcmp(FSTR("/logout"), buf)) {
                if (username_p != NULL) {
                    authorized = 0;
                }

                continue;
            }

            fs_call(buf, chin_p, chout_p);
        }

        std_fprintf(chout_p, FSTR(SHELL_PROMPT));
    }

    return (NULL);
}
