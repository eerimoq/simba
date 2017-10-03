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

static ssize_t read_mock(void *self_p,
                         void *buf_p,
                         size_t size)
{
    ssize_t res;

    harness_mock_read("read_mock(): return (res)",
                      &res,
                      sizeof(res));

    if (res > 0) {
        harness_mock_read("read_mock(): return (buf_p)",
                          buf_p,
                          size);
    }

    return (res);
}

static ssize_t write_mock(void *self_p,
                          const void *buf_p,
                          size_t size)
{
    ssize_t res;

    harness_mock_write("write_mock(buf_p)",
                       buf_p,
                       size);
    harness_mock_read("write_mock(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

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

static int test_filter(void)
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

static int test_null_channels(void)
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

static int test_list(void)
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

static int test_getc(void)
{
    struct chan_t chan;
    ssize_t res;
    unsigned char character;
    
    BTASSERT(chan_init(&chan,
                       read_mock,
                       chan_write_null,
                       chan_size_null) == 0);

    /* A positive signed char. */
    res = sizeof(character);
    harness_mock_write("read_mock(): return (res)",
                       &res,
                       sizeof(res));
    character = 'f';
    harness_mock_write("read_mock(): return (buf_p)",
                       &character,
                       sizeof(character));

    BTASSERTI(chan_getc(&chan), ==, 'f');

    /* A negative signed char. */
    res = sizeof(character);
    harness_mock_write("read_mock(): return (res)",
                       &res,
                       sizeof(res));
    character = 0xff;
    harness_mock_write("read_mock(): return (buf_p)",
                       &character,
                       sizeof(character));

    BTASSERTI(chan_getc(&chan), ==, 0xff);

    /* Input output error gives negative error code. */
    res = -EIO;
    harness_mock_write("read_mock(): return (res)",
                       &res,
                       sizeof(res));

    BTASSERTI(chan_getc(&chan), ==, -EIO);
    
    return (0);
}

static int test_putc(void)
{
    struct chan_t chan;
    ssize_t res;
    unsigned char character;
    
    BTASSERT(chan_init(&chan,
                       chan_read_null,
                       write_mock,
                       chan_size_null) == 0);

    /* A positive signed char. */
    res = sizeof(character);
    harness_mock_write("write_mock(): return (res)",
                       &res,
                       sizeof(res));

    BTASSERTI(chan_putc(&chan, 'f'), ==, 0);

    character = '\0';
    harness_mock_read("write_mock(buf_p)",
                       &character,
                       sizeof(character));
    BTASSERTI(character, ==, 'f');

    /* A negative signed char. */
    res = sizeof(character);
    harness_mock_write("write_mock(): return (res)",
                       &res,
                       sizeof(res));

    BTASSERTI(chan_putc(&chan, 0xfe), ==, 0);

    character = '\0';
    harness_mock_read("write_mock(buf_p)",
                       &character,
                       sizeof(character));
    BTASSERTI(character, ==, 0xfe);

    /* Input output error gives negative error code. */
    res = -EIO;
    harness_mock_write("write_mock(): return (res)",
                       &res,
                       sizeof(res));

    BTASSERTI(chan_putc(&chan, ' '), ==, -EIO);

    character = '\0';
    harness_mock_read("write_mock(buf_p)",
                       &character,
                       sizeof(character));
    BTASSERTI(character, ==, ' ');
    
    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_filter, "test_filter" },
        { test_null_channels, "test_null_channels" },
        { test_list, "test_list" },
        { test_getc, "test_getc" },
        { test_putc, "test_putc" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
