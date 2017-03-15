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

#ifndef __COLLECTIONS_BINARY_TREE_H__
#define __COLLECTIONS_BINARY_TREE_H__

#include "simba.h"

struct binary_tree_node_t {
    int key;
    int height;
    struct binary_tree_node_t *left_p;
    struct binary_tree_node_t *right_p;
};

struct binary_tree_t {
    struct binary_tree_node_t *root_p;
};

/**
 * Initialize given binary tree.
 *
 * @param[in] self_p Binary tree.
 *
 * @return zero(0) or negative error code.
 */
int binary_tree_init(struct binary_tree_t *self_p);

/**
 * Insert given node into given binary tree.
 *
 * There can not be two or more nodes in the tree with the same
 * key. This function returns -1 if a node with the same key is
 * already in the binary tree.
 *
 * @param[in] self_p Binary tree to insert the node into.
 * @param[in] node_p Node to insert.
 *
 * @return zero(0) on success, -1 if a node with the same key is
 *         already in the binary tree, otherwise negative error code.
 */
int binary_tree_insert(struct binary_tree_t *self_p,
                       struct binary_tree_node_t *node_p);

/**
 * Delete given node from given binary tree.
 *
 * @param[in] self_p Binary tree to delete the node from.
 * @param[in] key Key of the node to delete.
 *
 * @return zero(0) on success, -1 if the node was not found, otherwise
 *         negative error code.
 */
int binary_tree_delete(struct binary_tree_t *self_p,
                       int key);

/**
 * Search the binary tree for the node with given key.
 *
 * @param[in] self_p Binary tree to search in.
 * @param[in] key Key of the binary tree node to search for.
 *
 * @return Pointer to found node or NULL if a node with given key was
 *         not found in the tree.
 */
struct binary_tree_node_t *
binary_tree_search(struct binary_tree_t *self_p,
                   int key);

/**
 * Print given binary tree.
 *
 * @param[in] self_p Binary tree to print.
 */
void binary_tree_print(struct binary_tree_t *self_p);

#endif
