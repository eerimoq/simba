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
#include "oscillator.h"

static q20_11_t
frequency_to_phase_increment(float frequency,
                             size_t waveform_length,
                             int sample_rate)
{
    return FLOAT_TO_Q20_11(((frequency * waveform_length)
                            / sample_rate));
}

int oscillator_init(struct oscillator_t *self_p,
                    int32_t *waveform_p,
                    size_t length,
                    float frequency,
                    float vibrato,
                    int sample_rate)
{
    self_p->sample_rate = sample_rate;
    self_p->sample_counter = 0;
    self_p->frequency = frequency;
    self_p->vibrato.value = vibrato;
    self_p->vibrato.phase_increment = FLOAT_TO_Q20_11(vibrato);
    self_p->phase = 0;
    self_p->phase_increment =
        frequency_to_phase_increment(frequency,
                                     length,
                                     sample_rate);
    self_p->waveform.length = length;
    self_p->waveform.buf_p = waveform_p;

    return (0);
}

int oscillator_set_frequency(struct oscillator_t *self_p,
                             float frequency)
{
    self_p->frequency = frequency;
    self_p->phase_increment =
        frequency_to_phase_increment(frequency,
                                     self_p->waveform.length,
                                     self_p->sample_rate);

    return (0);
}

int oscillator_set_vibrato(struct oscillator_t *self_p,
                           float vibrato)
{
    if (self_p->vibrato.value < 0.0) {
        vibrato *= -1.0;
    }

    self_p->vibrato.value = vibrato;
    self_p->vibrato.phase_increment = FLOAT_TO_Q20_11(vibrato);

    return (0);
}

int oscillator_read(struct oscillator_t *self_p,
                    int32_t *samples_p,
                    size_t length)
{
    int i;
    int index;

    for (i = 0; i < length; i++) {
        if ((self_p->sample_counter % 4096) == 0) {
            self_p->vibrato.value *= -1.0;
            self_p->vibrato.phase_increment =
                FLOAT_TO_Q20_11(self_p->vibrato.value);
        }

        self_p->phase += self_p->phase_increment;
        self_p->phase += self_p->vibrato.phase_increment;
        index = (((uint32_t)Q20_11_TO_INT(self_p->phase))
                 % self_p->waveform.length);
        samples_p[i] = self_p->waveform.buf_p[index];

        self_p->sample_counter++;
    }

    return (0);
}
