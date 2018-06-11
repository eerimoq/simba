/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "binary_tree_mock.h"

int mock_write_binary_tree_init(int res)
{
    harness_mock_write("binary_tree_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(binary_tree_init)(struct binary_tree_t *self_p)
{
    int res;

    harness_mock_read("binary_tree_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_binary_tree_insert(struct binary_tree_node_t *node_p,
                                  int res)
{
    harness_mock_write("binary_tree_insert(node_p)",
                       node_p,
                       sizeof(*node_p));

    harness_mock_write("binary_tree_insert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(binary_tree_insert)(struct binary_tree_t *self_p,
                                                    struct binary_tree_node_t *node_p)
{
    int res;

    harness_mock_assert("binary_tree_insert(node_p)",
                        node_p,
                        sizeof(*node_p));

    harness_mock_read("binary_tree_insert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_binary_tree_delete(int key,
                                  int res)
{
    harness_mock_write("binary_tree_delete(key)",
                       &key,
                       sizeof(key));

    harness_mock_write("binary_tree_delete(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(binary_tree_delete)(struct binary_tree_t *self_p,
                                                    int key)
{
    int res;

    harness_mock_assert("binary_tree_delete(key)",
                        &key,
                        sizeof(key));

    harness_mock_read("binary_tree_delete(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_binary_tree_search(int key,
                                  struct binary_tree_node_t *
res)
{
    harness_mock_write("binary_tree_search(key)",
                       &key,
                       sizeof(key));

    harness_mock_write("binary_tree_search(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct binary_tree_node_t *
__attribute__ ((weak)) STUB(binary_tree_search)(struct binary_tree_t *self_p,
                                                                            int key)
{
    struct binary_tree_node_t *
res;

    harness_mock_assert("binary_tree_search(key)",
                        &key,
                        sizeof(key));

    harness_mock_read("binary_tree_search(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_binary_tree_print()
{
    return (0);
}

void __attribute__ ((weak)) STUB(binary_tree_print)(struct binary_tree_t *self_p)
{
}
