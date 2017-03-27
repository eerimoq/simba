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

static THRD_STACK(rx_thrd_stack, 1024);

/**
 * Initialize and start the two CAN controllers, CAN0 and CAN1, with
 * given baudrate.
 *
 * @apram[in] speed Baudrate to use.
 */
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

/**
 * Stop the two CAN controllers, CAN0 and CAN1.
 */
static int stop()
{
    BTASSERT(can_stop(&can0) == 0);
    BTASSERT(can_stop(&can1) == 0);

    return (0);
}

/**
 * Thread that receives CAN frames on controller CAN1 and verifies its
 * content.
 *
 * This thread is used by the test `test_max_throughput()`.
 */
static void *rx_thrd(void *arg_p)
{
    struct can_frame_t frame;
    int id = 0;
    int i = 0;

    thrd_set_name("rx_thrd");

    while (1) {
        BTASSERTN(can_read(&can1, &frame, sizeof(frame)) == sizeof(frame));
        BTASSERTN(frame.id == id,
                  FSTR(" i = %d, frame.id = %d, id = %d\r\n"),
                  i,
                  frame.id,
                  id);
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

/**
 * Ping-pong test. Starts the CAN controllers with given speed, sends
 * a few CAN frames and then stops the CAN controllers.
 *
 * @param[in] speed Badrate to use.
 *
 * @param[in] extended_frame Use extendend CAN frame id if true(1),
 *                           otherwise use standard CAN id.
 */
static int test_ping_pong(uint32_t speed, int extended_frame)
{
    struct can_frame_t frame;

    start(speed);

    /* Ping is transmitted from can0 to can1. */

    /* Write ping. */
    frame.id = PING_ID;
    frame.id <<= (10 * extended_frame);
    frame.extended_frame = extended_frame;
    frame.size = 1;
    frame.data.u8[0] = 0xfe;
    BTASSERT(can_write(&can0, &frame, sizeof(frame)) == sizeof(frame));

    /* Read ping. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can1, &frame, sizeof(frame)) == sizeof(frame));
    frame.id >>= (10 * extended_frame);
    BTASSERT(frame.id == PING_ID);
    BTASSERT(frame.extended_frame == extended_frame);
    BTASSERT(frame.size == 1);
    BTASSERT(frame.data.u8[0] == 0xfe);

    /* Pong is transmitted from can1 to can0. */

    /* Write pong. */
    frame.id = PONG_ID;
    frame.id <<= (10 * extended_frame);
    frame.extended_frame = extended_frame;
    frame.size = 0;
    BTASSERT(can_write(&can1, &frame, sizeof(frame)) == sizeof(frame));

    /* Read pong. */
    memset(&frame, 0, sizeof(frame));
    BTASSERT(can_read(&can0, &frame, sizeof(frame)) == sizeof(frame));
    frame.id >>= (10 * extended_frame);
    BTASSERT(frame.id == PONG_ID);
    BTASSERT(frame.extended_frame == extended_frame);
    BTASSERT(frame.size == 0);

    stop();

    return (0);
}

/**
 * Ping-pong tests with differnet configurations.
 */
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

static int test_ping_pong_1000k_extended_frame(struct harness_t *harness_p)
{
    return (test_ping_pong(CAN_SPEED_1000KBPS, 1));
}

/**
 * Test the maximum throughput on the CAN bus.
 */
static int test_max_throughput(struct harness_t *harness_p)
{
    int i, j, k, id;
    struct can_frame_t frames[8];
    struct time_t start_time, stop_time, diff_time;
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

        /* Prepare the array of frames. */
        for (k = 0; k < membersof(frames); k++) {
            frames[k].extended_frame = 0;
            frames[k].size = sizes[i];
            memset(frames[k].data.u8, 0xaa, frames[k].size);
        }

        /* Write the array of frames to the hardware. */
        for (j = 0; j < 10000 / membersof(frames); j++) {
            for (k = 0; k < membersof(frames); k++) {
                frames[k].id = id;
                id++;
            }

            BTASSERT(can_write(&can0, frames, sizeof(frames))
                     == sizeof(frames));
        }

        time_get(&stop_time);

        /* Statistics. */
        time_subtract(&diff_time, &stop_time, &start_time);
        elapsed_time = (diff_time.seconds
                        + diff_time.nanoseconds / 1000000000.0);

        frames_per_second = (10000.0 / elapsed_time);
        bits_per_second = (frames_per_second *
                           (1 + 11 + 1 + 1 + 1 + 4 + 15 + 1 + 1 + 1 + 7
                            + sizes[i] * 8));
        data_bits_per_second = (frames_per_second * sizes[i] * 8);

        std_printf(FSTR("elapsed time = %f s, %d frames/s, %d bits/s, "
                        "%d data bits/s\r\n"),
                   elapsed_time,
                   (int)frames_per_second,
                   (int)bits_per_second,
                   (int)data_bits_per_second);
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
        { test_ping_pong_1000k_extended_frame, "test_ping_pong_1000k_extended_frame" },
        { test_max_throughput, "test_max_throughput" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
