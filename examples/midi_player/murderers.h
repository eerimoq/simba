/**
 * @file murderers.h
 * @version 3.1.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

#ifndef __SONG_H__
#define __SONG_H__

/* The song Murderers. */
static const FAR struct operation_t song[] = {

    { PACE, 200 },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_D4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_F5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_D4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_F5 + 1, 0 } },
    { SPACING },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G5 + 1, 0 } },
    { SPACING },

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
    { SPACING },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G5 + 1, 0 } },
    { SPACING },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_B5, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_B5, 0 } },
    { SPACING },

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
    { SPACING },

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
    { SPACING },

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
    { SPACING },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E4, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_G5 + 1, 0x45 } },
    { DELAY, 2 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E4, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_G5 + 1, 0 } },
    { SPACING },

    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_C4 + 1, 0x45 } },
    { COMMAND, 3, { MIDI_NOTE_ON, MIDI_NOTE_E5 + 1, 0x45 } },
    { DELAY, 4 },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_C4 + 1, 0 } },
    { COMMAND, 3, { MIDI_NOTE_OFF, MIDI_NOTE_E5 + 1, 0 } },

    { DELAY, 4 }
};

#endif
