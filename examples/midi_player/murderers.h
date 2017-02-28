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
