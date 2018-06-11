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

struct my_elem_t {
    struct list_elem_t base;
    int foo;
};

static int test_init(void)
{
    struct list_t list;

    BTASSERT(list_init(&list) == 0);

    return (0);
}

static int test_add_remove(void)
{
    struct list_t list;
    struct my_elem_t elems[3];

    BTASSERT(list_init(&list) == 0);

    BTASSERT(list_add_head(&list, &elems[0]) == 0);
    BTASSERT(list_add_head(&list, &elems[1]) == 0);
    BTASSERT(list_add_head(&list, &elems[2]) == 0);

    BTASSERT(list_remove_head(&list) == &elems[2]);
    BTASSERT(list_remove_head(&list) == &elems[1]);
    BTASSERT(list_remove_head(&list) == &elems[0]);

    BTASSERT(list_add_head(&list, &elems[0]) == 0);
    BTASSERT(list_add_head(&list, &elems[1]) == 0);
    BTASSERT(list_add_tail(&list, &elems[2]) == 0);

    BTASSERT(list_remove_head(&list) == &elems[1]);
    BTASSERT(list_remove_head(&list) == &elems[0]);
    BTASSERT(list_remove_head(&list) == &elems[2]);

    BTASSERT(list_remove_head(&list) == NULL);
    BTASSERT(list_remove(&list, &elems[0]) == NULL);

    BTASSERT(list_add_tail(&list, &elems[2]) == 0);
    BTASSERT(list_add_tail(&list, &elems[1]) == 0);
    BTASSERT(list_remove(&list, &elems[0]) == NULL);
    BTASSERT(list_add_tail(&list, &elems[0]) == 0);
    BTASSERT(list_remove(&list, &elems[1]) == &elems[1]);
    BTASSERT(list_remove(&list, &elems[1]) == NULL);
    BTASSERT(list_remove(&list, &elems[2]) == &elems[2]);
    BTASSERT(list_remove(&list, &elems[2]) == NULL);
    BTASSERT(list_remove(&list, &elems[0]) == &elems[0]);
    BTASSERT(list_remove(&list, &elems[0]) == NULL);

    return (0);
}

static int test_iter(void)
{
    struct list_t list;
    struct my_elem_t elems[3];
    struct list_iter_t iter;

    BTASSERT(list_init(&list) == 0);

    BTASSERT(list_add_head(&list, &elems[0]) == 0);
    BTASSERT(list_add_head(&list, &elems[1]) == 0);
    BTASSERT(list_add_head(&list, &elems[2]) == 0);

    BTASSERT(list_iter_init(&iter, &list) == 0);

    BTASSERT(list_iter_next(&iter) == &elems[2]);
    BTASSERT(list_iter_next(&iter) == &elems[1]);
    BTASSERT(list_iter_next(&iter) == &elems[0]);
    BTASSERT(list_iter_next(&iter) == NULL);

    return (0);
}

static int test_peek(void)
{
    struct list_t list;
    struct my_elem_t elems[3];

    BTASSERT(list_init(&list) == 0);

    BTASSERT(list_peek_head(&list) == NULL);

    BTASSERT(list_add_head(&list, &elems[0]) == 0);
    BTASSERT(list_peek_head(&list) == &elems[0]);

    BTASSERT(list_add_head(&list, &elems[1]) == 0);
    BTASSERT(list_peek_head(&list) == &elems[1]);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_add_remove, "test_add_remove" },
        { test_iter, "test_iter" },
        { test_peek, "test_peek" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
