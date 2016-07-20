/**
 * @file main.c
 * @version 2.0.0
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

static char buffer[2048];

static int test_alloc_free(struct harness_t *harness)
{
    int i;
    struct heap_t heap;
    void *buffers[16];
    size_t sizes[8] = { 16, 32, 64, 128, 256, 512, 512, 512 };

    BTASSERT(heap_init(&heap, buffer, sizeof(buffer), sizes) == 0);

    /* Allocate a few buffers... */
    for (i = 0; i < 16; i++) {
        buffers[i] = heap_alloc(&heap, 1 + (4 * i));
        BTASSERT(buffers[i] != NULL);
    }

    /* ...and free them. */
    for (i = 0; i < 16; i++) {
        BTASSERT(heap_free(&heap, buffers[i]) == 0);
    }

    /* Allocate from the free list... */
    for (i = 0; i < 16; i++) {
        buffers[i] = heap_alloc(&heap, 1 + (4 * i));
        BTASSERT(buffers[i] != NULL);
    }

    /* ...and free them. */
    for (i = 0; i < 16; i++) {
        BTASSERT(heap_free(&heap, buffers[i]) == 0);
    }

    return (0);
}

static int test_double_free(struct harness_t *harness)
{
    struct heap_t heap;
    void *buf_p;
    size_t sizes[8] = { 16, 32, 64, 128, 256, 512, 512, 512 };

    BTASSERT(heap_init(&heap, buffer, sizeof(buffer), sizes) == 0);

    buf_p = heap_alloc(&heap, 1);
    BTASSERT(buf_p != NULL);
    BTASSERT(heap_free(&heap, buf_p) == 0);
    BTASSERT(heap_free(&heap, buf_p) == -1);

    return (0);
}

static int test_share(struct harness_t *harness)
{
    struct heap_t heap;
    void *buf_p;
    size_t sizes[8] = { 16, 32, 64, 128, 256, 512, 512, 512 };

    BTASSERT(heap_init(&heap, buffer, sizeof(buffer), sizes) == 0);

    buf_p = heap_alloc(&heap, 1);
    BTASSERT(buf_p != NULL);
    BTASSERT(heap_share(&heap, buf_p, 1) == 0);
    BTASSERT(heap_free(&heap, buf_p) == 1);
    BTASSERT(heap_free(&heap, buf_p) == 0);

    return (0);
}

static int test_big_buffer(struct harness_t *harness)
{
    struct heap_t heap;
    void *buffers[4];
    size_t sizes[8] = { 16, 32, 64, 128, 256, 512, 512, 512 };

    BTASSERT(heap_init(&heap, buffer, sizeof(buffer), sizes) == 0);

    /* Allocate new block. */
    buffers[0] = heap_alloc(&heap, 513);
    BTASSERT(buffers[0] != NULL);
    BTASSERT(heap_free(&heap, buffers[0]) == 0);

    /* Allocate from the free list. */
    buffers[0] = heap_alloc(&heap, 513);
    BTASSERT(buffers[0] != NULL);
    BTASSERT(heap_free(&heap, buffers[0]) == 0);

    /* Allocate from the end of the free list*/
    buffers[0] = heap_alloc(&heap, 513);
    BTASSERT(buffers[0] != NULL);
    buffers[1] = heap_alloc(&heap, 514);
    BTASSERT(buffers[0] != NULL);
    BTASSERT(heap_free(&heap, buffers[1]) == 0);
    BTASSERT(heap_free(&heap, buffers[0]) == 0);
    buffers[0] = heap_alloc(&heap, 514);
    BTASSERT(buffers[0] != NULL);
    BTASSERT(heap_free(&heap, buffers[0]) == 0);

    return (0);
}

static int test_out_of_memory(struct harness_t *harness)
{
    struct heap_t heap;
    void *buf_p;
    size_t sizes[8] = { 16, 32, 64, 128, 256, 512, 512, 512 };

    BTASSERT(heap_init(&heap, buffer, 32, sizes) == 0);

    buf_p = heap_alloc(&heap, 1);
    BTASSERT(buf_p != NULL);

    buf_p = heap_alloc(&heap, 26);
    BTASSERT(buf_p == NULL);

    /* Big buffer. */
    buf_p = heap_alloc(&heap, 3000);
    BTASSERT(buf_p == NULL);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_alloc_free, "test_alloc_free" },
        { test_double_free, "test_double_free" },
        { test_share, "test_share" },
        { test_big_buffer, "test_big_buffer" },
        { test_out_of_memory, "test_out_of_memory" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
