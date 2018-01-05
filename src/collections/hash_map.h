/**
 * @section License
 *
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

#ifndef __COLLECTIONS_HASH_MAP_H__
#define __COLLECTIONS_HASH_MAP_H__

#include "simba.h"

typedef int (*hash_map_hash_t)(longptr_t key);

struct hash_map_entry_t {
    struct hash_map_entry_t *next_p;
    longptr_t key;
    longptr_t value;
};

struct hash_map_bucket_t {
    struct hash_map_entry_t *list_p;
};

struct hash_map_t {
    struct hash_map_bucket_t *buckets_p;
    size_t buckets_max;
    struct hash_map_entry_t *entries_p;
    hash_map_hash_t hash;
};

/**
 * Initialize hash map with given parameters.
 *
 * @param[in,out] self_p Initialized hash map.
 * @param[in] buckets_p Array of buckets.
 * @param[in] buckets_max Number of entries in `buckets_p`.
 * @param[in] entries_p Array of empty entries.
 * @param[in] entries_max Number of entries in `entries_p`.
 * @param[in] hash Hash function.
 *
 * @return zero(0) or negative error code.
 */
int hash_map_init(struct hash_map_t *self_p,
                  struct hash_map_bucket_t *buckets_p,
                  size_t buckets_max,
                  struct hash_map_entry_t *entries_p,
                  size_t entries_max,
                  hash_map_hash_t hash);

/**
 * Add given key-value pair into hash map. Overwrites old value if the
 * key is already present in map.
 *
 * @param[in] self_p Initialized hash map.
 * @param[in] key Key to add.
 * @param[in] value Value to insert for key.
 *
 * @return zero(0) or negative error code.
 */
int hash_map_add(struct hash_map_t *self_p,
                 longptr_t key,
                 longptr_t value);

/**
 * Remove given key from hash map.
 *
 * @param[in] self_p Initialized hash map.
 * @param[in] key Key to remove.
 *
 * @return zero(0) or negative error code.
 */
int hash_map_remove(struct hash_map_t *self_p,
                    longptr_t key);

/**
 * Get value for given key.
 *
 * @param[in] self_p Initialized hash map.
 * @param[in] key Key to find.
 * @param[out] value_p Value found for given key. Unmodified if the
 *                     key was not found.
 *
 * @return zero(0) if the key was found, otherwise negative error
 *         code.
 */
int hash_map_get(struct hash_map_t *self_p,
                 longptr_t key,
                 longptr_t *value_p);

#endif
