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
