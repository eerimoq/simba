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

struct operation_t {
    int code;
    size_t size;
    uint8_t buf[4];
};

/* The song Murderers. */
static struct operation_t murderers[] = {
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_D4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_F5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_D4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_F5 + 1, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G5 + 1, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_F4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_A5, 0x45 } },
    { DELAY, 4 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_F4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_A5, 0 } },

    { DELAY, 4 },


    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_D4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_F5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_D4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_F5 + 1, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G5 + 1, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_B5, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_B5, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_F4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_A5, 0x45 } },
    { DELAY, 4 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_F4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_A5, 0 } },

    { DELAY, 4 },


    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_D4 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E6, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_D4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E6, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_B5, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_B5, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E4, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G5 + 1, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_F4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_A5, 0x45 } },
    { DELAY, 4 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_F4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_A5, 0 } },

    { DELAY, 4 },


    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_D4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_F5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_D4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_F5 + 1, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G5 + 1, 0 } },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_C4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E5 + 1, 0x45 } },
    { DELAY, 4 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_C4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E5 + 1, 0 } },

    { DELAY, 4 }
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

    uart_init(&uart_midi, &uart_device[0], MIDI_BAUDRATE, NULL, 0);
    uart_start(&uart_midi);
}

int main()
{
    int i, j;

    init();

    while (1) {
        for (i = 0; i < 128; i++) {
            std_printf(FSTR("instrument = %d\r\n"), i);
            set_instrument(0, i);

            for (j = 0; j < membersof(murderers); j++) {
                switch (murderers[j].code) {

                case COMMAND:
                    uart_write(&uart_midi,
                               murderers[j].buf,
                               murderers[j].size);
                    break;

                case DELAY:
                    thrd_usleep(200000 * murderers[j].size);
                    break;

                default:
                    std_printf(FSTR("bad op code %d\r\n"),
                               murderers[i].code);
                    break;
                }
            }
        }
    }

    return (0);
}
