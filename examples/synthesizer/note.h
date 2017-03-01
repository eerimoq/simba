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

#ifndef __NOTE_H__
#define __NOTE_H__

#include "simba.h"
#include "oscillator.h"
#include "envelope.h"

struct note_t {
    int note;
    struct oscillator_t oscillator;
    struct envelope_t envelope;
};

/**
 * Initialize a note with given configuration.
 *
 * @param[in] self_p The note to initialize.
 * @param[in] waveform_p Waveform to use.
 * @param[in] length Length of the waveform.
 * @param[in] frequency Note frequency.
 * @param[in] sample_rate Sample rate.
 *
 * @return zero(0) or negative error code.
 */
int note_init(struct note_t *self_p,
              int note,
              int32_t *waveform_p,
              size_t length,
              float frequency,
              float vibrato,
              long attack,
              long decay,
              long release,
              int sample_rate);

/**
 * Start given note.
 *
 * @param[in] self_p The note to initialize.
 *
 * @return zero(0) or negative error code.
 */
int note_start(struct note_t *self_p);

/**
 * Stop given note.
 *
 * @param[in] self_p The note to initialize.
 *
 * @return zero(0) or negative error code.
 */
int note_stop(struct note_t *self_p);

#endif
