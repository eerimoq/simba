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

        std_printf(FSTR("Read frame.    id: 0x%x, size: %d, data:"),
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

        std_printf(FSTR("Writing frame. id: 0x%x, size: %d, data:"),
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
