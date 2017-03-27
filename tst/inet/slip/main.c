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

static struct slip_t slip;
static uint8_t slip_buf[64];
static struct queue_t queue;
static uint8_t queue_buf[64];

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(queue_init(&queue, &queue_buf[0], sizeof(queue_buf)) == 0);
    BTASSERT(slip_init(&slip, &slip_buf[0], sizeof(slip_buf), &queue) == 0);

    return (0);
}

static int test_input(struct harness_t *harness_p)
{
    /* Write a packet to the slip channel. */
    BTASSERT(slip_input(&slip, 0xc0) == 0);
    BTASSERT(slip_input(&slip, 0x00) == 0);
    BTASSERT(slip_input(&slip, 0xdb) == 0);
    BTASSERT(slip_input(&slip, 0xdc) == 0);
    BTASSERT(slip_input(&slip, 0xdb) == 0);
    BTASSERT(slip_input(&slip, 0xdd) == 0);
    BTASSERT(slip_input(&slip, 0xdc) == 0);
    BTASSERT(slip_input(&slip, 0xdd) == 0);
    BTASSERT(slip_input(&slip, 0xc0) == 5);

    BTASSERT(slip.rx.buf_p[0] == 0x00);
    BTASSERT(slip.rx.buf_p[1] == 0xc0);
    BTASSERT(slip.rx.buf_p[2] == 0xdb);
    BTASSERT(slip.rx.buf_p[3] == 0xdc);
    BTASSERT(slip.rx.buf_p[4] == 0xdd);

    return (0);
}

static int test_output(struct harness_t *harness_p)
{
    uint8_t buf[64];

    /* Write a packet to the slip channel. */
    BTASSERT(chan_write(slip_get_output_channel(&slip),
                        "\x00\xc0\xdb\xdc\xdd",
                        5) == 5);

    BTASSERT(chan_read(&queue, &buf[0], 9) == 9);
    BTASSERT(buf[0] == 0xc0);
    BTASSERT(buf[1] == 0x00);
    BTASSERT(buf[2] == 0xdb); /* Escape. */
    BTASSERT(buf[3] == 0xdc);
    BTASSERT(buf[4] == 0xdb); /* Escape. */
    BTASSERT(buf[5] == 0xdd);
    BTASSERT(buf[6] == 0xdc);
    BTASSERT(buf[7] == 0xdd);
    BTASSERT(buf[8] == 0xc0);

    return (0);
}

static int test_bad_input(struct harness_t *harness_p)
{
    /* Bad byte (0x00) after escape byte. */
    BTASSERT(slip_input(&slip, 0xc0) == 0);
    BTASSERT(slip_input(&slip, 0xdb) == 0);
    BTASSERT(slip_input(&slip, 0x00) == -1);

    return (0);
}

static int test_truncate_input(struct harness_t *harness_p)
{
    int i;

    BTASSERT(slip_input(&slip, 0xc0) == 0);

    for (i = 0; i < sizeof(slip_buf); i++) {
        BTASSERT(slip_input(&slip, i) == 0);
    }

    BTASSERT(slip_input(&slip, i) == -1);
    BTASSERT(slip_input(&slip, 0xc0) == sizeof(slip_buf));

    for (i = 0; i < sizeof(slip_buf); i++) {
        BTASSERT(slip.rx.buf_p[i] == i);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_input, "test_input" },
        { test_output, "test_output" },
        { test_bad_input, "test_bad_input" },
        { test_truncate_input, "test_truncate_input" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
