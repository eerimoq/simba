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

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
