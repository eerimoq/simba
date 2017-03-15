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

/* CAN receive buffer. */
static char can_rx_buf[256];

struct can_driver_t can;

int main()
{
    struct can_frame_t frame;
    int i;

    sys_start();

    /* Start the CAN device. */
    can_init(&can,
             &can_device[0],
             CAN_SPEED_500KBPS,
             can_rx_buf,
             sizeof(can_rx_buf));
    can_start(&can);

    std_printf(FSTR("CAN server started.\r\n"));

    while (1) {
        /* Read a frame from the CAN bus. */
        memset(&frame, 0, sizeof(frame));

        if (can_read(&can, &frame, sizeof(frame)) != sizeof(frame)) {
            std_printf(FSTR("Failed to read a CAN frame. Aborting.\r\n"));
            break;
        }

        std_printf(FSTR("Read frame.    extended: %d, id: 0x%x, size: %d, data:"),
                   frame.extended_frame,
                   frame.id,
                   frame.size);

        for (i = 0; i < frame.size; i++) {
            std_printf(FSTR(" 0x%02x"), frame.data.u8[i]);
        }

        std_printf("\r\n");

        /* Modify the read frame and write it to the CAN bus. */
        frame.id++;

        for (i = 0; i < frame.size; i++) {
            frame.data.u8[i]++;
        }

        std_printf(FSTR("Writing frame. extended: %d, id: 0x%x, size: %d, data:"),
                   frame.extended_frame,
                   frame.id,
                   frame.size);

        for (i = 0; i < frame.size; i++) {
            std_printf(FSTR(" 0x%02x"), frame.data.u8[i]);
        }

        std_printf("\r\n");

        if (can_write(&can, &frame, sizeof(frame)) != sizeof(frame)) {
            std_printf(FSTR("Failed to write a CAN frame. Aborting.\r\n"));
            break;
        }
    }

    return (0);
}
