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

int test_read_write(struct harness_t *harness_p)
{
    struct circular_buffer_t foo;
    uint8_t foobuf[32];
    uint8_t buf[64];

    BTASSERT(circular_buffer_init(&foo, &foobuf[0], sizeof(foobuf)) == 0);

    BTASSERT(circular_buffer_used_size(&foo) == 0);
    BTASSERT(circular_buffer_unused_size(&foo) == sizeof(foobuf) - 1);

    /* Read from empty buffer. */
    BTASSERT(circular_buffer_read(&foo, &buf[0], 0) == 0);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 1) == 0);
    BTASSERT(circular_buffer_read(&foo, &buf[0], sizeof(foobuf)) == 0);

    /* Write some data. */
    memset(&buf[0], '1', 3);
    BTASSERT(circular_buffer_write(&foo, &buf[0], 3) == 3);
    memset(&buf[0], '2', 4);
    BTASSERT(circular_buffer_write(&foo, &buf[0], 4) == 4);

    /* Read the written data. */
    memset(&buf[0], '0', 1);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 1) == 1);
    BTASSERT(memcmp(&buf[0], "1", 1) == 0);
    memset(&buf[0], '0', 1);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 1) == 1);
    BTASSERT(memcmp(&buf[0], "1", 1) == 0);
    memset(&buf[0], '0', 5);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 20) == 5);
    BTASSERT(memcmp(&buf[0], "12222", 5) == 0);

    /* Wrap around. */
    BTASSERT(circular_buffer_used_size(&foo) == 0);
    memset(&buf[0], '3', 33);
    BTASSERT(circular_buffer_write(&foo, &buf[0], 33) == 31);
    memset(&buf[0], '0', 31);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 32) == 31);
    BTASSERT(memcmp(&buf[0], "3333333333333333333333333333333", 31) == 0);

    /* Reinitialize the circular buffer and the writing to end of
       buffer. */
    BTASSERT(circular_buffer_init(&foo, &foobuf[0], sizeof(foobuf)) == 0);
    memset(&buf[0], '3', 31);
    BTASSERT(circular_buffer_write(&foo, &buf[0], 31) == 31);
    BTASSERT(circular_buffer_used_size(&foo) == 31);
    BTASSERT(circular_buffer_write(&foo, &buf[0], 1) == 0);
    BTASSERT(circular_buffer_used_size(&foo) == 31);
    memset(&buf[0], '0', 31);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 2) == 2);
    BTASSERT(memcmp(&buf[0], "33", 2) == 0);
    BTASSERT(circular_buffer_used_size(&foo) == 29);
    BTASSERT(circular_buffer_write(&foo, &buf[0], 1) == 1);
    BTASSERT(circular_buffer_used_size(&foo) == 30);

    return (0);
}

int test_skip(struct harness_t *harness_p)
{
    struct circular_buffer_t foo;
    uint8_t foobuf[8];
    uint8_t buf[8];

    BTASSERT(circular_buffer_init(&foo, &foobuf[0], sizeof(foobuf)) == 0);

    /* Skip on empty buffer should not have any effect. */
    BTASSERT(circular_buffer_skip_front(&foo, 1) == 0);
    BTASSERT(circular_buffer_reverse_skip_back(&foo, 1) == 0);

    /* Write some data to the buffer (index 0-5). */
    BTASSERT(circular_buffer_write(&foo, "123456", 6) == 6);

    /* Skip from front.*/
    BTASSERT(circular_buffer_skip_front(&foo, 1) == 1);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 1) == 1);
    BTASSERT(buf[0] == '2')
    BTASSERT(circular_buffer_used_size(&foo) == 4);
    BTASSERT(circular_buffer_skip_front(&foo, 1) == 1);
    BTASSERT(circular_buffer_used_size(&foo) == 3);

    /* Skip from back. */
    BTASSERT(circular_buffer_reverse_skip_back(&foo, 2) == 2);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 2) == 1);
    BTASSERT(buf[0] == '4')
    BTASSERT(circular_buffer_used_size(&foo) == 0);

    /* Test wrap around skip front (index 6-3). */
    BTASSERT(circular_buffer_write(&foo, "789012", 6) == 6);
    BTASSERT(circular_buffer_skip_front(&foo, 4) == 4);
    BTASSERT(circular_buffer_used_size(&foo) == 2);
    BTASSERT(circular_buffer_read(&foo, &buf[0], 1) == 1);
    BTASSERT(circular_buffer_skip_front(&foo, 2) == 1);
    BTASSERT(circular_buffer_used_size(&foo) == 0);

    /* Test wrap around reverse skip back (index 4-1). */
    BTASSERT(circular_buffer_write(&foo, "345678", 6) == 6);
    BTASSERT(circular_buffer_reverse_skip_back(&foo, 2) == 2);
    BTASSERT(circular_buffer_used_size(&foo) == 4);
    BTASSERT(circular_buffer_reverse_skip_back(&foo, 2) == 2);
    BTASSERT(circular_buffer_used_size(&foo) == 2);

    return (0);
}

int test_array(struct harness_t *harness_p)
{
    struct circular_buffer_t foo;
    uint8_t foobuf[8];
    void *buf_p;

    BTASSERT(circular_buffer_init(&foo, &foobuf[0], sizeof(foobuf)) == 0);

    /* Get array for empty buffer. */
    BTASSERT(circular_buffer_array_one(&foo, &buf_p, 3) == 0);
    BTASSERT(circular_buffer_array_one(&foo, &buf_p, 0) == 0);
    BTASSERT(circular_buffer_array_two(&foo, &buf_p, 3) == 0);
    BTASSERT(circular_buffer_array_two(&foo, &buf_p, 0) == 0);

    /* Write some data to the buffer. */
    BTASSERT(circular_buffer_write(&foo, "123456", 6) == 6);

    /* Get array one. Array two is empty. */
    BTASSERT(circular_buffer_array_one(&foo, &buf_p, 3) == 3);
    BTASSERT(buf_p == &foobuf[0]);
    BTASSERT(circular_buffer_array_two(&foo, &buf_p, 3) == 0);

    /* Write a byte. No wrap occurs. No space left at the end of the
       buffer. */
    BTASSERT(circular_buffer_write(&foo, "1", 1) == 1);

    /* Get array one. Array two is empty. */
    BTASSERT(circular_buffer_array_one(&foo, &buf_p, 1) == 1);
    BTASSERT(circular_buffer_array_two(&foo, &buf_p, 3) == 0);

    /* Skip two bytes. Write two bytes. Wrap occurs. Buffer two has
       data. */
    BTASSERT(circular_buffer_skip_front(&foo, 2) == 2);
    BTASSERT(circular_buffer_write(&foo, "23", 2) == 2);

    /* Get array one and two. */
    BTASSERT(circular_buffer_used_size(&foo) == 7);
    BTASSERT(circular_buffer_array_one(&foo, &buf_p, 7) == 6);
    BTASSERT(buf_p == &foobuf[2]);
    BTASSERT(circular_buffer_array_two(&foo, &buf_p, 1) == 1);
    BTASSERT(buf_p == &foobuf[0]);
    BTASSERT(circular_buffer_array_two(&foo, &buf_p, 2) == 1);
    BTASSERT(buf_p == &foobuf[0]);
    BTASSERT(circular_buffer_skip_front(&foo, 6) == 6);
    BTASSERT(circular_buffer_array_one(&foo, &buf_p, 7) == 1);
    BTASSERT(buf_p == &foobuf[0]);
    BTASSERT(circular_buffer_array_two(&foo, &buf_p, 2) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { test_skip, "test_skip" },
        { test_array, "test_array" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
