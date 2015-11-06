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

/* Throughput can frame id. */
#define THROUGHPUT_ID 0x25

/* CAN receive buffers. */
static char can0_rx_buf[128];
static char can1_rx_buf[128];

struct can_driver_t can0;
struct can_driver_t can1;

static int start(uint32_t speed)
{
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

    return (0);
}

static int stop()
{
    BTASSERT(can_stop(&can0) == 0);
    BTASSERT(can_stop(&can1) == 0);

    return (0);
}

static int test_ping_pong(uint32_t speed, int extended_id)
{
    struct can_frame_t frame;

    start(speed);

    /* Ping is transmitted from can0 to can1. */

    /* Write ping. */
    frame.id = PING_ID;
    frame.id <<= (10 * extended_id);
    frame.extended_id = extended_id;
    frame.size = 1;
    frame.data.u8[0] = 0xfe;
    BTASSERT(can_write(&can0, &frame) == sizeof(frame));

    /* Read ping. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can1, &frame) == sizeof(frame));
    frame.id >>= (10 * extended_id);
    BTASSERT(frame.id == PING_ID);
    BTASSERT(frame.extended_id == extended_id);
    BTASSERT(frame.size == 1);
    BTASSERT(frame.data.u8[0] == 0xfe);

    /* Pong is transmitted from can1 to can0. */

    /* Write pong. */
    frame.id = PONG_ID;
    frame.id <<= (10 * extended_id);
    frame.extended_id = extended_id;
    frame.size = 0;
    BTASSERT(can_write(&can1, &frame) == sizeof(frame));

    /* Read pong. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can0, &frame) == sizeof(frame));
    frame.id >>= (10 * extended_id);
    BTASSERT(frame.id == PONG_ID);
    BTASSERT(frame.extended_id == extended_id);
    BTASSERT(frame.size == 0);

    stop();

    return (0);
}

static int test_ping_pong_250k(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_250KBPS, 0));
}

static int test_ping_pong_500k(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_500KBPS, 0));
}

static int test_ping_pong_1000k(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_1000KBPS, 0));
}

static int test_ping_pong_1000k_extended_id(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_1000KBPS, 1));
}

extern long long COUNTER(can_rx_channel_overflow);

static THRD_STACK(rx_thrd_stack, 1024);

static void *rx_thrd(void *arg_p)
{
    struct can_frame_t frame;

    while (1) {
        BTASSERT(can_read(&can1, &frame) == sizeof(frame));
        BTASSERT(frame.id == THROUGHPUT_ID);
    }

    return (NULL);
}

static int test_max_throughput(struct harness_t *harness_p)
{
    struct can_frame_t frame;
    int i, j;
    struct time_t start_time, stop_time;
    float frames_per_second;
    float data_bits_per_second;
    int sizes[] = {0, 4, 8};
    float elapsed_time;

    thrd_spawn(rx_thrd,
               NULL,
               -1,
               rx_thrd_stack,
               sizeof(rx_thrd_stack));

    start(CAN_SPEED_1000KBPS);

    for (i = 0; i < membersof(sizes); i++) {
        frame.id = THROUGHPUT_ID;
        frame.extended_id = 0;
        frame.size = sizes[i];

        std_printf(FSTR("Writing 10000 frames with data size %d.\r\n"),
                   frame.size);

        time_get(&start_time);

        for (j = 0; j < 10000; j++) {
            BTASSERT(can_write(&can0, &frame) == sizeof(frame));
        }

        time_get(&stop_time);

        elapsed_time = (stop_time.seconds - start_time.seconds) / (float)SYS_TICK_FREQUENCY;

        frames_per_second = (10000.0 / elapsed_time);
        data_bits_per_second = (frames_per_second * frame.size * 8);

        std_printf(FSTR("elapsed_time = %f, frames_per_second = %f, data_bits_per_second = %f\r\n"),
                   elapsed_time,
                   frames_per_second,
                   data_bits_per_second);
        std_printf(FSTR("can rx overflow counter = %lu\r\n"), (uint32_t)COUNTER(can_rx_channel_overflow));
        COUNTER(can_rx_channel_overflow) = 0;
    }

    stop();

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_ping_pong_250k, "test_ping_pong_250k" },
        { test_ping_pong_500k, "test_ping_pong_500k" },
        { test_ping_pong_1000k, "test_ping_pong_1000k" },
        { test_ping_pong_1000k_extended_id, "test_ping_pong_1000k_extended_id" },
        { test_max_throughput, "test_max_throughput" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
