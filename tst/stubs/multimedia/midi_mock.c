/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "midi_mock.h"

int mock_write_midi_note_to_frequency(int note,
                                      float res)
{
    harness_mock_write("midi_note_to_frequency(note)",
                       &note,
                       sizeof(note));

    harness_mock_write("midi_note_to_frequency(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

float __attribute__ ((weak)) STUB(midi_note_to_frequency)(int note)
{
    float res;

    harness_mock_assert("midi_note_to_frequency(note)",
                        &note,
                        sizeof(note));

    harness_mock_read("midi_note_to_frequency(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
