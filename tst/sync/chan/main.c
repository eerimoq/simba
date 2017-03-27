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

static int write_filter_return_value;
static char buffer[8];

static ssize_t write_cb(void *self_p,
                        const void *buf_p,
                        size_t size)
{
    memcpy(buffer, buf_p, size);

    return (size);
}

static int write_filter(void *self_p, const void *buf_p, size_t size)
{
    return (write_filter_return_value);
}

static int test_filter(struct harness_t *harness_p)
{
    struct chan_t chan;
    char buf[2];

    BTASSERT(chan_init(&chan,
                       chan_read_null,
                       write_cb,
                       chan_size_null) == 0);

    /* Write without filter function. */
    buf[0] = 1;
    buf[1] = 2;
    BTASSERT(chan_write(&chan, &buf[0], 2) == 2);
    BTASSERT(memcmp(buffer, buf, 2) == 0);

    /* Filter out the message. */
    BTASSERT(chan_set_write_filter_cb(&chan, write_filter) == 0);
    buf[0] = 1;
    buf[1] = 2;
    write_filter_return_value = 1;
    memset(buffer, -1, sizeof(buffer));
    BTASSERT(chan_write(&chan, &buf[0], 2) == 2);
    buf[0] = -1;
    buf[1] = -1;
    BTASSERT(memcmp(buffer, buf, 2) == 0);
    BTASSERT(chan_set_write_filter_cb(&chan, NULL) == 0);

    /* Write isr without filter function. */
    BTASSERT(chan_set_write_isr_cb(&chan, write_cb) == 0);
    buf[0] = 1;
    buf[1] = 2;
    BTASSERT(chan_write_isr(&chan, &buf[0], 2) == 2);
    BTASSERT(memcmp(buffer, buf, 2) == 0);

    /* Filter out the message. */
    BTASSERT(chan_set_write_filter_isr_cb(&chan, write_filter) == 0);
    buf[0] = 1;
    buf[1] = 2;
    write_filter_return_value = 1;
    memset(buffer, -1, sizeof(buffer));
    BTASSERT(chan_write_isr(&chan, &buf[0], 2) == 2);
    buf[0] = -1;
    buf[1] = -1;
    BTASSERT(memcmp(buffer, buf, 2) == 0);
    BTASSERT(chan_set_write_filter_isr_cb(&chan, NULL) == 0);

    return (0);
}

static int test_null_channels(struct harness_t *harness_p)
{
    struct chan_t chan;
    char value;

    BTASSERT(chan_init(&chan,
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);

    BTASSERT(chan_read(&chan, &value, 1) == -1);
    BTASSERT(chan_write(&chan, &value, 1) == 1);
    BTASSERT(chan_size(&chan) == 1);

    return (0);
}

static int test_list(struct harness_t *harness_p)
{
    struct chan_list_t list;
    void *workspace[1];
    struct chan_t chan[2];

    BTASSERT(chan_init(&chan[0],
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);
    BTASSERT(chan_init(&chan[1],
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);

    BTASSERT(chan_list_init(&list, &workspace[0], sizeof(workspace)) == 0);
    BTASSERT(chan_list_add(&list, &chan[0]) == 0);
    BTASSERT(chan[0].list_p != NULL);
    BTASSERT(chan_list_add(&list, &chan[1]) == -ENOMEM);
    BTASSERT(chan_list_remove(&list, &chan[1]) == -1);
    BTASSERT(chan_list_remove(&list, &chan[0]) == 0);
    BTASSERT(chan[0].list_p == NULL);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_filter, "test_filter" },
        { test_null_channels, "test_null_channels" },
        { test_list, "test_list" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
