/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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
