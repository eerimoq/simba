/**
 * @file kernel/fs.h
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
#include <ctype.h>

#define FS_COMMAND_ARGS_MAX 8
#define FS_COMMAND_MAX 48

#define FS_COUNTER_NAME_MAX 64

FS_COMMAND_DEFINE("/kernel/fs/counter_list", fs_cmd_counter_list);
FS_COMMAND_DEFINE("/kernel/fs/counter_reset", fs_cmd_counter_reset);

extern const FAR struct fs_node_t fs_nodes[];
extern struct fs_counter_t fs_counter___base;

int fs_parameter_handler_int_set(int *parameter_p,
                                 int argc,
                                 const char *argv[],
                                 chan_t *chout_p,
                                 chan_t *chin_p)
{
    long value;

    if (std_strtol(argv[1], &value)) {
        std_fprintf(chout_p, FSTR("bad value '%s'\r\n"), argv[1]);
        return (1);
    }

    *parameter_p = value;

    return (0);
}

int fs_parameter_handler_int_get(int *parameter_p,
                                 int argc,
                                 const char *argv[],
                                 chan_t *chout_p,
                                 chan_t *chin_p)
{
    std_fprintf(chout_p, FSTR("%d\r\n"), *parameter_p);

    return (0);
}

int fs_cmd_counter_list(int argc,
                        const char *argv[],
                        chan_t *chout_p,
                        chan_t *chin_p)
{
    UNUSED(chin_p);

    int i;
    struct fs_counter_t *counter_p;
    char buf[FS_COUNTER_NAME_MAX];

    std_fprintf(chout_p, FSTR("NAME                                             VALUE\r\n"));

    counter_p = fs_counter___base.next_p;

    while (counter_p != NULL) {
        for (i = 0; i < FS_COUNTER_NAME_MAX; i++) {
            buf[i] = counter_p->name_p[i];
        }
        std_fprintf(chout_p,
                    FSTR("%-48s %012lx%08lx\r\n"),
                    buf,
                    (long)(counter_p->value >> 32),
                    (long)(counter_p->value & 0xffffffff));
        counter_p = counter_p->next_p;
    }

    return (0);
}

int fs_cmd_counter_reset(int argc,
                         const char *argv[],
                         chan_t *chout_p,
                         chan_t *chin_p)
{
    UNUSED(chin_p);

    struct fs_counter_t *counter_p;
    const char *filter_p = NULL;

    if (argc > 1) {
        filter_p = argv[1];
    }

    counter_p = fs_counter___base.next_p;

    while (counter_p != NULL) {
        if ((filter_p == NULL) || (std_strcmp(counter_p->name_p, filter_p) == 0)) {
            counter_p->value = 0;
        }

        counter_p = counter_p->next_p;
    }

    return (0);
}

/**
 * Remove whitespaces from beginning and end of string. Replace sequence
 * of whitespaces within the string with a single space.
 */
static void pack(char *string_p)
{
    char *write_p;
    char *read_p;

    write_p = string_p;
    read_p = string_p;

    if (*read_p == '\0') {
        return;
    }

    /* Remove beginning whitespaces.*/
    while (isspace(*read_p)) {
        read_p++;
    }

    /* Merge internal whitespaces.*/
    while (*read_p != '\0') {
        *write_p++ = *read_p++;
        while (isspace(*read_p) && isspace(read_p[1])) {
            read_p++;
        }
    }

    /* Remove trailing whitespace.*/
    if (isspace(write_p[-1])) {
        write_p--;
    }

    *write_p = '\0';
}

static int command_parse(char *command_p, const char *argv[])
{
    int argc;

    pack(command_p);
    argc = 0;
    argv[argc++] = command_p;

    while (*command_p != '\0') {
        if (*command_p == ' ') {
            *command_p = '\0';
            command_p++;

            if (argc == FS_COMMAND_ARGS_MAX) {
                return (-E2BIG);
            }

            argv[argc++] = command_p;
        }
        command_p++;
    }

    return (argc);
}

static int cmp(FAR const char *name_p, const char *cmd_p, int len)
{
    while (len > 0) {
        if (*name_p != *cmd_p) {
            return (1);
        }

        name_p++;
        cmd_p++;
        len--;
    }

    return (0);
}

static int node_find(const char *cmd_p)
{
    int index = 0, len;
    const char *p;

    /* Skip '/'.*/
    while (*cmd_p == '/') {
        cmd_p++;
    }

    /* Find length.*/
    p = cmd_p;
    len = 0;

    while ((*p != '/') && (*p != '\0')) {
        len++;
        p++;
    }

    /* Empty command is root node.*/
    if (len == 0) {
        return (0);
    }

    index = fs_nodes[0].children.begin;

    while (index != 0) {
        /* Node found?*/
        if (cmp(fs_nodes[index].name_p, cmd_p, len) == 0) {
            cmd_p += len;

            if ((*cmd_p == '\0') ||
                ((*cmd_p == '/') && (cmd_p[1] == '\0'))) {
                return (index);
            }

            /* Skip '/'.*/
            while (*cmd_p == '/') {
                cmd_p++;
            }

            /* Find length.*/
            p = cmd_p;
            len = 0;

            while ((*p != '/') && (*p != '\0')) {
                len++;
                p++;
            }

            index = fs_nodes[index].children.begin;

            continue;
        }

        index = fs_nodes[index].next;
    }

    return (-ENOENT);
}

int fs_call(char *command_p,
            chan_t *chin_p,
            chan_t *chout_p)
{
    int err, argc;
    const char *argv[FS_COMMAND_ARGS_MAX];
    FAR const struct fs_node_t *node_p;

    argc = command_parse(command_p, argv);
    if (argc < 0) {
        err = argc;
        goto out;
    }

    err = node_find(argv[0]);

    if (err <= 0) {
        err = -ENOENT;
        goto out;
    }

    node_p = &fs_nodes[err];

    if (node_p->callback == NULL) {
        err = -EISDIR;
        goto out;
    }

    node_p->callback(argc, argv, chout_p, chin_p);

    return (0);

 out:
    std_fprintf(chout_p, FSTR("%s: command not found\r\n"), command_p);

    return (err);
}

int fs_list(const char *path_p,
            const char *filter_p,
            chan_t *chout_p)
{
    int root, head, to_write, written, next, flen = 0;
    const FAR struct fs_node_t *node_p;

    /* Find root node.*/
    root = node_find(path_p);

    if (root < 0) {
        return (-ENOENT);
    }

    /* List in alphabetical order.*/
    head = fs_nodes[root].children.begin;
    written = -1;

    if (filter_p != NULL) {
        flen = strlen(filter_p);
    }

    while (1) {
        to_write = -1;
        next = head;

        while (next != 0) {
            /* Before current to_write node.*/
            if ((to_write == -1) ||
                (std_strcmp_f(fs_nodes[next].name_p, fs_nodes[to_write].name_p) < 0)) {

                /* After last written node.*/
                if ((written == -1) ||
                    (std_strcmp_f(fs_nodes[next].name_p, fs_nodes[written].name_p) > 0)) {
                    to_write = next;
                }
            }
            next = fs_nodes[next].next;
        }

        /* Break when all nodes are written to output chan.*/
        if (to_write == -1) {
            break;
        }

        /* Write node to output chan.*/
        node_p = &fs_nodes[to_write];

        if ((flen == 0) || (cmp(node_p->name_p, filter_p, flen) == 0)) {
            std_fprintf(chout_p, node_p->name_p);

            if (node_p->callback == NULL) {
                std_fprintf(chout_p, FSTR("/"));
            }

            std_fprintf(chout_p, FSTR("\r\n"));
        }

        written = to_write;
    }

    return (0);
}

int fs_auto_complete(char *path, chan_t *chout)
{
    int node, head, pos, flag, next,
        command_length, node_name_length, len;
    char c, *pcmd_p;
    int err;

    /* Split command into path and uncompleted command. */
    fs_split(path, &path, &pcmd_p);
    node = node_find(path);
    fs_merge(path, pcmd_p);

    if (node == -ENOENT) {
        return (-ENOENT);
    }

    head = fs_nodes[node].children.begin;
    node = head;
    command_length = strlen(pcmd_p);
    len = 0;

    /* Any nodes matching path? */
    if (command_length > 0) {
        next = head;

        while (next != 0) {
            if (cmp(fs_nodes[next].name_p, pcmd_p, command_length) == 0) {
                node_name_length = std_strlen(fs_nodes[next].name_p);

                if (node_name_length > len) {
                    len = node_name_length;
                    node = next;
                }
            }

            next = fs_nodes[next].next;
        }

        if (len == 0) {
            return (-ENOENT);
        }
    }

    err = 0;
    pos = command_length;
    node_name_length = std_strlen(fs_nodes[node].name_p);

    /* Auto-complete. */
    while (pos < node_name_length) {
        next = head;
        flag = 0;
        c = fs_nodes[node].name_p[pos];

        while (next != 0) {
            if (std_strncmp_f(fs_nodes[next].name_p, fs_nodes[node].name_p, pos) == 0) {
                if (c != fs_nodes[next].name_p[pos]) {
                    flag = 1;
                    break;
                }
            }

            next = fs_nodes[next].next;
        }

        /* Completion happend? */
        if (flag == 0) {
            err = (pos - command_length + 1);
            pcmd_p[pos] = c;
            chan_write(chout, &c, sizeof(c));
        } else {
            break;
        }

        pos++;
    }

    /* Append '/' or ' ' on full match. */
    if (pos == node_name_length) {
        err++;

        if (fs_nodes[node].callback == NULL) {
            c = '/';
        } else {
            c = ' ';
        }

        pcmd_p[pos++] = c;
        chan_write(chout, &c, sizeof(c));
    }

    pcmd_p[pos] = '\0';

    return (err);
}

void fs_split(char *buf_p, char **path_pp, char **cmd_pp)
{
    *path_pp = buf_p;
    *cmd_pp = NULL;

    while (*buf_p != '\0') {
        if (*buf_p == '/') {
            *cmd_pp = buf_p;
        }

        buf_p++;
    }

    if (*cmd_pp != NULL) {
        **cmd_pp = '\0';
        (*cmd_pp)++;
    } else {
        *cmd_pp = *path_pp;
        *path_pp = "";
    }
}

void fs_merge(char *path_p, char *cmd_p)
{
    if (*path_p != '\0') {
        *--cmd_p = '/';
    }
}
