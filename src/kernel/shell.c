/**
 * @file shell.c
 * @version 0.2.0
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

#define SHELL_COMMAND_MAX 64
#define SHELL_PROMPT "$ "

FS_COMMAND_DEFINE("/logout", shell_cmd_logout);

/**
 * Unused command callback. Logout handling in shell_main().
 */
int shell_cmd_logout(int argc,
                     const char *argv[],
                     chan_t *chout_p,
                     chan_t *chin_p)
{
    return (-1);
}

static int read_line(char *buf_p,
                     chan_t *chin_p,
                     chan_t *chout_p,
                     int sensitive)
{
    char c, *write_p = buf_p;
    int newline_found = 0;

    while (newline_found == 0) {
        chan_read(chin_p, &c, sizeof(c));

        if (c == '\n') {
            newline_found = 1;
        } else if (c == '\r') {
        } else if (c == 127) {
            /* Delete character. */
            if (write_p > buf_p) {
                write_p--;
                std_fprintf(chout_p, FSTR("\x08 \x08"));
            }
            continue;
        } else {
            *write_p++ = c;

            if (sensitive != 0) {
                c = '*';
            }
        }

        chan_write(chout_p, &c, sizeof(c));
    }

    /* string null termination.*/
    *write_p = '\0';

    return (0);
}

static int login(char *buf_p,
                 chan_t *chin_p,
                 chan_t *chout_p,
                 const char *username_p,
                 const char *password_p)
{
    int correct_username;
    int correct_password;

    while (1) {
        correct_username = 0;
        correct_password = 0;

        /* Read the username. */
        std_fprintf(chout_p, FSTR("username: "));
        read_line(buf_p, chin_p, chout_p, 0);

        /* Write 'username: ' on empty string. */
        if (*buf_p == '\0') {
            continue;
        }

        correct_username = !strcmp(username_p, buf_p);

        /* Read the  password. */
        std_fprintf(chout_p, FSTR("password: "));
        read_line(buf_p, chin_p, chout_p, 1);
        correct_password = !strcmp(password_p, buf_p);

        if (correct_username && correct_password) {
            break;
        } else {
            std_fprintf(chout_p, FSTR("authentication failure\r\n"));
        }
    }

    /* Write a prompt on successful login. */
    std_fprintf(chout_p, FSTR(SHELL_PROMPT));

    return (0);
}

static int read_command(char *buf_p,
                        chan_t *chin_p,
                        chan_t *chout_p)
{
    char c, *write_p = buf_p, *filter_p, *path_p;
    int err;

    while (write_p < &buf_p[SHELL_COMMAND_MAX]) {
        chan_read(chin_p, &c, sizeof(c));

        if (c == '\t') {
            /* Auto-completion. */
            *write_p = '\0';
            err = fs_auto_complete(buf_p, chout_p);

            if (err > 0) {
                write_p += err;
            } else if (err == 0) {
                fs_split(buf_p, &path_p, &filter_p);
                std_fprintf(chout_p, FSTR("\r\n"));
                fs_list(path_p, filter_p, chout_p);
                fs_merge(path_p, filter_p);
                std_fprintf(chout_p, FSTR(SHELL_PROMPT "%s"), buf_p);
            }
        } else if (c == '\n') {
            /* Comamnd termination. */
            if (((write_p > buf_p) > 0) && (write_p[-1] == '\r')) {
                write_p--;
            }

            chan_write(chout_p, &c, sizeof(c));
            *write_p = '\0';

            return (write_p - buf_p);
        } else if (c == 127) {
            /* Delete character. */
            if (write_p > buf_p) {
                write_p--;
                std_fprintf(chout_p, FSTR("\x08 \x08"));
            }
        } else {
            /* Echo other characters. */
            chan_write(chout_p, &c, sizeof(c));
            *write_p++ = c;
        }
    }

    std_fprintf(chout_p, FSTR("\r\nshell: error: command too long\r\n"));

    return (-E2BIG);
}

void *shell_main(void *arg_p)
{
    chan_t *chin_p, *chout_p;
    const char *username_p, *password_p;
    struct shell_args_t *shell_args_p;
    char buf[SHELL_COMMAND_MAX];
    int authorized = 0;

    shell_args_p = arg_p;

    if (shell_args_p->name_p == NULL) {
        shell_args_p->name_p = "shell";
    }

    thrd_set_name(shell_args_p->name_p);

    chin_p = shell_args_p->chin_p;
    chout_p = shell_args_p->chout_p;
    username_p = shell_args_p->username_p;
    password_p = shell_args_p->password_p;

    /* Always authorized if no login is required. */
    if (username_p == NULL) {
        authorized = 1;
    }

    while (1) {
        /* Authorization. */
        if (authorized == 0) {
            login(buf, chin_p, chout_p, username_p, password_p);
            authorized = 1;
        }

        /* Read command.*/
        if (read_command(buf, chin_p, chout_p) > 0) {
            /* Logout handling. */
            if (!std_strcmp(std_strip(buf, NULL), FSTR("logout"))) {
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
