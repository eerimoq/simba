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

static struct queue_t queue;
static char queue_buf[256];

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(queue_init(&queue, queue_buf, sizeof(queue_buf)) == 0);

    BTASSERT(nvm_module_init() == 0);
    BTASSERT(nvm_module_init() == 0);

    return (0);
}

static int test_format_mount(struct harness_t *harness_p)
{
    BTASSERT(nvm_format() == 0);
    BTASSERT(nvm_mount() == 0);

    return (0);
}

static int test_read_write(struct harness_t *harness_p)
{
    uint8_t byte;

    byte = 0;
    BTASSERT(nvm_write(0, &byte, sizeof(byte)) == sizeof(byte));
    BTASSERT(nvm_read(&byte, 1, sizeof(byte)) == sizeof(byte));
    BTASSERT(byte == 0xff);

    return (0);
}

static int test_read_write_bad_address(struct harness_t *harness_p)
{
    uint8_t byte;

    byte = 0;
    BTASSERT(nvm_write(CONFIG_NVM_SIZE, &byte, sizeof(byte)) == -EINVAL);
    BTASSERT(nvm_read(&byte, CONFIG_NVM_SIZE, sizeof(byte)) == -EINVAL);

    return (0);
}

static int test_fs_commands(struct harness_t *harness_p)
{
    char buf[64];

    /* Write. */
    strcpy(&buf[0], "/oam/nvm/write");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == -EINVAL);
    BTASSERT(harness_expect(&queue,
                            "Usage: write <address> <data>\r\n",
                            NULL) == 31);

    strcpy(&buf[0], "/oam/nvm/write foo bar");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == -EINVAL);
    BTASSERT(harness_expect(&queue, "foo: bad address\r\n", NULL) == 18);

    strcpy(&buf[0], "/oam/nvm/write 0x2 bar");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == -EINVAL);
    BTASSERT(harness_expect(&queue, "bar: bad data\r\n", NULL) == 15);

    strcpy(&buf[0], "/oam/nvm/write 0x2 0x123");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == -EINVAL);
    BTASSERT(harness_expect(&queue,
                            "0x123: data out of range\r\n",
                            NULL) == 26);

    strcpy(&buf[0], "/oam/nvm/write 0x2 0x12");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == 0);

    /* Read. */
    strcpy(&buf[0], "/oam/nvm/read");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == -EINVAL);
    BTASSERT(harness_expect(&queue,
                            "Usage: read <address>\r\n",
                            NULL) == 23);

    strcpy(&buf[0], "/oam/nvm/read fie");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == -EINVAL);
    BTASSERT(harness_expect(&queue, "fie: bad address\r\n", NULL) == 18);

    strcpy(&buf[0], "/oam/nvm/read 0x2");
    BTASSERT(fs_call(&buf[0], chan_null(), &queue, NULL) == 0);
    BTASSERT(harness_expect(&queue, "0x12\r\n", NULL) == 6);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_format_mount, "test_format_mount" },
        { test_read_write, "test_read_write" },
        { test_read_write_bad_address, "test_read_write_bad_address" },
        { test_fs_commands, "test_fs_commands" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
