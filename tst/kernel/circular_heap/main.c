/**
 * @file main.c
 * @version 1.0.0
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

static char buffer[256];

static int test_alloc_free(struct harness_t *harness_p)
{
    struct circular_heap_t circular_heap;
    void *bufs[16];
    int number_of_buffers;
    int i;

    /* Initialize the circular heap. */
    BTASSERT(circular_heap_init(&circular_heap,
                                  buffer,
                                  sizeof(buffer)) == 0);
    
    /* Allocate all memory (). */
    number_of_buffers = 0;

    while (1) {
        bufs[number_of_buffers] = circular_heap_alloc(&circular_heap, 32);

        if (bufs[number_of_buffers] == NULL) {
            break;
        }

        number_of_buffers++;
    }

    BTASSERT(number_of_buffers > 0);
    
    /* Free the three oldest buffers. */
    BTASSERT(circular_heap_free(&circular_heap, bufs[0]) == 0);
    BTASSERT(circular_heap_free(&circular_heap, bufs[1]) == 0);
    BTASSERT(circular_heap_free(&circular_heap, bufs[2]) == 0);

    /* Allocate two buffers. */
    bufs[0] = circular_heap_alloc(&circular_heap, 32);
    BTASSERT(bufs[0] != NULL);
    bufs[1] = circular_heap_alloc(&circular_heap, 32);
    BTASSERT(bufs[1] != NULL);

    /* Out of memory. */
    BTASSERT(circular_heap_alloc(&circular_heap, 256) == NULL);

    /* Free all memory. */
    for (i = 3; i < number_of_buffers; i++) {
        BTASSERT(circular_heap_free(&circular_heap, bufs[i]) == 0);
    }

    BTASSERT(circular_heap_free(&circular_heap, bufs[0]) == 0);
    BTASSERT(circular_heap_free(&circular_heap, bufs[1]) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_alloc_free, "test_alloc_free" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
