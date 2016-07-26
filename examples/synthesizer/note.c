/**
 * @file note.c
 * @version 3.0.0
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

#include "simba.h"
#include "note.h"

int note_init(struct note_t *self_p,
              int note,
              int32_t *waveform_p,
              size_t length,
              float frequency,
              float vibrato,
              long attack,
              long decay,
              long release,
              int sample_rate)
{
    self_p->note = note;

    oscillator_init(&self_p->oscillator,
                    waveform_p,
                    length,
                    frequency,
                    vibrato,
                    sample_rate);

    return (envelope_init(&self_p->envelope,
                          attack,
                          decay,
                          release));
}

int note_start(struct note_t *self_p)
{
    return (0);
}

int note_stop(struct note_t *self_p)
{
    return (envelope_set_phase_release(&self_p->envelope));
}
