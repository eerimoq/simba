/**
 * @file envelope.h
 * @version 7.0.0
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

#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__

#include "simba.h"
#include "types.h"

/* Envelope phases. */
#define ENVELOPE_PHASE_ATTACK  0
#define ENVELOPE_PHASE_DECAY   1
#define ENVELOPE_PHASE_SUSTAIN 2
#define ENVELOPE_PHASE_RELEASE 3

struct envelope_amplitude_sample_t {
    int32_t factor;
    int32_t shift;
};

struct envelope_phase_t {
    q11_20_t position; /* The current position in the amplitude
                        * curve. */
    q11_20_t increment; /* The number of steps to take in the
                         * amplitude curve for each sample. */
    size_t step;
    size_t steps_max; /* Number of steps to take in the phase before
                       * going to the next phase. */
    struct envelope_amplitude_sample_t *amplitude_curve_p; /* The amplitude curve. */
};

struct envelope_t {
    int phase; /* The current phase in the envelope. */
    int released; /* Set when the note has been released. */
    struct envelope_phase_t attack;
    struct envelope_phase_t decay;
    struct {
        struct envelope_amplitude_sample_t gain;
    } sustain;
    struct envelope_phase_t release;
};

/**
 * Initialize a envelope with given configuration.
 *
 * @param[in] self_p The envelope to initialize.
 * @param[in] waveform_p Waveform to use.
 * @param[in] length Length of the waveform.
 * @param[in] frequency Envelope frequency.
 * @param[in] sample_rate Sample rate.
 *
 * @return zero(0) or negative error code.
 */
int envelope_init(struct envelope_t *self_p,
                  long attack,
                  long decay,
                  long release);

/**
 * Start the release phase.
 *
 * @param[in] self_p The envelope.
 *
 * @return zero(0) or negative error code.
 */
int envelope_set_phase_release(struct envelope_t *self_p);

/**
 * Apply the envelope effects on given samples.
 *
 * @param[in] self_p The envelope.
 * @param[in,out] samples_p Read samples.
 * @param[in] length Number of samples to process.
 *
 * @return zero(0) or negative error code.
 */
size_t envelope_apply(struct envelope_t *self_p,
                      int32_t *samples_p,
                      size_t length);

#endif
