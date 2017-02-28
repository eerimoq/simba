/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
    for (i = 4; i < membersof(nodes) / 2; i++) {
        key = i;
        std_printf(FSTR("Deleting node with key %d\r\n"), key);
        BTASSERT(binary_tree_delete(&foo, key) == 0);
    }

    for (; i < membersof(nodes) - 4; i++) {
        key = (2 * membersof(nodes) - i);
        std_printf(FSTR("Deleting node with key %d\r\n"), key);
        BTASSERT(binary_tree_delete(&foo, key) == 0);
    }

    /* Print the binary tree. */
    binary_tree_print(&foo);

    /* Delete a non-existing node. */
    BTASSERT(binary_tree_delete(&foo, 4) == -1);

    /* Delete the rest of the nodes. */
    for (i = membersof(nodes) - 4; i < membersof(nodes); i++) {
        key = (2 * membersof(nodes) - i);
        std_printf(FSTR("Deleting node with key %d\r\n"), key);
        BTASSERT(binary_tree_delete(&foo, key) == 0);
    }

    for (i = 0; i < 4; i++) {
        key = i;
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
