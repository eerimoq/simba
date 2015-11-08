/**
 * @file main.c
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

#include "simba.h"
#include "dac_gen.i"

/* 12 bits resolution on SAM3. */
#define AMPLITUDE_MAX 0xfff

static int samples[4096];

static int test_sine_440_hz(struct harness_t *harness_p)
{
    int i;
    struct dac_driver_t dac;
    int samples_per_second = (membersof(dac_gen_sine) * 440);
    float sample;

    BTASSERT(dac_init(&dac,
                      &dac_0_dev,
                      &pin_dac1_dev,
                      samples_per_second) == 0);

    /* Samples are in the range -1.0 to 1.0. Convert them to the range
       0 to AMPLITUDE_MAX. */
    for (i = 0; i < membersof(samples); i++) {
        sample = dac_gen_sine[i % membersof(dac_gen_sine)];
        samples[i] =  AMPLITUDE_MAX * ((sample + 1.0) / 2.0);
    }

    std_printf(FSTR("Writing %d samples to DAC.\r\n"), 5 * samples_per_second);

    /* Output the signal on the DAC0-pin for 5 seconds. */
    for (i = 0; i < (5 * samples_per_second / membersof(samples)); i++) {
        BTASSERT(dac_async_convert(&dac, samples, membersof(samples)) == 0);
        std_printf(FSTR("Wrote samples %d to %d.\r\n"),
                   i * membersof(samples),
                   (i + 1) * membersof(samples));
    }

    BTASSERT(dac_async_wait(&dac) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_sine_440_hz, "test_sine_440_hz" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();
    dac_module_init();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
