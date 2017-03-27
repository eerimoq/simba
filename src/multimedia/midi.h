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

#ifndef __MULTIMEDIA_MIDI_H__
#define __MULTIMEDIA_MIDI_H__

#include "simba.h"

/* The MIDI baudrate. */
#define MIDI_BAUDRATE 31250

/* MIDI commands. */
#define MIDI_NOTE_OFF                 0x80
#define MIDI_NOTE_ON                  0x90
#define MIDI_POLYPHONIC_KEY_PRESSURE  0xa0
#define MIDI_CONTROL_CHANGE           0xb0
#define MIDI_PROGRAM_CHANGE           0xc0
#define MIDI_CHANNEL_PRESSURE         0xd0
#define MIDI_PITCH_BEND_CHANGE        0xe0
#define MIDI_SET_INTRUMENT            0xc0
#define MIDI_PERC                     0x99

#define MIDI_NOTE_MAX 128

/* Midi notees. */
#define MIDI_NOTE_A0      21
#define MIDI_NOTE_B0      23

#define MIDI_NOTE_C1      24
#define MIDI_NOTE_D1      26
#define MIDI_NOTE_E1      28
#define MIDI_NOTE_F1      29
#define MIDI_NOTE_G1      31
#define MIDI_NOTE_A1      33
#define MIDI_NOTE_B1      35

#define MIDI_NOTE_C2      36
#define MIDI_NOTE_D2      38
#define MIDI_NOTE_E2      40
#define MIDI_NOTE_F2      41
#define MIDI_NOTE_G2      43
#define MIDI_NOTE_A2      45
#define MIDI_NOTE_B2      47

#define MIDI_NOTE_C3      48
#define MIDI_NOTE_D3      50
#define MIDI_NOTE_E3      52
#define MIDI_NOTE_F3      53
#define MIDI_NOTE_G3      55
#define MIDI_NOTE_A3      57
#define MIDI_NOTE_B3      59

#define MIDI_NOTE_C4      60
#define MIDI_NOTE_D4      62
#define MIDI_NOTE_E4      64
#define MIDI_NOTE_F4      65
#define MIDI_NOTE_G4      67
#define MIDI_NOTE_A4      69
#define MIDI_NOTE_B4      71

#define MIDI_NOTE_C5      72
#define MIDI_NOTE_D5      74
#define MIDI_NOTE_E5      76
#define MIDI_NOTE_F5      77
#define MIDI_NOTE_G5      79
#define MIDI_NOTE_A5      81
#define MIDI_NOTE_B5      83

#define MIDI_NOTE_C6      84
#define MIDI_NOTE_D6      86
#define MIDI_NOTE_E6      88
#define MIDI_NOTE_F6      89
#define MIDI_NOTE_G6      91
#define MIDI_NOTE_A6      93
#define MIDI_NOTE_B6      95

#define MIDI_NOTE_C7      96
#define MIDI_NOTE_D7      98
#define MIDI_NOTE_E7     100
#define MIDI_NOTE_F7     101
#define MIDI_NOTE_G7     103
#define MIDI_NOTE_A7     105
#define MIDI_NOTE_B7     107

#define MIDI_NOTE_C8     108

/* General MIDI Percussion MIDI Map Table. */
#define MIDI_PERC_ACOUSTIC_BASS_DRUM 35
#define MIDI_PERC_BASS_DRUM_1        36
#define MIDI_PERC_SIDE_STICK         37
#define MIDI_PERC_ACOUSTIC_SNARE     38
#define MIDI_PERC_HAND_CLAP          39
#define MIDI_PERC_ELECTRIC_SNARE     40
#define MIDI_PERC_LOW_FLOOR_TOM      41
#define MIDI_PERC_CLOSED_HI_HAT      42
#define MIDI_PERC_HIGH_FLOOR_TOM     43
#define MIDI_PERC_PEDAL_HI_HAT       44
#define MIDI_PERC_LOW_TOM            45
#define MIDI_PERC_OPEN_HI_HAT        46
#define MIDI_PERC_LOW_MID_TOM        47
#define MIDI_PERC_HI_MID_TOM         48
#define MIDI_PERC_CRASH_CYMBAL_1     49
#define MIDI_PERC_HIGH_TOM           50
#define MIDI_PERC_RIDE_CYMBAL_1      51
#define MIDI_PERC_CHINESE_CYMBAL     52
#define MIDI_PERC_RIDE_BELL          53
#define MIDI_PERC_TAMBOURINE         54
#define MIDI_PERC_SPLASH_CYMBAL      55
#define MIDI_PERC_COWBELL            56
#define MIDI_PERC_CRASH_CYMBAL_2     57
#define MIDI_PERC_VIBRASLAP          58
#define MIDI_PERC_RIDE_CYMBAL_2      59
#define MIDI_PERC_HI_BONGO           60
#define MIDI_PERC_LOW_BONGO          61
#define MIDI_PERC_MUTE_HI_CONGA      62
#define MIDI_PERC_OPEN_HI_CONGA      63
#define MIDI_PERC_LOW_CONGA          64
#define MIDI_PERC_HIGH_TIMBALE       65
#define MIDI_PERC_LOW_TIMBALE        66
#define MIDI_PERC_HIGH_AGOGO         67
#define MIDI_PERC_LOW_AGOGO          68
#define MIDI_PERC_CABASA             69
#define MIDI_PERC_MARACAS            70
#define MIDI_PERC_SHORT_WHISTLE      71
#define MIDI_PERC_LONG_WHISTLE       72
#define MIDI_PERC_SHORT_GUIRO        73
#define MIDI_PERC_LONG_GUIRO         74
#define MIDI_PERC_CLAVES             75
#define MIDI_PERC_HI_WOOD_BLOCK      76
#define MIDI_PERC_LOW_WOOD_BLOCK     77
#define MIDI_PERC_MUTE_CUICA         78
#define MIDI_PERC_OPEN_CUICA         79
#define MIDI_PERC_MUTE_TRIANGLE      80
#define MIDI_PERC_OPEN_TRIANGLE      81

/**
 * Get the frequency for given note.
 *
 * @param[in] note MIDI note.
 *
 * @return Note frequency.
 */
float midi_note_to_frequency(int note);

#endif
