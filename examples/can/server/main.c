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

#include "simba.h"

static char qinbuf[32];
static struct uart_driver_t uart;

int main()
{
    struct canif_frame_t frame;
    struct mcp2515_driver_t mcp2515;
    struct pin_driver_t pin[3];

    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    /* Initialize led pins. */
    pin_init(&pin[0], &pin_d7_dev, PIN_OUTPUT);
    pin_init(&pin[1], &pin_d8_dev, PIN_OUTPUT);
    pin_init(&pin[2], &pin_d9_dev, PIN_OUTPUT);

    while (1) {
        mcp2515_read(&mcp2515, CANIF_MAILBOX_ANY, &frame);
        std_fprintf(&uart.chout,
                    FSTR("id = %d, data = 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\r\n"),
                    (int)frame.id,
                    (int)frame.data[0],
                    (int)frame.data[1],
                    (int)frame.data[2],
                    (int)frame.data[3],
                    (int)frame.data[4],
                    (int)frame.data[5],
                    (int)frame.data[6],
                    (int)frame.data[7]);

        /* Upadte LED. */
        pin_write(&pin[0], (frame.data[0] >> 0) & 0x1);
        pin_write(&pin[1], (frame.data[0] >> 1) & 0x1);
        pin_write(&pin[2], (frame.data[0] >> 2) & 0x1);
    }

    return (0);
}
