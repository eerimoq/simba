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

#include "simba.h"
#include "channel.h"

int channel_init(struct channel_t *self_p,
                 int id,
                 int32_t *buf_p,
                 size_t length)
{
    self_p->id = id;
    self_p->state = CHANNEL_STATE_ON;
    self_p->length = 0;
    self_p->waveform.buf_p = buf_p;
    self_p->waveform.length = length;

    return (0);
}

struct note_t *channel_note_alloc(struct channel_t *self_p)
{
    struct note_t *note_p;

    /* Any free note? */
    if (self_p->length == membersof(self_p->notes)) {
        return (NULL);
    }

    note_p = &self_p->notes[self_p->length];
    self_p->length++;

    return (note_p);
}

struct note_t *channel_note_get(struct channel_t *self_p,
                                int note)
{
    int i;
    struct note_t *note_p;

    for (i = 0; i < self_p->length; i++) {
        note_p = &self_p->notes[i];

        if (note_p->note == note) {
            return (note_p);
        }
    }

    return (NULL);
}

int channel_note_iter_reset(struct channel_t *self_p)
{
    self_p->iter.pos = 0;

    return (0);
}

struct note_t *channel_note_iter_next(struct channel_t *self_p)
{
    return (NULL);
}

int channel_set_waveform(struct channel_t *self_p,
                         int32_t *buf_p,
                         size_t length)
{
    self_p->waveform.buf_p = buf_p;
    self_p->waveform.length = length;

    return (0);
}

int channel_set_state(struct channel_t *self_p,
                      int state)
{
    self_p->state = state;

    return (0);
}

int channel_get_id(struct channel_t *self_p)
{
    return (self_p->id);
}

int channel_process(struct channel_t *self_p,
                    int32_t *samples_p,
                    int32_t *buf_p,
                    size_t length)
{
    int i, j;
    struct note_t *note_p;

    if (self_p->state == CHANNEL_STATE_OFF) {
        return (0);
    }

    /* Process all notes for the channel. */
    for (i = 0; i < self_p->length; i++) {
        note_p = &self_p->notes[i];

        oscillator_read(&note_p->oscillator,
                        buf_p,
                        length);

        if (envelope_apply(&note_p->envelope, buf_p, length) != length) {
            /* Free the note when the envelope ends. */
            self_p->notes[i] = self_p->notes[self_p->length - 1];
            self_p->length--;
            i--;
        }

        for (j = 0; j < length; j++) {
            samples_p[j] += buf_p[j];
        }
    }

    return (0);
}
