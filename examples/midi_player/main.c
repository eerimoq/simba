/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015, Erik Moqvist
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

static struct uart_driver_t uart_midi;

/* Player operation codes. */
#define COMMAND  0
#define DELAY    1
#define SPACING  2
#define PACE     3

struct operation_t {
    int code;
    size_t size;
    uint8_t buf[4];
};

//#include "murderers.h"
#include "christmas.h"

static int set_instrument(int channel, int instrument)
{
    uint8_t message[2];

    message[0] = (MIDI_SET_INTRUMENT | channel);
    message[1] = instrument;

    return (uart_write(&uart_midi,
                       message,
                       sizeof(message)) != sizeof(message));
}

static void init(void)
{
    sys_start();
    uart_module_init();

    uart_init(&uart_midi, &uart_device[0], MIDI_BAUDRATE, NULL, 0);
    uart_start(&uart_midi);
}

int main()
{
    int i, j;
    struct operation_t operation;
    unsigned long pace = 200;

    init();

    while (1) {
        for (i = 0; i < 128; i++) {
            std_printf(FSTR("instrument = %d\r\n"), i);
            set_instrument(0, i);

            for (j = 0; j < membersof(song); j++) {
                operation = song[j];

                switch (operation.code) {

                case COMMAND:
                    uart_write(&uart_midi,
                               operation.buf,
                               operation.size);
                    break;

                case DELAY:
                    thrd_usleep(pace * operation.size);
                    break;

                case SPACING:
                    thrd_usleep(5000);
                    break;

                case PACE:
                    pace = 1000UL * operation.size;
                    break;

                default:
                    std_printf(FSTR("bad op code %d\r\n"),
                               operation.code);
                    break;
                }
            }
        }
    }

    return (0);
}
