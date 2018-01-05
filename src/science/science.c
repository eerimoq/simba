/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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
#include <math.h>

#define MOLAR_MASS                               0.0289644
#define EARTH_SURFACE_GRAVITATIONAL_ACCELERATION 9.80665
#define IDEAL_GAS_CONSTANT                       8.31432
#define SEA_LEVEL_TEMPERATURE                    288.15
#define TEMPERATURE_LAPSE_RATE                   0.0065

int science_module_init()
{
    return (0);
}

float science_pressure_to_altitude(float pressure,
                                   float pressure_at_sea_level)
{
    float altitude;
    float rl_gm;

    if (pressure > 22632.1) {
        rl_gm = ((IDEAL_GAS_CONSTANT * TEMPERATURE_LAPSE_RATE)
                 / (EARTH_SURFACE_GRAVITATIONAL_ACCELERATION * MOLAR_MASS));
        altitude = (1.0 - pow(pressure / SCIENCE_SEA_LEVEL_STANDARD_PRESSURE,
                              rl_gm));
        altitude *= (SEA_LEVEL_TEMPERATURE / TEMPERATURE_LAPSE_RATE);
    } else {
        altitude = NAN;
    }

    return (altitude);
}

float science_pressure_from_altitude(float altitude,
                                     float pressure_at_sea_level)
{
    float pressure;
    float lh_t0;
    float gm_rl;

    if (altitude < 11000.0) {
        lh_t0 = ((TEMPERATURE_LAPSE_RATE * altitude) / SEA_LEVEL_TEMPERATURE);
        gm_rl = ((EARTH_SURFACE_GRAVITATIONAL_ACCELERATION * MOLAR_MASS)
                 / (IDEAL_GAS_CONSTANT * TEMPERATURE_LAPSE_RATE));
        pressure = (pressure_at_sea_level * pow(1.0 - lh_t0, gm_rl));
    } else {
        pressure = NAN;
    }

    return (pressure);
}

float science_mps_to_kmph(float speed)
{
    return (3.6 * speed);
}

float science_mps_from_kmph(float speed)
{
    return (speed / 3.6);
}

float science_mps_to_knots(float speed)
{
    return (1.94384 * speed);
}

float science_mps_from_knots(float speed)
{
    return (speed / 1.94384);
}

float science_mps_to_mph(float speed)
{
    return (2.23694 * speed);
}

float science_mps_from_mph(float speed)
{
    return (speed / 2.23694);
}
