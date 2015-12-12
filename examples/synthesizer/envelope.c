/**
 * @file envelope.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015, Erik Moqvist
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

static q20_11_t rising_curve[] = {
    1
};

static q20_11_t falling_curve[] = {
    1
};

static size_t phase_process(struct envelope_phase_t *phase_p,
                            uint32_t *samples_p,
                            size_t length)
{
    int i;
    int index;
    size_t left = (phase_p->steps_max - phase_p->step);

    if (left < length) {
        length = left;
    }

    /* for (i = 0; i < length; i++) { */
    /*     phase_p->position += phase_p->increment; */
    /*     index = (uint32_t)Q20_11_TO_INT(phase_p->position); */
    /*     samples_p[i] = phase_p->amplitude_curve_p[index]; */
    /* } */

    phase_p->step += length;

    return (length);
}

int envelope_init(struct envelope_t *self_p)
{
    self_p->phase = ENVELOPE_PHASE_ATTACK;
    self_p->released = 0;

    self_p->attack.position = 0;
    self_p->attack.increment = 1;
    self_p->attack.step = 0;
    self_p->attack.steps_max = 1;
    self_p->attack.amplitude_curve_p = rising_curve;

    self_p->decay.position = 0;
    self_p->decay.increment = 1;
    self_p->decay.step = 0;
    self_p->decay.steps_max = 1;
    self_p->decay.amplitude_curve_p = rising_curve;

    self_p->release.position = 0;
    self_p->release.increment = 1;
    self_p->release.step = 0;
    self_p->release.steps_max = 1;
    self_p->release.amplitude_curve_p = falling_curve;

    return (0);
}

int envelope_set_phase_release(struct envelope_t *self_p)
{
    self_p->released = 1;

    return (0);
}

size_t envelope_apply(struct envelope_t *self_p,
                       uint32_t *samples_p,
                       size_t length)
{
    int i;
    size_t size = 0;

    /* Attack phase. */
    if (self_p->phase == ENVELOPE_PHASE_ATTACK) {
        size += phase_process(&self_p->attack,
                              &samples_p[0],
                              length);

        if (size < length) {
            self_p->phase = ENVELOPE_PHASE_DECAY;
        }
    }

    /* Decay phase. */
    if (self_p->phase == ENVELOPE_PHASE_DECAY) {
        size += phase_process(&self_p->decay,
                              &samples_p[size],
                              length - size);

        if (size < length) {
            self_p->phase = ENVELOPE_PHASE_SUSTAIN;
        }
    }

    /* Sustain phase. */
    if (self_p->phase == ENVELOPE_PHASE_SUSTAIN) {
        if (self_p->released == 0) {
            /* for (i = size; i < length; i++) { */
            /*     samples_p[i] = 0; */
            /* } */
            
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
