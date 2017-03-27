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

static void print_node(struct binary_tree_node_t *node_p)
{
    std_printf(FSTR("key = %d\r\n"), node_p->key);

    if (node_p->left_p != NULL) {
        std_printf(FSTR("left: "));
        print_node(node_p->left_p);
    }

    if (node_p->right_p != NULL) {
        std_printf(FSTR("right: "));
        print_node(node_p->right_p);
    }
}

static int node_height(struct binary_tree_node_t *node_p)
{
    return (node_p ? node_p->height : 0);
}

static void node_recalc(struct binary_tree_node_t *node_p)
{
    node_p->height = (1 + MAX(node_height(node_p->left_p),
                              node_height(node_p->right_p)));
}

static struct binary_tree_node_t *
node_rotate_right(struct binary_tree_node_t *node_p)
{
    struct binary_tree_node_t *left_p = node_p->left_p;

    node_p->left_p = left_p->right_p;
    left_p->right_p = node_p;

    node_recalc(node_p);
    node_recalc(left_p);

    return (left_p);
}

static struct binary_tree_node_t *
node_rotate_left(struct binary_tree_node_t * node_p)
{
    struct binary_tree_node_t *right_p = node_p->right_p;

    node_p->right_p = right_p->left_p;
    right_p->left_p = node_p;

    node_recalc(node_p);
    node_recalc(right_p);

    return (right_p);
}

static struct binary_tree_node_t *
node_balance(struct binary_tree_node_t *node_p)
{
    node_recalc(node_p);

    if ((node_height(node_p->left_p)
         - node_height(node_p->right_p)) == 2) {
        if (node_height(node_p->left_p->right_p)
            > node_height(node_p->left_p->left_p)) {
            node_p->left_p = node_rotate_left(node_p->left_p);
        }

        return (node_rotate_right(node_p));
    } else if ((node_height(node_p->right_p)
                - node_height(node_p->left_p)) == 2) {
        if (node_height(node_p->right_p->left_p)
            > node_height(node_p->right_p->right_p)) {
            node_p->right_p = node_rotate_right(node_p->right_p);
        }

        return (node_rotate_left(node_p));
    }

    return (node_p);
}

static int node_insert(struct binary_tree_node_t **parent_pp,
                       struct binary_tree_node_t *node_p)
{
    int res = 0;
    struct binary_tree_node_t *parent_p = *parent_pp;

    if (parent_p == NULL) {
        *parent_pp = node_p;
        return (0);
    }

    if (node_p->key < parent_p->key) {
        res = node_insert(&parent_p->left_p, node_p);
    } else if (node_p->key > parent_p->key ) {
        res = node_insert(&parent_p->right_p, node_p);
    } else {
        return (-1);
    }

    *parent_pp = node_balance(parent_p);

    return (res);
}

static struct binary_tree_node_t *
node_find_min(struct binary_tree_node_t *node_p)
{
    if (node_p->left_p != NULL) {
        return (node_find_min(node_p->left_p));
    } else {
        return (node_p);
    }
}

static struct binary_tree_node_t *
node_delete_min(struct binary_tree_node_t *node_p)
{
    if (node_p->left_p == NULL) {
        return (node_p->right_p);
    }

    node_p->left_p = node_delete_min(node_p->left_p);

    return (node_balance(node_p));
}

static int node_delete_item(struct binary_tree_node_t **node_pp,
                            int key)
{
    int res = 0;
    struct binary_tree_node_t *left_p;
    struct binary_tree_node_t *right_p;
    struct binary_tree_node_t *m_p;
    struct binary_tree_node_t *node_p = *node_pp;

    if (node_p == NULL) {
        return (-1);
    }

    if (key < node_p->key) {
        res = node_delete_item(&node_p->left_p, key);
    } else if (key > node_p->key) {
        res = node_delete_item(&node_p->right_p, key);
    } else {
        left_p = node_p->left_p;
        right_p = node_p->right_p;

        if (right_p == NULL) {
            *node_pp = left_p;

            return (0);
        }

        m_p = node_find_min(right_p);
        m_p->right_p = node_delete_min(right_p);
        m_p->left_p = left_p;

        *node_pp = node_balance(m_p);

        return (0);
    }

    *node_pp = node_balance(node_p);

    return (res);
}

static struct binary_tree_node_t *
node_search(struct binary_tree_node_t *node_p, int key)
{
    if (node_p == NULL) {
        return (NULL);
    }

    if (key < node_p->key) {
        return node_search(node_p->left_p, key);
    } else if (key > node_p->key) {
        return node_search(node_p->right_p, key);
    } else {
        return (node_p);
    }
}

int binary_tree_init(struct binary_tree_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->root_p = NULL;

    return (0);
}

int binary_tree_insert(struct binary_tree_t *self_p,
                       struct binary_tree_node_t *node_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(node_p != NULL, EINVAL);

    node_p->height = 1;
    node_p->left_p = NULL;
    node_p->right_p= NULL;

    return (node_insert(&self_p->root_p, node_p));
}

int binary_tree_delete(struct binary_tree_t *self_p,
                       int key)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (node_delete_item(&self_p->root_p, key));
}

struct binary_tree_node_t *
binary_tree_search(struct binary_tree_t *self_p,
                   int key)
{
    ASSERTNRN(self_p != NULL, EINVAL);

    return (node_search(self_p->root_p, key));
}

void binary_tree_print(struct binary_tree_t *self_p)
{
    ASSERTNRV(self_p != NULL, EINVAL);

    if (self_p->root_p == NULL) {
        std_printf(FSTR("empty\r\n"));
    } else {
        std_printf(FSTR("root: "));
        print_node(self_p->root_p);
        std_printf(FSTR("\r\n"));
    }
}
