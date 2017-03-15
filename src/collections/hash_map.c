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
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buckets_p != NULL, EINVAL);
    ASSERTN(buckets_max > 0, EINVAL);
    ASSERTN(entries_p != NULL, EINVAL);
    ASSERTN(entries_max > 0, EINVAL);
    ASSERTN(hash != NULL, EINVAL);

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
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(value_p != NULL, EINVAL);

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
    ASSERTN(self_p != NULL, EINVAL);

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
    ASSERTNRN(self_p != NULL, EINVAL);

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
