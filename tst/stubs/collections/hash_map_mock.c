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

#include "simba.h"
#include "hash_map_mock.h"

int mock_write_hash_map_init(struct hash_map_bucket_t *buckets_p,
                             size_t buckets_max,
                             struct hash_map_entry_t *entries_p,
                             size_t entries_max,
                             hash_function_t hash,
                             int res)
{
    harness_mock_write("hash_map_init(buckets_p)",
                       buckets_p,
                       sizeof(*buckets_p));

    harness_mock_write("hash_map_init(buckets_max)",
                       &buckets_max,
                       sizeof(buckets_max));

    harness_mock_write("hash_map_init(entries_p)",
                       entries_p,
                       sizeof(*entries_p));

    harness_mock_write("hash_map_init(entries_max)",
                       &entries_max,
                       sizeof(entries_max));

    harness_mock_write("hash_map_init(hash)",
                       &hash,
                       sizeof(hash));

    harness_mock_write("hash_map_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hash_map_init)(struct hash_map_t *self_p,
                                               struct hash_map_bucket_t *buckets_p,
                                               size_t buckets_max,
                                               struct hash_map_entry_t *entries_p,
                                               size_t entries_max,
                                               hash_function_t hash)
{
    int res;

    harness_mock_assert("hash_map_init(buckets_p)",
                        buckets_p,
                        sizeof(*buckets_p));

    harness_mock_assert("hash_map_init(buckets_max)",
                        &buckets_max,
                        sizeof(buckets_max));

    harness_mock_assert("hash_map_init(entries_p)",
                        entries_p,
                        sizeof(*entries_p));

    harness_mock_assert("hash_map_init(entries_max)",
                        &entries_max,
                        sizeof(entries_max));

    harness_mock_assert("hash_map_init(hash)",
                        &hash,
                        sizeof(hash));

    harness_mock_read("hash_map_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hash_map_add(long key,
                            void *value_p,
                            int res)
{
    harness_mock_write("hash_map_add(key)",
                       &key,
                       sizeof(key));

    harness_mock_write("hash_map_add(value_p)",
                       value_p,
                       sizeof(value_p));

    harness_mock_write("hash_map_add(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hash_map_add)(struct hash_map_t *self_p,
                                              long key,
                                              void *value_p)
{
    int res;

    harness_mock_assert("hash_map_add(key)",
                        &key,
                        sizeof(key));

    harness_mock_assert("hash_map_add(value_p)",
                        value_p,
                        sizeof(*value_p));

    harness_mock_read("hash_map_add(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hash_map_remove(long key,
                               int res)
{
    harness_mock_write("hash_map_remove(key)",
                       &key,
                       sizeof(key));

    harness_mock_write("hash_map_remove(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hash_map_remove)(struct hash_map_t *self_p,
                                                 long key)
{
    int res;

    harness_mock_assert("hash_map_remove(key)",
                        &key,
                        sizeof(key));

    harness_mock_read("hash_map_remove(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hash_map_get(long key,
                            void *res)
{
    harness_mock_write("hash_map_get(key)",
                       &key,
                       sizeof(key));

    harness_mock_write("hash_map_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(hash_map_get)(struct hash_map_t *self_p,
                                                long key)
{
    void *res;

    harness_mock_assert("hash_map_get(key)",
                        &key,
                        sizeof(key));

    harness_mock_read("hash_map_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
