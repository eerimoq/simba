/**
 * @file main.c
 * @version 6.0.0
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

static struct binary_tree_t foo;
static struct binary_tree_node_t nodes[16];
static struct binary_tree_node_t duplicate;

int test_init(struct harness_t *harness_p)
{
    BTASSERT(binary_tree_init(&foo) == 0);

    return (0);
}

int test_insert(struct harness_t *harness_p)
{
    int i;

    /* Insert a few nodes. */
    for (i = 0; i < membersof(nodes) / 2; i++) {
        nodes[i].key = i;
        std_printf(FSTR("Inserting node with key %d\r\n"), nodes[i].key);
        BTASSERT(binary_tree_insert(&foo, &nodes[i]) == 0);
    }

    /* Insert more nodes. */
    for (; i < membersof(nodes); i++) {
        nodes[i].key = (2 * membersof(nodes) - i);
        std_printf(FSTR("Inserting node with key %d\r\n"), nodes[i].key);
        BTASSERT(binary_tree_insert(&foo, &nodes[i]) == 0);
    }

    /* Insert a duplicate. */
    duplicate.key = 4;
    std_printf(FSTR("Inserting node with key %d\r\n"), duplicate.key);
    BTASSERT(binary_tree_insert(&foo, &duplicate) == -1);

    /* Print the binary tree. */
    binary_tree_print(&foo);

    return (0);
}

int test_search(struct harness_t *harness_p)
{
    int i;
    int key;

    /* Search for a non-existing node. */
    BTASSERT(binary_tree_search(&foo, 30) == NULL);

    /* Search for a few nodes. */
    for (i = 0; i < membersof(nodes) / 2; i++) {
        key = i;
        BTASSERT(binary_tree_search(&foo, key) == &nodes[i]);
    }

    /* Search for more nodes. */
    for (; i < membersof(nodes); i++) {
        key = (2 * membersof(nodes) - i);
        BTASSERT(binary_tree_search(&foo, key) == &nodes[i]);
    }
    
    return (0);
}

int test_delete(struct harness_t *harness_p)
{
    int i;
    int key;

    /* Delete a few nodes. */
    for (i = 0; i < membersof(nodes) / 2; i++) {
        key = i;
        std_printf(FSTR("Deleting node with key %d\r\n"), key);
        BTASSERT(binary_tree_delete(&foo, key) == 0);
    }

    /* Print the binary tree. */
    binary_tree_print(&foo);

    /* Delete a non-existing node. */
    BTASSERT(binary_tree_delete(&foo, 0) == -1);

    /* Delete the rest of the nodes. */
    for (; i < membersof(nodes); i++) {
        key = (2 * membersof(nodes) - i);
        std_printf(FSTR("Deleting node with key %d\r\n"), key);
        BTASSERT(binary_tree_delete(&foo, key) == 0);
    }

    /* Print the binary tree. */
    binary_tree_print(&foo);

    return (0);
}

int test_search_empty(struct harness_t *harness_p)
{
    /* Search for a non-existing node in the empty tree. */
    BTASSERT(binary_tree_search(&foo, 30) == NULL);
    
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_insert, "test_insert" },
        { test_search, "test_search" },
        { test_delete, "test_delete" },
        { test_search_empty, "test_search_empty" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
