/**
 * @file note.h
 * @version 0.6.0
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
