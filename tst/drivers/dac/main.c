/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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
#include "dac_gen.i"

#if defined(ARCH_ESP32)

#define SKIP_TEST_SINE_440_HZ
#define SKIP_TEST_PCM1611S

#    define pin_0_dev pin_dac1_dev
#    define pin_1_dev pin_dac2_dev

/* 8 bits resolution. */
#    define AMPLITUDE_MAX 0xff

static uint8_t samples[4096];

#else

#    define pin_0_dev pin_dac0_dev
#    define pin_1_dev pin_dac1_dev

/* 12 bits resolution on SAM3. */
#    define AMPLITUDE_MAX 0xfff

static uint16_t samples[4096];

#endif

static int test_ramp(struct harness_t *harness_p)
{
    int i;
    struct dac_driver_t dac;

    BTASSERT(dac_init(&dac,
                      &dac_0_dev,
                      &pin_0_dev,
                      &pin_1_dev,
                      1) == 0);

    for (i = 0; i < AMPLITUDE_MAX + 1; i++) {
        samples[0] = i;
        samples[1] = (AMPLITUDE_MAX - i);
        std_printf(FSTR("samples[0]: %5d, samples[1]: %5d\r\n"),
                   samples[0],
                   samples[1]);
        BTASSERT(dac_convert(&dac, samples, 2) == 0);
        thrd_sleep(10.0f / (AMPLITUDE_MAX + 1));
    }

    return (0);
}

static int test_sine_440_hz(struct harness_t *harness_p)
{
#if !defined(SKIP_TEST_SINE_440_HZ)
    
    int i;
    float sample;
    int samples_per_second = (membersof(dac_gen_sine) * 440);
    struct dac_driver_t dac;
    int total_number_of_samples;

    BTASSERT(dac_init(&dac,
                      &dac_0_dev,
                      &pin_0_dev,
                      NULL,
                      samples_per_second) == 0);

    /* Samples are in the range -1.0 to 1.0. Convert them to the range
       0 to AMPLITUDE_MAX. */
    for (i = 0; i < membersof(samples); i++) {
        sample = dac_gen_sine[i % membersof(dac_gen_sine)];
        samples[i] =  AMPLITUDE_MAX * ((sample + 1.0) / 2.0);
    }

    std_printf(FSTR("Converting %d samples.\r\n"), (int)membersof(samples));
    BTASSERT(dac_convert(&dac, (uint32_t *)samples, membersof(samples) / 2) == 0);

    /* Converting the signal on the DAC0-pin for 5 seconds. */
    total_number_of_samples = (5 * samples_per_second);
    std_printf(FSTR("Converting %d samples.\r\n"),
               total_number_of_samples);

    for (i = 0; i < total_number_of_samples; i += membersof(samples)) {
        BTASSERT(dac_async_convert(&dac,
                                   (uint32_t *)samples,
                                   membersof(samples) / 2) == 0);
        std_printf(FSTR("Samples left = %d\r\n"), total_number_of_samples - i);
    }

    std_printf(FSTR("Waiting for last samples to be converted\r\n"));

    BTASSERT(dac_async_wait(&dac) == 0);
    
    return (0);

#else

    (void)dac_gen_sine;
    
    return (1);
    
#endif
}

static int test_pcm1611s(struct harness_t *harness_p)
{
#if !defined(SKIP_TEST_PCM1611S)

    int i;
    int samples_per_second = 2 * 11025;
    struct dac_driver_t dac;

    BTASSERT(dac_init(&dac,
                      &dac_0_dev,
                      &pin_0_dev,
                      &pin_1_dev,
                      samples_per_second) == 0);

    for (i = 0; i < membersof(dac_gen_pcm1611s); i += 4096) {
        memcpy(samples, &dac_gen_pcm1611s[i], sizeof(samples));
        BTASSERT(dac_async_convert(&dac,
                                   (uint32_t *)samples,
                                   4096 / 2) == 0);
    }

    BTASSERT(dac_async_wait(&dac) == 0);

    return (0);

#else
    
    return (1);
    
#endif
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_ramp, "test_ramp" },
        { test_sine_440_hz, "test_sine_440_hz" },
        { test_pcm1611s, "test_pcm1611s" },
        { NULL, NULL }
    };

    sys_start();
    dac_module_init();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
