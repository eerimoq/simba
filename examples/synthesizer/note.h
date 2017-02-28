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
