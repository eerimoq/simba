/**
 * @file kernel/binary_tree.h
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

#ifndef __KERNEL_BINARY_TREE_H__
#define __KERNEL_BINARY_TREE_H__

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
 * Initialize binary tree.
 *
 * @param[in,out] self_p Binary tree.
 *
 * @return zero(0) or negative error code.
 */
int binary_tree_init(struct binary_tree_t *self_p);

/**
 * Insert given nodeent into given binary tree.
 *
 * @param[in] self_p Binary tree.
 * @param[in] node_p Nodeent to insert. There can not be two or more
 *                   nodeents in the tree with the same key.
 *
 * @return zero(0) or negative error code.
 */
int binary_tree_insert(struct binary_tree_t *self_p,
                       struct binary_tree_node_t *node_p);

/**
 * Delete given nodeent from given binary tree.
 *
 * @param[in] self_p Binary tree.
 * @param[in] key Key to delete.
 *
 * @return zero(0) or negative error code.
 */
int binary_tree_delete(struct binary_tree_t *self_p,
                       int key);

/**
 * Search for the binary tree nodeent with given key.
 *
 * @param[in] self_p Binary tree.
 * @param[in] key Key of the binary tree nodeent to search for.
 *
 * @return Value for key or NULL if key was not found in the tree.
 */
struct binary_tree_node_t *
binary_tree_search(struct binary_tree_t *self_p,
                   int key);

/**
 * Print given binary tree.
 *
 * @param[in] self_p binary tree.
 */
void binary_tree_print(struct binary_tree_t *self_p);

#endif
