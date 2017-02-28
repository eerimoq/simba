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

/* CAN receive buffer. */
static char can_rx_buf[256];
struct can_driver_t can;

static int test_ping_pong_500k(struct harness_t *harness_p)
{
    int i, j;
    struct can_frame_t frame;

    BTASSERT(can_init(&can,
                      &can_device[0],
                      CAN_SPEED_500KBPS,
                      can_rx_buf,
                      sizeof(can_rx_buf)) == 0);
    BTASSERT(can_start(&can) == 0);

    for (i = 0; i <= 8; i++) {
        /* Write a frame to the server ... */
        memset(&frame, 0, sizeof(frame));
        frame.id = 0x7a0 + 2 * i;
        frame.extended_frame = 0;
        frame.size = i;

        for (j = 0; j < i; j++) {
            frame.data.u8[j] = j + i;
        }

        std_printf(FSTR("Writing frame. id: 0x%x, size: %d, data:"),
                   frame.id,
                   frame.size);

        for (j = 0; j < i; j++) {
            std_printf(FSTR(" 0x%02x"), frame.data.u8[j]);
        }

        std_printf("\r\n");

        BTASSERT(can_write(&can, &frame, sizeof(frame)) == sizeof(frame));

        /* ... and read the response. */
        memset(&frame, 0, sizeof(frame));
        BTASSERT(can_read(&can, &frame, sizeof(frame)) == sizeof(frame));

        std_printf(FSTR("Read frame.    id: 0x%x, size: %d, data:"),
                   frame.id,
                   frame.size);

        for (j = 0; j < i; j++) {
            std_printf(FSTR(" 0x%02x"), frame.data.u8[j]);
        }

        BTASSERT(frame.id == 0x7a0 + 2 * i + 1);
        BTASSERT(frame.extended_frame == 0);
        BTASSERT(frame.size == i);

        for (j = 0; j < i; j++) {
            BTASSERT(frame.data.u8[j] == j + i + 1);
        }

        std_printf("\r\n");
    }

    BTASSERT(can_stop(&can) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_ping_pong_500k, "test_ping_pong_500k" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
