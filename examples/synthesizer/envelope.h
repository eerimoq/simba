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
