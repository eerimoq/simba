/**
 * @file oscillator.h
 * @version 0.7.0
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

#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

#include "simba.h"
#include "types.h"

struct oscillator_t {
    float frequency;
    int sample_rate;
    int sample_counter;
    q20_11_t phase; /* Current phase in the waveform. */
    q20_11_t phase_increment; /* The number of steps to increment the
                               * phase to get the next sample in the
                               * waveform. */
    struct {
        float value;
        q20_11_t phase_increment;
    } vibrato;
    struct {
        int length; /* Length (number of samples) in the waveform
                     * cycle. */
        int32_t *buf_p; /* Pointer to the waveform cycle. */
    } waveform;
};

/**
 * Initialize a oscillator with given waveform and frequency.
 *
 * @param[in] self_p The oscillator to initialize.
 * @param[in] waveform_p Waveform to use.
 * @param[in] length Length of the waveform.
 * @param[in] frequency Oscillator frequency.
 * @param[in] sample_rate Sample rate.
 *
 * @return zero(0) or negative error code.
 */
int oscillator_init(struct oscillator_t *self_p,
                    int32_t *waveform_p,
                    size_t length,
                    float frequency,
                    float vibrato,
                    int sample_rate);

/**
 * Set the frequency of given oscillator.
 *
 * @param[in] self_p The oscillator.
 * @param[in] frequency Frequency to set.
 *
 * @return zero(0) or negative error code.
 */
int oscillator_set_frequency(struct oscillator_t *self_p,
                             float frequency);

/**
 * Set the vibrato of given oscillator.
 *
 * @param[in] self_p The oscillator.
 * @param[in] vibrato Vibrato to set.
 *
 * @return zero(0) or negative error code.
 */
int oscillator_set_vibrato(struct oscillator_t *self_p,
                           float vibrato);

/**
 * Read the next samples from the oscillator.
 *
 * @param[in] self_p The oscillator.
 * @param[out] samples_p Read samples.
 * @param[in] length Number of samples to read.
 *
 * @return zero(0) or negative error code.
 */
int oscillator_read(struct oscillator_t *self_p,
                    int32_t *samples_p,
                    size_t length);

#endif
