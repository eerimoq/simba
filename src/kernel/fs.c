/**
 * @file kernel/fs.h
 * @version 0.1.0
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
#include <ctype.h>

#define FS_COMMAND_ARGS_MAX 16
#define FS_COMMAND_MAX 64

#define FS_NAME_MAX 64

FS_COMMAND_DEFINE("/kernel/fs/counters_list", fs_cmd_counters_list);
FS_COMMAND_DEFINE("/kernel/fs/counters_reset", fs_cmd_counters_reset);

FS_COMMAND_DEFINE("/kernel/fs/parameters_list", fs_cmd_parameters_list);

extern const FAR struct fs_node_t fs_nodes[];
extern const FAR int fs_counters[];
extern const FAR int fs_parameters[];

/**
 * Create the absolute path in the file tree for given node.
 *
 * @return pointer to destination string or NULL
 */
static char *get_abspath(char *buf_p,
                         size_t size,
                         const FAR struct fs_node_t *node_p)
{
    const FAR char *name_p;
    size_t length;
    ssize_t left = size;

    /* Add null termination. */
    buf_p += (size - 1);
    *buf_p = '\0';

    /* Stop when the root element is found. */
    while (node_p->parent != -1) {
        name_p = node_p->name_p;
        length = std_strlen(name_p);

        /* Prepend a slash. */
        length += 1;
        left -= length;

        /* The name does not fit. */
        if (left <= 0) {
            return (NULL);
        }

        buf_p -= length;
        *buf_p++ = '/';

        /* Copy to output buffer. */
        while (*name_p != '\0') {
            *buf_p++ = *name_p++;
        }

        buf_p -= length;
        node_p = &fs_nodes[node_p->parent];
    }

    return (buf_p);
}

static int list_indexed_items(chan_t *chout_p,
                              chan_t *chin_p,
                              const FAR int *index_p,
                              const FAR char *path_format)
{
    int i;
    const char *argv_callback[1];
    const FAR struct fs_node_t *node_p;
    char buf[FS_NAME_MAX], *abspath_p;

    while (*index_p != -1) {
        node_p = &fs_nodes[*index_p];

        abspath_p = get_abspath(buf, membersof(buf), node_p);

        std_fprintf(chout_p, path_format, abspath_p);

        for (i = 0; i < FS_NAME_MAX; i++) {
            buf[i] = node_p->name_p[i];
        }

        argv_callback[0] = buf;
        node_p->callback(FS_ARGC_GET, argv_callback, chout_p, chin_p);

        index_p++;
    }

    return (0);
}

int fs_counter_get(int argc,
                   const char *argv[],
                   chan_t *chout_p,
                   chan_t *chin_p,
                   long long *counter_p)
{
    std_fprintf(chout_p,
                FSTR("%012lx%08lx\r\n"),
                (long)(*counter_p >> 32),
                (long)(*counter_p & 0xffffffff));

    return (0);
}

int fs_counter_set(int argc,
                   const char *argv[],
                   chan_t *chout_p,
                   chan_t *chin_p,
                   long long *counter_p)
{
    *counter_p = 0;

    return (0);
}

int fs_parameter_handler_int_set(int argc,
                                 const char *argv[],
                                 chan_t *chout_p,
                                 chan_t *chin_p,
                                 int *parameter_p)
{
    long value;

    if (std_strtol(argv[1], &value)) {
        std_fprintf(chout_p, FSTR("bad value '%s'\r\n"), argv[1]);
        return (1);
    }

    *parameter_p = value;

    return (0);
}

int fs_parameter_handler_int_get(int argc,
                                 const char *argv[],
                                 chan_t *chout_p,
                                 chan_t *chin_p,
                                 int *parameter_p)
{
    std_fprintf(chout_p, FSTR("%d\r\n"), *parameter_p);

    return (0);
}

int fs_cmd_counters_list(int argc,
                         const char *argv[],
                         chan_t *chout_p,
                         chan_t *chin_p)
{
    UNUSED(chin_p);

    std_fprintf(chout_p, FSTR("NAME                                                 VALUE\r\n"));

    return (list_indexed_items(chout_p,
                               chin_p,
                               fs_counters,
                               FSTR("%-52s ")));
}

int fs_cmd_counters_reset(int argc,
                          const char *argv[],
                          chan_t *chout_p,
                          chan_t *chin_p)
{
    UNUSED(chin_p);

    char buf[64];
    int i;
    const char *argv_callback[2];
    const FAR int *index_p;
    const FAR struct fs_node_t *node_p;

    index_p = fs_counters;

    while (*index_p != -1) {
        node_p = &fs_nodes[*index_p];

        for (i = 0; i < FS_NAME_MAX; i++) {
            buf[i] = node_p->name_p[i];
        }

        argv_callback[0] = buf;
        argv_callback[1] = "0";
        node_p->callback(2, argv_callback, chout_p, chin_p);

        index_p++;
    }

    return (0);
}

int fs_cmd_parameters_list(int argc,
                           const char *argv[],
                           chan_t *chout_p,
                           chan_t *chin_p)
{
    UNUSED(chin_p);

    return (list_indexed_items(chout_p,
                               chin_p,
                               fs_parameters,
                               FSTR("%s ")));
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
    while (isspace((int)*read_p)) {
        read_p++;
    }

    /* Merge internal whitespaces.*/
    while (*read_p != '\0') {
        *write_p++ = *read_p++;
        while (isspace((int)*read_p) && isspace((int)read_p[1])) {
            read_p++;
        }
    }

    /* Remove trailing whitespace.*/
    if (isspace((int)write_p[-1])) {
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
        len = std_strlen(fs_nodes[index].name_p);

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

int fs_auto_complete(char *path, chan_t *chout_p)
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
            chan_write(chout_p, &c, sizeof(c));
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
        chan_write(chout_p, &c, sizeof(c));
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
