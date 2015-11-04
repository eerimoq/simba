/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

/**
 * For Arduino Due:
 *
 * +---------------+
 * |        CAN0TX o----------o----------------+
 * |               |          |                |
 * |               |   Schottky diode          |
 * |               |          |                |
 * |        CAN0RX o----------o--------+       |
 * |               |                   |       |
 * |               |                 R 3k3   R 3k3
 * |               |                   |       |
 * | D53  - CAN1TX o----------o------- o       |
 * |               |          |                |
 * |               |   Schottky diode          |
 * |               |          |                |
 * | DAC0 - CAN1RX o----------o----------------+
 * +---------------+
 */

#include "simba.h"

/* Ping-pong can frame ids. */
#define PING_ID 0x23
#define PONG_ID 0x24

/* CAN receive buffers. */
static char can0_rx_buf[128];
static char can1_rx_buf[128];

static int test_ping_pong(uint32_t speed)
{
    struct can_frame_t frame;
    struct can_driver_t can0;
    struct can_driver_t can1;

    BTASSERT(can_init(&can0,
                      &can_device[0],
                      speed,
                      can0_rx_buf,
                      sizeof(can0_rx_buf)) == 0);
    BTASSERT(can_start(&can0) == 0);

    BTASSERT(can_init(&can1,
                      &can_device[1],
                      speed,
                      can1_rx_buf,
                      sizeof(can1_rx_buf)) == 0);
    BTASSERT(can_start(&can1) == 0);

    /* Ping is transmitted from can0 to can1. */

    /* Write ping. */
    frame.id = PING_ID;
    frame.size = 1;
    frame.data.u8[0] = 0xfe;
    BTASSERT(can_write(&can0, &frame) == sizeof(frame));

    /* Read ping. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can1, &frame) == sizeof(frame));
    BTASSERT(frame.id == PING_ID);
    BTASSERT(frame.size == 1);
    BTASSERT(frame.data.u8[0] == 0xfe);

    /* Pong is transmitted from can1 to can0. */

    /* Write pong. */
    frame.id = PONG_ID;
    frame.size = 0;
    BTASSERT(can_write(&can1, &frame) == sizeof(frame));

    /* Read pong. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can0, &frame) == sizeof(frame));
    BTASSERT(frame.id == PONG_ID);
    BTASSERT(frame.size == 0);

    BTASSERT(can_stop(&can0) == 0);
    BTASSERT(can_stop(&can1) == 0);

    return (0);
}

static int test_ping_pong_250k(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_250KBPS));
}

static int test_ping_pong_500k(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_500KBPS));
}

static int test_ping_pong_1000k(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_1000KBPS));
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_ping_pong_250k, "test_ping_pong_250k" },
        { test_ping_pong_500k, "test_ping_pong_500k" },
        { test_ping_pong_1000k, "test_ping_pong_1000k" },
        { NULL, NULL }
    };
    

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
