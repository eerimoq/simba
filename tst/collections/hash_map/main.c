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

static int hash(long key)
{
    return (0);
}

int test_add_get_remove(struct harness_t *harness)
{
    struct hash_map_t map;
    struct hash_map_bucket_t buckets[8];
    struct hash_map_entry_t entries[4];

    hash_map_init(&map,
                  buckets,
                  membersof(buckets),
                  entries,
                  membersof(entries),
                  hash);

    /* Add three entries. */
    BTASSERT(hash_map_add(&map, 37, (void *)34) == 0);
    BTASSERT(hash_map_add(&map, 38, (void *)35) == 0);
    BTASSERT(hash_map_add(&map, 39, (void *)36) == 0);
    BTASSERT(hash_map_add(&map, 39, (void *)36) == 0);

    /* Get them. */
    BTASSERT(hash_map_get(&map, 38) == (void *)35);
    BTASSERT(hash_map_get(&map, 39) == (void *)36);
    BTASSERT(hash_map_get(&map, 37) == (void *)34);

    /* Remove first two. */
    BTASSERT(hash_map_remove(&map, 37) == 0);
    BTASSERT(hash_map_remove(&map, 38) == 0);
    BTASSERT(hash_map_remove(&map, 38) == -1);

    /* Get removed entries. */
    BTASSERT(hash_map_get(&map, 37) == NULL);
    BTASSERT(hash_map_get(&map, 38) == NULL);

    /* Get, remove and get last entry. */
    BTASSERT(hash_map_get(&map, 39) == (void *)36);
    BTASSERT(hash_map_remove(&map, 39) == 0);
    BTASSERT(hash_map_remove(&map, 39) == -1);
    BTASSERT(hash_map_get(&map, 39) == NULL);

    /* Add one entry over limit. */
    BTASSERT(hash_map_add(&map, 37, (void *)4) == 0);
    BTASSERT(hash_map_add(&map, 39, (void *)5) == 0);
    BTASSERT(hash_map_add(&map, 41, (void *)6) == 0);
    BTASSERT(hash_map_add(&map, 43, (void *)7) == 0);
    BTASSERT(hash_map_add(&map, 45, (void *)8) == -ENOMEM);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_add_get_remove, "test_add_get_remove" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
