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
static struct uart_driver_t uart_midi;

/* The MIDI baudrate. */
#define MIDI_BAUDRATE 31250

/* MIDI commands. */
#define MIDI_NOTE_OFF      0x80
#define MIDI_NOTE_ON       0x90
#define MIDI_SET_INTRUMENT 0xc0
#define MIDI_DRUM          0x99

/* Player operation codes. */
#define COMMAND  0
#define DELAY    1

struct operation_t {
    int code;
    size_t size;
    uint8_t buf[4];
};

/* The song Spanien with notes and drums. */
static struct operation_t spanien[] = {
    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x23, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* C */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c } },
    /* D */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 2, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 2 } },

    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x26, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* E */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 4 } },

    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x23, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* C */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c } },
    /* D */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 2, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 2 } },

    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x26, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* E */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 4 } },

    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x23, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* D */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 2, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 2 } },
    /* D */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 2, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 2 } },

    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x26, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* D */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 2, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 2 } },
    /* D */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c + 2, 0x45 } },
    { DELAY, 1 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c + 2 } },

    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x23, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* C */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c } },

    /* Drum */
    { COMMAND, 3, { MIDI_DRUM, 0x26, 0x45 } },
    /* Hi Hat */
    { COMMAND, 3, { MIDI_DRUM, 0x2a, 0x45 } },
    /* Cymbal */
    { COMMAND, 3, { MIDI_DRUM, 0x31, 0x55 } },
    /* C */
    { COMMAND, 3, { MIDI_NOTE_ON, 0x3c, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 2, { MIDI_NOTE_OFF, 0x3c } },

    /* End of the song. */
    { DELAY, 3 },
};

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

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    uart_init(&uart_midi, &uart_device[1], MIDI_BAUDRATE, NULL, 0);
    uart_start(&uart_midi);

    sys_set_stdout(&uart.chout);

    std_printf(sys_get_info());
}

int main()
{
    int i, j;

    init();

    while (1) {
        for (i = 0; i < 128; i++) {
            std_printf(FSTR("instrument = %d\r\n"), i);
            set_instrument(0, i);

            for (j = 0; j < membersof(spanien); j++) {
                switch (spanien[j].code) {

                case COMMAND:
                    uart_write(&uart_midi, spanien[j].buf, spanien[j].size);
                    break;

                case DELAY:
                    thrd_usleep(300000 * spanien[j].size);
                    break;

                default:
                    std_printf(FSTR("bad op code %d\r\n"), spanien[i].code);
                    break;
                }
            }
        }
    }

    return (0);
}
