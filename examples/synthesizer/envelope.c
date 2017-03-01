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

/**
 * The envelope amplitude over time.
 *
 *    ATTACK  DECAY   SUSTAIN                    RELEASE
 *
 *  1        /\
 *          /    \
 *         /        \
 *        /           +-------------------------+
 *       /                                       \
 *      /                                         \
 *     /                                           \
 *  0 /                                             \
 */

#include "envelope.h"
#include "types.h"

static struct envelope_amplitude_sample_t attack_curve[] = {
    {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7},
    {8, 7}, {9, 7}, {10, 7}, {11, 7}, {12, 7}, {13, 7}, {14, 7}, {15, 7},
    {16, 7}, {17, 7}, {18, 7}, {19, 7}, {20, 7}, {21, 7}, {22, 7}, {23, 7},
    {24, 7}, {25, 7}, {26, 7}, {27, 7}, {28, 7}, {29, 7}, {30, 7}, {31, 7},
    {32, 7}, {33, 7}, {34, 7}, {35, 7}, {36, 7}, {37, 7}, {38, 7}, {39, 7},
    {40, 7}, {41, 7}, {42, 7}, {43, 7}, {44, 7}, {45, 7}, {46, 7}, {47, 7},
    {48, 7}, {49, 7}, {50, 7}, {51, 7}, {52, 7}, {53, 7}, {54, 7}, {55, 7},
    {56, 7}, {57, 7}, {58, 7}, {59, 7}, {60, 7}, {61, 7}, {62, 7}, {63, 7}
};

static struct envelope_amplitude_sample_t decay_curve[] = {
    {63, 7}, {62, 7}, {61, 7}, {60, 7}, {59, 7}, {58, 7}, {57, 7}, {56, 7},
    {55, 7}, {54, 7}, {53, 7}, {52, 7}, {51, 7}, {50, 7}, {49, 7}, {48, 7},
    {47, 7}, {46, 7}, {45, 7}, {44, 7}, {43, 7}, {42, 7}, {41, 7}, {40, 7},
    {39, 7}, {38, 7}, {37, 7}, {36, 7}, {35, 7}, {34, 7}, {33, 7}, {32, 7},
    {31, 7}, {30, 7}, {29, 7}, {28, 7}, {27, 7}, {26, 7}, {25, 7}, {24, 7},
    {23, 7}, {22, 7}, {21, 7}, {20, 7}, {19, 7}, {18, 7}, {17, 7}, {16, 7},
    {15, 7}, {14, 7}, {13, 7}, {12, 7}, {11, 7}, {10, 7}, {9, 7}, {8, 7},
    {7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}
};

static struct envelope_amplitude_sample_t release_curve[] = {
    {63, 7}, {62, 7}, {61, 7}, {60, 7}, {59, 7}, {58, 7}, {57, 7}, {56, 7},
    {55, 7}, {54, 7}, {53, 7}, {52, 7}, {51, 7}, {50, 7}, {49, 7}, {48, 7},
    {47, 7}, {46, 7}, {45, 7}, {44, 7}, {43, 7}, {42, 7}, {41, 7}, {40, 7},
    {39, 7}, {38, 7}, {37, 7}, {36, 7}, {35, 7}, {34, 7}, {33, 7}, {32, 7},
    {31, 7}, {30, 7}, {29, 7}, {28, 7}, {27, 7}, {26, 7}, {25, 7}, {24, 7},
    {23, 7}, {22, 7}, {21, 7}, {20, 7}, {19, 7}, {18, 7}, {17, 7}, {16, 7},
    {15, 7}, {14, 7}, {13, 7}, {12, 7}, {11, 7}, {10, 7}, {9, 7}, {8, 7},
    {7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}
};

static size_t phase_process(struct envelope_phase_t *phase_p,
                            int32_t *samples_p,
                            size_t length)
{
    int i;
    int index;
    size_t left = (phase_p->steps_max - phase_p->step);

    if (left < length) {
        length = left;
    }

    for (i = 0; i < length; i++) {
        phase_p->position += phase_p->increment;
        index = (uint32_t)Q11_20_TO_INT(phase_p->position);
        samples_p[i] = ((samples_p[i] * phase_p->amplitude_curve_p[index].factor)
                        >> phase_p->amplitude_curve_p[index].shift);
    }

    phase_p->step += length;

    return (length);
}

static int phase_is_complete(struct envelope_phase_t *phase_p)
{
    return (phase_p->step == phase_p->steps_max);
}

int envelope_init(struct envelope_t *self_p,
                  long attack,
                  long decay,
                  long release)
{
    self_p->phase = ENVELOPE_PHASE_ATTACK;
    self_p->released = 0;

    self_p->attack.position = FLOAT_TO_Q11_20(0.0);
    self_p->attack.increment = FLOAT_TO_Q11_20((float)membersof(attack_curve) / attack);
    self_p->attack.step = 0;
    self_p->attack.steps_max = attack;
    self_p->attack.amplitude_curve_p = attack_curve;

    self_p->decay.position = FLOAT_TO_Q11_20(0.0);
    self_p->decay.increment = FLOAT_TO_Q11_20((float)membersof(decay_curve) / decay);
    self_p->decay.step = 0;
    self_p->decay.steps_max = decay;
    self_p->decay.amplitude_curve_p = decay_curve;

    self_p->sustain.gain.factor = 1;
    self_p->sustain.gain.shift = 0;

    self_p->release.position = FLOAT_TO_Q11_20(0.0);
    self_p->release.increment = FLOAT_TO_Q11_20((float)membersof(release_curve) / release);
    self_p->release.step = 0;
    self_p->release.steps_max = release;
    self_p->release.amplitude_curve_p = release_curve;

    return (0);
}

int envelope_set_phase_release(struct envelope_t *self_p)
{
    self_p->released = 1;

    return (0);
}

size_t envelope_apply(struct envelope_t *self_p,
                      int32_t *samples_p,
                      size_t length)
{
    int i;
    size_t size = 0;

    /* Attack phase. */
    if (self_p->phase == ENVELOPE_PHASE_ATTACK) {
        size += phase_process(&self_p->attack,
                              &samples_p[0],
                              length);

        if (phase_is_complete(&self_p->attack)) {
            self_p->phase = ENVELOPE_PHASE_DECAY;
        }
    }

    /* Decay phase. */
    if (self_p->phase == ENVELOPE_PHASE_DECAY) {
        size += phase_process(&self_p->decay,
                              &samples_p[size],
                              length - size);

        if (phase_is_complete(&self_p->decay)) {
            self_p->phase = ENVELOPE_PHASE_SUSTAIN;
        }
    }

    /* Sustain phase. */
    if (self_p->phase == ENVELOPE_PHASE_SUSTAIN) {
        if (self_p->released == 0) {
            for (i = size; i < length; i++) {
                samples_p[i] = ((samples_p[i] * self_p->sustain.gain.factor)
                                >> self_p->sustain.gain.shift);
            }

            size = length;
        } else {
            self_p->phase = ENVELOPE_PHASE_RELEASE;
        }
    }

    /* Release phase. */
    if (self_p->phase == ENVELOPE_PHASE_RELEASE) {
        size += phase_process(&self_p->release,
                              &samples_p[size],
                              length - size);

        /* Clear all samples after the envelope. */
        for (i = size; i < length; i++) {
            samples_p[i] = 0;
        }
    }

    return (size);
}
