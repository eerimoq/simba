/**
 * @file main.c
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
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
