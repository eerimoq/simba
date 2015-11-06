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
static char can0_rx_buf[256];
static char can1_rx_buf[256];

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
    BTASSERT(can_write(&can0, &frame, sizeof(frame)) == sizeof(frame));

    /* Read ping. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can1, &frame, sizeof(frame)) == sizeof(frame));
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
    BTASSERT(can_write(&can1, &frame, sizeof(frame)) == sizeof(frame));

    /* Read pong. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can0, &frame, sizeof(frame)) == sizeof(frame));
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
    int id = 0;
    int i = 0;

    while (1) {
        BTASSERT(can_read(&can1, &frame, sizeof(frame)) == sizeof(frame));
        BTASSERT(frame.id == id, FSTR(" i = %d, frame.id = %d, id = %d\r\n"), i, frame.id, id);
        i++;

        if (i == 10000) {
            i = 0;
            id = 0;
        } else {
            id++;
            id %= 0x800;
        }
    }

    return (NULL);
}

static int test_max_throughput(struct harness_t *harness_p)
{
    int i, j, k, id;
    struct can_frame_t frames[8];
    struct time_t start_time, stop_time;
    float frames_per_second;
    float bits_per_second;
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
        std_printf(FSTR("Writing 10000 frames with data size %d.\r\n"),
                   sizes[i]);
        time_get(&start_time);
        id = 0;

        for (k = 0; k < membersof(frames); k++) {
            frames[k].extended_id = 0;
            frames[k].size = sizes[i];
            memset(frames[k].data.u8, 0xaa, frames[k].size);
        }

        for (j = 0; j < 1250; j++) {
            for (k = 0; k < membersof(frames); k++) {
                frames[k].id = id;
                id++;
            }

            BTASSERT(can_write(&can0, frames, sizeof(frames)) == sizeof(frames));
        }

        time_get(&stop_time);

        elapsed_time = (stop_time.seconds - start_time.seconds) / (float)SYS_TICK_FREQUENCY;

        frames_per_second = (10000.0 / elapsed_time);
        bits_per_second = (frames_per_second *
                           (1 + 11 + 1 + 1 + 1 + 4 + 15 + 1 + 1 + 1 + 7
                            + sizes[i] * 8));
        data_bits_per_second = (frames_per_second * sizes[i] * 8);

        std_printf(FSTR("elapsed_time = %f, %d frames/s, %d bits/s, %d data bits/s\r\n"),
                   elapsed_time,
                   (int)frames_per_second,
                   (int)bits_per_second,
                   (int)data_bits_per_second);
        BTASSERT(COUNTER(can_rx_channel_overflow) == 0);
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
