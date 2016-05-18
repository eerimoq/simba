/**
 * @file hash_map.c
 * @version 0.5.0
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

static struct hash_map_entry_t *entry_alloc(struct hash_map_t *self_p)
{
    struct hash_map_entry_t *entry_p;

    entry_p = self_p->entries_p;

    if (entry_p != NULL) {
        self_p->entries_p = entry_p->next_p;
    }

    return (entry_p);
}

static void entry_free(struct hash_map_t *self_p,
                       struct hash_map_entry_t *entry_p)
{
    entry_p->next_p = self_p->entries_p;
    self_p->entries_p = entry_p;
}

int hash_map_init(struct hash_map_t *self_p,
                  struct hash_map_bucket_t *buckets_p,
                  size_t buckets_max,
                  struct hash_map_entry_t *entries_p,
                  size_t entries_max,
                  hash_function_t hash)
{
    size_t i;

    self_p->buckets_p = buckets_p;
    self_p->buckets_max = buckets_max;
    self_p->hash = hash;

    /* Initialize buckets. */
    for (i = 0; i < buckets_max; i++) {
        buckets_p[i].list_p = NULL;
    }

    /* Initialize entries free list. */
    self_p->entries_p = entries_p;

    for (i = 0; i < entries_max - 1; i++) {
        entries_p[i].next_p = &entries_p[i+1];
    }

    entries_p[i].next_p = NULL;

    return (0);
}

int hash_map_add(struct hash_map_t *self_p,
                 long key,
                 void *value_p)
{
    int hash;
    struct hash_map_bucket_t *bucket_p;
    struct hash_map_entry_t *entry_p;

    /* Hash key. */
    hash = self_p->hash(key);
    hash %= self_p->buckets_max;
    bucket_p = &self_p->buckets_p[hash];

    /* Is the key already in map? */
    if (bucket_p->list_p != NULL) {
        entry_p = bucket_p->list_p;

        while (entry_p != NULL) {
            if (entry_p->key == key) {
                entry_p->value_p = value_p;

                return (0);
            }

            entry_p = entry_p->next_p;
        }
    }

    /* Allocate new entry. */
    entry_p = entry_alloc(self_p);

    if (entry_p == NULL) {
        return (-ENOMEM);
    }

    /* Initiate entry.*/
    entry_p->key = key;
    entry_p->value_p = value_p;

    /* Insert in list. */
    entry_p->next_p = bucket_p->list_p;
    bucket_p->list_p = entry_p;

    return (0);
}

int hash_map_remove(struct hash_map_t *self_p,
                    long key)
{
    int hash;
    struct hash_map_bucket_t *bucket_p;
    struct hash_map_entry_t *entry_p, *prev_p;

    /* Hash key. */
    hash = self_p->hash(key);
    hash %= self_p->buckets_max;
    bucket_p = &self_p->buckets_p[hash];

    /* Is the key already in map? */
    if (bucket_p->list_p != NULL) {
        entry_p = bucket_p->list_p;
        prev_p = NULL;

        while (entry_p != NULL) {
            if (entry_p->key == key) {
                if (prev_p != NULL) {
                    prev_p->next_p = entry_p->next_p;
                } else {
                    bucket_p->list_p = entry_p->next_p;
                }

                entry_free(self_p, entry_p);

                return (0);
            }

            prev_p = entry_p;
            entry_p = entry_p->next_p;
        }
    }

    return (-1);
}

void *hash_map_get(struct hash_map_t *self_p,
                   long key)
{
    int hash;
    struct hash_map_bucket_t *bucket_p;
    struct hash_map_entry_t *entry_p;

    /* Hash key. */
    hash = self_p->hash(key);
    hash %= self_p->buckets_max;
    bucket_p = &self_p->buckets_p[hash];

    /* Search for key. */
    if (bucket_p->list_p != NULL) {
        entry_p = bucket_p->list_p;
        
        while (entry_p != NULL) {
            if (entry_p->key == key) {
                return (entry_p->value_p);
            }
            
            entry_p = entry_p->next_p;
        }        
    }

    return (NULL);
}
