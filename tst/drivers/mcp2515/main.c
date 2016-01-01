/**
 * @file main.c
 * @version 0.1.0
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
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
