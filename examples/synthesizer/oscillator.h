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
