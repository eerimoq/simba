/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
