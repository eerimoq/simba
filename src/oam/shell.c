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

#define TAB                 '\t'
#define CARRIAGE_RETURN     '\r'
#define NEWLINE             '\n'
#define BACKSPACE            127
#define CTRL_A                 1
#define CTRL_E                 5
#define CTRL_D                 4
#define CTRL_K                11
#define CTRL_T                20
#define CTRL_R                18
#define CTRL_G                 7
#define ALT                   27

struct module_t {
    int initialized;
};

static struct module_t module;

static struct fs_command_t cmd_logout;

#if CONFIG_SHELL_MINIMAL == 0

static struct fs_command_t cmd_help;
static struct fs_command_t cmd_history;

static int history_init(struct shell_t *self_p);

#endif

static int shell_command_compare(const char *line_p,
                                 FAR const char *command_p,
                                 size_t length)
{
    return ((std_strncmp(command_p, line_p, length) == 0)
            && ((line_p[length] == '\0')
                || (line_p[length] == ' ')));
}

/**
 * Check if given line is a comment.
 *
 * @return true(1) if given line is a comment, otherwise false(0).
 */
static int is_comment(const char *line_p)
{
    return (*line_p == '#');
}

/**
 * Check if given line is a shell command.
 *
 * @return true(1) if given line is a shell command, otherwise
 *         false(0).
 */
static int is_shell_command(const char *line_p)
{
    if (*line_p == '/') {
        line_p++;
    }

    return (shell_command_compare(line_p, FSTR("logout"), 6)
            || shell_command_compare(line_p, FSTR("history"), 7)
            || shell_command_compare(line_p, FSTR("help"), 4));
}

/**
 * Unused command callback. Logout handling in shell_main().
 */
static int cmd_logout_cb(int argc,
                         const char *argv[],
                         void *chout_p,
                         void *chin_p,
                         void *arg_p,
                         void *call_arg_p)
{
    struct shell_t *self_p = call_arg_p;

    if (self_p->username_p != NULL) {
        self_p->authorized = 0;
    }

    return (1);
}

static int line_init(struct shell_line_t *self_p)
{
    self_p->buf[0] = '\0';
    self_p->cursor = 0;
    self_p->length = 0;

    return (0);
}

static int line_insert(struct shell_line_t *self_p,
                       char c)
{
    /* Buffer full? */
    if (self_p->length == CONFIG_SHELL_COMMAND_MAX - 1) {
        return (-1);
    }

    /* Move the string, including the NULL termination, one step to
       the right and insert the new character. */
    memmove(&self_p->buf[self_p->cursor + 1],
            &self_p->buf[self_p->cursor],
            self_p->length - self_p->cursor + 1);
    self_p->buf[self_p->cursor++] = c;
    self_p->length++;

    return (0);
}

static int line_insert_string(struct shell_line_t *self_p,
                              char *str_p)
{
    while (*str_p != '\0') {
        if (line_insert(self_p, *str_p) != 0) {
            return (-1);
        }

        str_p++;
    }

    return (0);
}

static int line_delete(struct shell_line_t *self_p)
{
    /* End of buffer? */
    if (self_p->cursor == self_p->length) {
        return (-1);
    }

    /* Move the string, including the NULL termination, one step to
       the left to overwrite the deleted character. */
    memmove(&self_p->buf[self_p->cursor],
            &self_p->buf[self_p->cursor + 1],
            self_p->length - self_p->cursor);
    self_p->length--;

    return (0);
}

#if CONFIG_SHELL_MINIMAL == 0

static char line_peek(struct shell_line_t *self_p)
{
    return (self_p->buf[self_p->cursor]);
}

static int line_truncate(struct shell_line_t *self_p)
{
    self_p->length = self_p->cursor;
    self_p->buf[self_p->length] = '\0';

    return (0);
}

#endif

static int line_is_empty(struct shell_line_t *self_p)
{
    return (self_p->length == 0);
}

static char *line_get_buf(struct shell_line_t *self_p)
{
    return (self_p->buf);
}

static int line_get_length(struct shell_line_t *self_p)
{
    return (self_p->length);
}

static int line_seek(struct shell_line_t *self_p, int pos)
{
    if (pos < 0) {
        if ((self_p->cursor + pos) < 0) {
            return (-1);
        }
    } else {
        if ((self_p->cursor + pos) > self_p->length) {
            return (-1);
        }
    }

    self_p->cursor += pos;

    return (0);
}

static int line_get_cursor(struct shell_line_t *self_p)
{
    return (self_p->cursor);
}

#if CONFIG_SHELL_MINIMAL == 0

static int line_seek_begin(struct shell_line_t *self_p)
{
    self_p->cursor = 0;

    return (0);
}

static int line_seek_end(struct shell_line_t *self_p)
{
    self_p->cursor = self_p->length;

    return (0);
}

static int cmd_help_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct shell_t *self_p = call_arg_p;
    const char title_color_on[] = COLOR(COLOR_BOLD_ON);
    const char title_color_off[] = COLOR(COLOR_RESET);

    std_fprintf(chout_p,
                FSTR("%sCursor movement%s\r\n"
                     "\r\n"
                     "         LEFT   Go left one character.\r\n"
                     "        RIGHT   Go right on character.\r\n"
                     "  HOME/Ctrl+A   Go to the beginning of the line.\r\n"
                     "   END/Ctrl+E   Go to the end of the line.\r\n"
                     "\r\n"
                     "%sEdit%s\r\n"
                     "\r\n"
                     "        Alt+D   Delete the word at the cursor.\r\n"
                     "       Ctrl+D   Delete the chracter at the cursor.\r\n"
                     "       Ctrl+K   Cut the line from cursor to end.\r\n"
                     "       Ctrl+T   Swap the last two characters before the cursor (typo).\r\n"
                     "          TAB   Tab completion for file/directory names.\r\n"
                     "    BACKSPACE   Delete the character before the cursor.\r\n"
                     "\r\n"
                     "%sHistory%s\r\n"
                     "\r\n"
                     "           UP   Previous command.\r\n"
                     "         DOWN   Next command.\r\n"
                     "       Ctrl+R   Recall the last command including the specified character(s)\r\n"
                     "                searches the command history as you type.\r\n"
                     "       Ctrl+G   Escape from history searching mode.\r\n"),
                title_color_on, title_color_off,
                title_color_on, title_color_off,
                title_color_on, title_color_off);

    std_fprintf(self_p->chout_p, FSTR(CONFIG_SHELL_PROMPT));

    return (0);
}

static int cmd_history_cb(int argc,
                          const char *argv[],
                          void *chout_p,
                          void *chin_p,
                          void *arg_p,
                          void *call_arg_p)
{
    struct shell_history_elem_t *current_p;
    struct shell_t *self_p = call_arg_p;
    int i;

    if (argc == 1) {
        current_p = self_p->history.head_p;
        i = 1;

        while (current_p != NULL) {
            std_fprintf(chout_p, FSTR("%d: %s\r\n"), i, current_p->buf);
            current_p = current_p->next_p;
            i++;
        }
    } else if (argc == 2) {
        if (strcmp(argv[1], "-c") == 0) {
            history_init(self_p);
        }
    }

    std_fprintf(self_p->chout_p, FSTR(CONFIG_SHELL_PROMPT));

    return (0);
}

static int history_init(struct shell_t *self_p)
{
    self_p->history.head_p = NULL;
    self_p->history.tail_p = NULL;
    self_p->history.current_p = NULL;
    line_init(&self_p->history.line);
    self_p->history.line_valid = 0;

    circular_heap_init(&self_p->history.heap.heap,
                       self_p->history.heap.buf,
                       sizeof(self_p->history.heap.buf));

    return (0);
}

static int history_append(struct shell_t *self_p,
                          char *command_p)
{
    struct shell_history_elem_t *elem_p, *head_p;
    size_t command_size;

    /* Do not append if the command already is at the end of the
       list. */
    if (self_p->history.tail_p != NULL) {
        if (strcmp(self_p->history.tail_p->buf, command_p) == 0) {
            return (0);
        }
    }

    elem_p = NULL;
    command_size = strlen(command_p) + 1;

    while (elem_p == NULL) {
        /* Allocate memory. */
        elem_p = circular_heap_alloc(&self_p->history.heap.heap,
                                     sizeof(*elem_p) + command_size);

        /* Free the oldest command if there is no memory available. */
        if (elem_p == NULL) {
            head_p = self_p->history.head_p;

            /* Any element to free? */
            if (head_p == NULL) {
                return (-1);
            }

            /* Remove the head element from the list. */
            if (head_p == self_p->history.tail_p) {
                self_p->history.head_p = NULL;
                self_p->history.tail_p = NULL;
            } else {
                self_p->history.head_p = head_p->next_p;
                head_p->next_p->prev_p = NULL;
            }

            (void)circular_heap_free(&self_p->history.heap.heap, head_p);
        }
    }

    if (elem_p != NULL) {
        strcpy(elem_p->buf, command_p);

        /* Append the command to the command history list. */
        elem_p->next_p = NULL;

        if (self_p->history.head_p == NULL) {
            elem_p->prev_p = NULL;
            self_p->history.head_p = elem_p;
        } else {
            elem_p->prev_p = self_p->history.tail_p;
            elem_p->prev_p->next_p = elem_p;
        }

        self_p->history.tail_p = elem_p;
    }

    return (0);
}

/**
 * Find the previous element, if any.
 */
static char *history_get_previous_command(struct shell_t *self_p)
{
    if (self_p->history.current_p == self_p->history.head_p) {
        return (NULL);
    } else if (self_p->history.current_p == NULL) {
        self_p->history.current_p = self_p->history.tail_p;

        /* Save the current command to be able to restore it when DOWN
           is pressed. */
        self_p->history.line = self_p->line;
        self_p->history.line_valid = 1;
    } else if (self_p->history.current_p != self_p->history.head_p) {
        self_p->history.current_p = self_p->history.current_p->prev_p;
    }

    if (self_p->history.current_p != NULL) {
        return (self_p->history.current_p->buf);
    } else {
        return (NULL);
    }
}

/**
 * Find the next element, if any.
 */
static char *history_get_next_command(struct shell_t *self_p)
{
    if (self_p->history.current_p != NULL) {
        self_p->history.current_p = self_p->history.current_p->next_p;
    }

    if (self_p->history.current_p != NULL) {
        return (self_p->history.current_p->buf);
    } else if (self_p->history.line_valid == 1) {
        self_p->history.line_valid = 0;

        return (line_get_buf(&self_p->history.line));
    } else {
        return (NULL);
    }
}

static int history_reset_current(struct shell_t *self_p)
{
    self_p->history.current_p = NULL;

    return (0);
}

static char *history_reverse_search(struct shell_t *self_p,
                                    const char *pattern_p)
{
    struct shell_history_elem_t *elem_p;

    elem_p = self_p->history.tail_p;

    while (elem_p != NULL) {
        if (strstr(elem_p->buf, pattern_p) != NULL) {
            return (elem_p->buf);
        }

        elem_p = elem_p->prev_p;
    }

    return (NULL);
}

#endif

static int read_line(struct shell_t *self_p,
                     int sensitive)
{
    char c;

    line_init(&self_p->line);
    self_p->newline_received = 0;

    while (self_p->newline_received == 0) {
        if (chan_read(self_p->chin_p, &c, sizeof(c)) != sizeof(c)) {
            return (-EIO);
        }

        switch (c) {

        case NEWLINE:
            self_p->newline_received = 1;
            break;

        case CARRIAGE_RETURN:
            break;

        case BACKSPACE:
            if (line_seek(&self_p->line, -1) == 0) {
                line_delete(&self_p->line);
                std_fprintf(self_p->chout_p, FSTR("\x08 \x08"));
            }
            continue;

        default:
            line_insert(&self_p->line, c);

            if (sensitive != 0) {
                c = '*';
            }
            break;
        }

        chan_write(self_p->chout_p, &c, sizeof(c));
    }

    return (0);
}

/**
 * Let the user write its username and password and compare them to
 * the allowed shell credentials.
 */
static int login(struct shell_t *self_p)
{
    int res;
    int correct_username;
    int correct_password;

    while (1) {
        correct_username = 0;
        correct_password = 0;

        /* Read the username. */
        std_fprintf(self_p->chout_p, FSTR("username: "));
        res = read_line(self_p, 0);

        if (res != 0) {
            return (res);
        }

        /* Write 'username: ' on empty string. */
        if (line_is_empty(&self_p->line)) {
            continue;
        }

        correct_username = !strcmp(self_p->username_p,
                                   line_get_buf(&self_p->line));

        /* Read the password. */
        std_fprintf(self_p->chout_p, FSTR("password: "));
        res = read_line(self_p, 1);

        if (res != 0) {
            return (-EIO);
        }

        correct_password = !strcmp(self_p->password_p,
                                   line_get_buf(&self_p->line));

        if (correct_username && correct_password) {
            break;
        } else {
            std_fprintf(self_p->chout_p,
                        FSTR("authentication failure\r\n"));
        }
    }

    /* Write a prompt on successful login. */
    std_fprintf(self_p->chout_p, FSTR(CONFIG_SHELL_PROMPT));

    return (0);
}

static int handle_tab(struct shell_t *self_p)
{
    int err;
    char *path_p, *filter_p, *new_p;
    char buf[CONFIG_SHELL_COMMAND_MAX];

    strcpy(buf, line_get_buf(&self_p->line));

    /* Auto-completion. */
    err = fs_auto_complete(buf);

    if (err > 0) {
        new_p = &buf[line_get_cursor(&self_p->line)];
        line_insert_string(&self_p->line, new_p);
    } else if (err == 0) {
        /* No auto-completion happend. */
        fs_split(buf, &path_p, &filter_p);
        std_fprintf(self_p->chout_p, FSTR("\r\n"));
        (void)fs_list(path_p, filter_p, self_p->chout_p);
        std_fprintf(self_p->chout_p,
                    FSTR(CONFIG_SHELL_PROMPT "%s"),
                    line_get_buf(&self_p->line));
    }

    return (0);
}

static int handle_carrige_return(struct shell_t *self_p)
{
    self_p->carriage_return_received = 1;

    return (0);
}

static int handle_newline(struct shell_t *self_p)
{
    self_p->newline_received = 1;

    return (0);
}

/**
 * BACKSPACE Delete the character before the cursor.
 */
static int handle_backspace(struct shell_t *self_p)
{
    if (line_seek(&self_p->line, -1) == -1) {
        return (0);
    }

    line_delete(&self_p->line);

    return (0);
}

#if CONFIG_SHELL_MINIMAL == 0

/**
 * Ctrl+A Go to the beginning of the line.
 */
static int handle_ctrl_a(struct shell_t *self_p)
{
    line_seek_begin(&self_p->line);

    return (0);
}

/**
 * Ctrl+E Go to the end of the line.
 */
static int handle_ctrl_e(struct shell_t *self_p)
{
    line_seek_end(&self_p->line);

    return (0);
}

/**
 * Ctrl+D Delete the chracter at the cursor.
 */
static int handle_ctrl_d(struct shell_t *self_p)
{
    line_delete(&self_p->line);

    return (0);
}

/**
 * Ctrl+K   Cut the line from cursor to end.
 */
static int handle_ctrl_k(struct shell_t *self_p)
{
    line_truncate(&self_p->line);

    return (0);
}

/**
 * Ctrl+T Swap the last two characters before the cursor (typo).
 */
static int handle_ctrl_t(struct shell_t *self_p)
{
    char c;
    int cursor;

    /* Is a swap possible? */
    cursor = line_get_cursor(&self_p->line);

    /* Cannot swap if the cursor is at the beginning of the line. */
    if (cursor == 0) {
        return (-1);
    }

    /* Cannot swap if there are less than two characters. */
    if (line_get_length(&self_p->line) < 2) {
        return (-1);
    }

    /* Move the cursor to the second character. */
    if (cursor == line_get_length(&self_p->line)) {
        line_seek(&self_p->line, -1);
    }

    /* Swap the two characters. */
    c = line_peek(&self_p->line);
    line_delete(&self_p->line);
    line_seek(&self_p->line, -1);
    line_insert(&self_p->line, c);
    line_seek(&self_p->line, 1);

    return (0);
}

static int restore_previous_line(struct shell_t *self_p,
                                 struct shell_line_t *pattern_p)
{
    int cursor;
    int length;

    std_fprintf(self_p->chout_p, FSTR("\x1b[%dD"
                                      "\x1b[K"
                                      "%s"),
                17 + line_get_length(pattern_p),
                line_get_buf(&self_p->prev_line));

    cursor = line_get_cursor(&self_p->prev_line);
    length = line_get_length(&self_p->prev_line);

    if (cursor != length) {
        std_fprintf(self_p->chout_p,
                    FSTR("\x1b[%dD"),
                    length - cursor);
    }

    return (0);
}

/**
 * Ctrl+R Recall the last command including the specified character(s)
 * searches the command history as you type.
 *
 * The original line buffer is printed and cursor reset, then the
 * selected command is copied into the line buffer. The output of the
 * new command occurs in the main command loop.
 */
static int handle_ctrl_r(struct shell_t *self_p)
{
    char c, *buf_p;

    line_init(&self_p->history.pattern);
    line_init(&self_p->history.match);

    if (!line_is_empty(&self_p->line)) {
        std_fprintf(self_p->chout_p,
                    FSTR("\x1b[%dD"),
                    line_get_length(&self_p->line));
    }

    std_fprintf(self_p->chout_p,
                FSTR("\x1b[K"
                     "(history-search)`': "
                     "\x1b[3D"));

    while (1) {
        if (chan_read(self_p->chin_p, &c, sizeof(c)) != sizeof(c)) {
            return (-EIO);
        }

        switch (c) {

        case BACKSPACE:
            if (!line_is_empty(&self_p->history.pattern)) {
                std_fprintf(self_p->chout_p, FSTR("\x1b[1D"
                                                  "\x1b[K"
                                                  "': "));
                line_seek(&self_p->history.pattern, -1);
                line_delete(&self_p->history.pattern);
                buf_p = history_reverse_search(self_p,
                                               line_get_buf(&self_p->history.pattern));
                line_init(&self_p->history.match);

                if (buf_p != NULL) {
                    line_insert_string(&self_p->history.match, buf_p);
                }

                std_fprintf(self_p->chout_p,
                            FSTR("%s"
                                 "\x1b[%dD"),
                            line_get_buf(&self_p->history.match),
                            line_get_length(&self_p->history.match) + 3);
            }
            break;

        case CARRIAGE_RETURN:
            self_p->carriage_return_received = 1;
            break;

        case CTRL_G:
            restore_previous_line(self_p, &self_p->history.pattern);
            return (0);

        default:
            if (isprint((int)c)) {
                if (line_insert(&self_p->history.pattern, c) == 0) {
                    std_fprintf(self_p->chout_p, FSTR("\x1b[K"
                                                      "%c"
                                                      "': "),
                                c);
                    buf_p = history_reverse_search(self_p,
                                                   line_get_buf(&self_p->history.pattern));
                    line_init(&self_p->history.match);

                    if (buf_p != NULL) {
                        line_insert_string(&self_p->history.match, buf_p);
                    }

                    std_fprintf(self_p->chout_p,
                                FSTR("%s"
                                     "\x1b[%dD"),
                                line_get_buf(&self_p->history.match),
                                line_get_length(&self_p->history.match) + 3);
                }
            } else {
                restore_previous_line(self_p, &self_p->history.pattern);

                /* Copy the match to current line. */
                self_p->line = self_p->history.match;

                if (c == NEWLINE) {
                    self_p->newline_received = 1;
                } else {
                    if (c == ALT) {
                        if (chan_read(self_p->chin_p,
                                      &c,
                                      sizeof(c)) != sizeof(c)) {
                            return (-EIO);
                        }

                        if (c != 'd') {
                            if (chan_read(self_p->chin_p,
                                          &c,
                                          sizeof(c)) != sizeof(c)) {
                                return (-EIO);
                            }
                        }
                    }
                }

                return (0);
            }
        }
    }

    return (0);
}

/**
 * ALT.
 */
static int handle_alt(struct shell_t *self_p)
{
    char c, *buf_p;

    if (chan_read(self_p->chin_p, &c, sizeof(c)) != sizeof(c)) {
        return (-EIO);
    }

    switch(c) {

    case 'd':
        /* Alt+D Delete the word at the cursor. */
        while (isblank((int)line_peek(&self_p->line))) {
            line_delete(&self_p->line);
        }

        while (!isblank((int)line_peek(&self_p->line))
               && (line_peek(&self_p->line) != '\0')) {
            line_delete(&self_p->line);
        }
        break;

    case 'O':
        if (chan_read(self_p->chin_p,
                      &c,
                      sizeof(c)) != sizeof(c)) {
            return (-EIO);
        }

        switch (c) {

        case 'H':
            /* HOME. */
            line_seek_begin(&self_p->line);
            break;

        case 'F':
            /* END. */
            line_seek_end(&self_p->line);
            break;

        default:
            break;
        }
        break;

    case '[':
        if (chan_read(self_p->chin_p,
                      &c,
                      sizeof(c)) != sizeof(c)) {
            return (-EIO);
        }

        switch (c) {

        case 'A':
        case 'B':
            if (c == 'A') {
                /* UP Previous command. */
                buf_p = history_get_previous_command(self_p);
            } else {
                /* DOWN Next command. */
                buf_p = history_get_next_command(self_p);
            }

            if (buf_p != NULL) {
                line_init(&self_p->line);
                line_insert_string(&self_p->line, buf_p);
            }
            break;

        case 'C':
            /* RIGHT Go right on character. */
            line_seek(&self_p->line, 1);
            break;

        case 'D':
            /* LEFT Go left one character. */
            line_seek(&self_p->line, -1);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return (0);
}

#endif

static int handle_other(struct shell_t *self_p,
                        char c)
{
    line_insert(&self_p->line, c);

    return (0);
}

/**
 * Show updated line to the user and update the cursor to its new
 * position.
 */
static int show_line(struct shell_t *self_p)
{
    int i;
    int cursor, new_cursor;
    int length, new_length, min_length;

    cursor = line_get_cursor(&self_p->prev_line);
    length = line_get_length(&self_p->prev_line);
    new_length = line_get_length(&self_p->line);

#if CONFIG_SHELL_MINIMAL == 0

    new_cursor = line_get_cursor(&self_p->line);
    min_length = MIN(line_get_length(&self_p->prev_line), new_length);

    /* Was the line edited? */
    if (strcmp(line_get_buf(&self_p->line),
               line_get_buf(&self_p->prev_line)) != 0) {
        /* Only output the change if the last part of the string
           shall be deleted. */
        if ((strncmp(line_get_buf(&self_p->line),
                     line_get_buf(&self_p->prev_line),
                     min_length) == 0)
            && (new_cursor == new_length)) {

#else
            UNUSED(new_cursor);
            UNUSED(min_length);
#endif

            if (length < new_length) {
                /* New character. */
                std_fprintf(self_p->chout_p,
                            FSTR("%s"),
                            &line_get_buf(&self_p->line)[cursor]);
            } else {
                /* Move the cursor to the end of the old line. */
                for (i = cursor; i < length; i++) {
                    std_fprintf(self_p->chout_p, FSTR(" "));
                }

                /* Backspace. */
                for (i = new_length; i < length; i++) {
                    std_fprintf(self_p->chout_p, FSTR("\x08 \x08"));
                }
            }

#if CONFIG_SHELL_MINIMAL == 0

        } else {
            if (cursor > 0) {
                std_fprintf(self_p->chout_p, FSTR("\x1b[%dD"), cursor);
            }

            std_fprintf(self_p->chout_p,
                        FSTR("\x1b[K"
                             "%s"),
                        line_get_buf(&self_p->line));

            if (new_cursor < new_length) {
                std_fprintf(self_p->chout_p,
                            FSTR("\x1b[%dD"),
                            new_length - new_cursor);
            }
        }
    } else if (cursor < new_cursor) {
        std_fprintf(self_p->chout_p,
                    FSTR("\x1b[%dC"),
                    new_cursor - cursor);
    } else if (new_cursor < cursor) {
        std_fprintf(self_p->chout_p,
                    FSTR("\x1b[%dD"),
                    cursor - new_cursor);
    }

#endif

    return (0);
}

/**
 * Execute the current line.
 */
static int execute_line(struct shell_t *self_p)
{
    if (self_p->carriage_return_received == 1) {
        std_fprintf(self_p->chout_p, FSTR("\r"));
    }

    std_fprintf(self_p->chout_p, FSTR("\n"));

#if CONFIG_SHELL_MINIMAL == 0

    /* Append the command to the history. */
    if (!line_is_empty(&self_p->line)) {
        history_append(self_p, line_get_buf(&self_p->line));
    }

    history_reset_current(self_p);

#endif

    return (line_get_length(&self_p->line));
}

/**
 * Read the next command.
 *
 * @return Command length or negative error code.
 */
static int read_command(struct shell_t *self_p)
{
    char c;

    /* Initialize the read command state. */
    line_init(&self_p->line);
    self_p->carriage_return_received = 0;
    self_p->newline_received = 0;

    while (1) {
        if (chan_read(self_p->chin_p, &c, sizeof(c)) != sizeof(c)) {
            return (-EIO);
        }

        /* Save current line. */
        self_p->prev_line = self_p->line;

        switch (c) {

        case TAB:
            handle_tab(self_p);
            break;

        case CARRIAGE_RETURN:
            handle_carrige_return(self_p);
            break;

        case NEWLINE:
            handle_newline(self_p);
            break;

        case BACKSPACE:
            handle_backspace(self_p);
            break;

#if CONFIG_SHELL_MINIMAL == 0

        case CTRL_A:
            handle_ctrl_a(self_p);
            break;

        case CTRL_E:
            handle_ctrl_e(self_p);
            break;

        case CTRL_D:
            handle_ctrl_d(self_p);
            break;

        case CTRL_K:
            handle_ctrl_k(self_p);
            break;

        case CTRL_T:
            handle_ctrl_t(self_p);
            break;

        case CTRL_R:
            handle_ctrl_r(self_p);
            break;

        case ALT:
            handle_alt(self_p);
            break;

#endif

        default:
            handle_other(self_p, c);
            break;
        }

        /* Show the new line to the user and execute it if enter was
           pressed. */
        show_line(self_p);

        if (self_p->newline_received == 1) {
            return (execute_line(self_p));
        }
    }

    return (-E2BIG);
}

int shell_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    fs_command_init(&cmd_logout,
                    FSTR("/logout"),
                    cmd_logout_cb,
                    NULL);
    fs_command_register(&cmd_logout);

#if CONFIG_SHELL_MINIMAL == 0

    fs_command_init(&cmd_history,
                    FSTR("/history"),
                    cmd_history_cb,
                    NULL);
    fs_command_register(&cmd_history);

    fs_command_init(&cmd_help,
                    FSTR("/help"),
                    cmd_help_cb,
                    NULL);
    fs_command_register(&cmd_help);

#endif

    return (0);
}

int shell_init(struct shell_t *self_p,
               void *chin_p,
               void *chout_p,
               void *arg_p,
               const char *name_p,
               const char *username_p,
               const char *password_p)
{
    if (name_p == NULL) {
        name_p = "shell";
    }

    self_p->chin_p = chin_p;
    self_p->chout_p = chout_p;
    self_p->arg_p = arg_p;
    self_p->name_p = name_p;
    self_p->username_p = username_p;
    self_p->password_p = password_p;

    /* Always authorized if no login is required. */
    if (username_p == NULL) {
        self_p->authorized = 1;
    } else {
        self_p->authorized = 0;
    }

#if CONFIG_SHELL_MINIMAL == 0
    /* Initialize the history. */
    history_init(self_p);
#endif

    return (0);
}

void *shell_main(void *arg_p)
{
    ASSERTNRN(arg_p != NULL, EINVAL);

    struct shell_t *self_p;
    int res;
    char *stripped_line_p;

    self_p = arg_p;

    thrd_set_name(self_p->name_p);

    while (1) {
        /* Authorization. */
        if (self_p->authorized == 0) {
            res = login(self_p);

            if (res != 0) {
                break;
            }

            self_p->authorized = 1;
        }

        /* Read command.*/
        res = read_command(self_p);

        if (res > 0) {
            stripped_line_p = std_strip(line_get_buf(&self_p->line),
                                        NULL);

            if (is_comment(stripped_line_p) == 1) {
                /* Just print a prompt. */
            } else if (is_shell_command(stripped_line_p) == 1) {
                (void)fs_call(stripped_line_p,
                              self_p->chin_p,
                              self_p->chout_p,
                              self_p);
                continue;
            } else {
                res = fs_call(stripped_line_p,
                              self_p->chin_p,
                              self_p->chout_p,
                              self_p->arg_p);

                if (res == 0) {
                    std_fprintf(self_p->chout_p, OSTR("OK\r\n"));
                } else {
                    std_fprintf(self_p->chout_p, OSTR("ERROR(%d)\r\n"), res);
                }
            }
        } else if (res == -EIO) {
            break;
        }

        std_fprintf(self_p->chout_p, FSTR(CONFIG_SHELL_PROMPT));
    }

    return (NULL);
}
