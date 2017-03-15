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
    struct canif_frame_t frame;
    struct canif_frame_t frames[1];
    struct mcp2515_driver_t mcp2515;

    BTASSERT(mcp2515_init(&mcp2515,
                          &spi_device[0],
                          &pin_d10_dev,
                          &exti_device[0],
                          MCP2515_MODE_LOOPBACK,
                          MCP2515_SPEED_1000KBPS,
                          NULL,
                          frames,
                          membersof(frames)) == 0);
    BTASSERT(mcp2515_start(&mcp2515) == 0);

    /* Write a frame to the device. */
    memset(&frame, 0, sizeof(frame));
    frame.id = 57;
    frame.data[0] = 9;
    BTASSERT(mcp2515_write(&mcp2515, &frame) == 0);

    /* Read a frame from the device. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(mcp2515_read(&mcp2515, &frame) == 0);

    /* Verify frame contents. */
    BTASSERT(frame.id == 57);
    BTASSERT(frame.data[0] == 9);

    BTASSERT(mcp2515_stop(&mcp2515) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
