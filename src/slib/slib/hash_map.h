/**
 * @file slib/hash_map.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#ifndef __SLIB_HASH_MAP_H__
#define __SLIB_HASH_MAP_H__

#include "simba.h"

typedef int (*hash_function_t)(long key);

struct hash_map_entry_t {
    struct hash_map_entry_t *next_p;
    long key;
    void *value_p;
};

struct hash_map_bucket_t {
    struct hash_map_entry_t *list_p;
};

struct hash_map_t {
    struct hash_map_bucket_t *buckets_p;
    size_t buckets_max;
    struct hash_map_entry_t *entries_p;
    hash_function_t hash;
};

/**
 * Initialize hash map with given parameters.
 *
 * @param[out] self_p Initialized hash map.
 * @param[in] entries_p Array of empty entries.
 * @param[in] entries_max Number of entries in 'entries'.
 * @param[in] hash Hash function.
 *
 * @return zero(0) or negative error code.
 */
int hash_map_init(struct hash_map_t *self_p,
                  struct hash_map_bucket_t *buckets_p,
                  size_t buckets_max,
                  struct hash_map_entry_t *entries_p,
                  size_t entries_max,
                  hash_function_t hash);

/**
 * Add given key-value pair into hash map. Overwrites old value if the
 * key is already present in map.
 *
 * @param[in] self_p Initialized hash map.
 * @param[in] key Key to hash.
 * @param[in] value_p Value to insert for key.
 *
 * @return zero(0) or negative error code.
 */
int hash_map_add(struct hash_map_t *self_p,
                 long key,
                 void *value_p);

/**
 * Remove given key from hash map.
 *
 * @param[in] self_p Initialized hash map.
 * @param[in] key Key to hash.
 *
 * @return zero(0) or negative error code.
 */
int hash_map_remove(struct hash_map_t *self_p,
                    long key);

/**
 * Get value for given key.
 *
 * @param[in] self_p Initialized hash map.
 * @param[in] key Key to hash.
 *
 * @return Value for key or NULL if key was not found in the map.
 */
void *hash_map_get(struct hash_map_t *self_p,
                   long key);

#endif
