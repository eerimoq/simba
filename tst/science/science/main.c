/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

static int test_pressure_to_altitude(struct harness_t *harness_p)
{
    float altitude;

    /* 22632.2 Pa, 11000.0 meters. */
    altitude = science_pressure_to_altitude(
                  22632.2,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(altitude, 10999.96, 11000.0);

    /* 46563.3 Pa, 6096.0 meters. */
    altitude = science_pressure_to_altitude(
                  46563.3,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(altitude, 6095.994, 6095.995);

    /* 101225 Pa, 8.3 meters. */
    altitude = science_pressure_to_altitude(
                  101225.0,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(altitude, 8.3276, 8.3277);

    /* 101325 Pa, 0.0 meters. */
    altitude = science_pressure_to_altitude(
                  101325.0,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT(altitude == 0.0);

    /* 101425 Pa, -8.3 meters. */
    altitude = science_pressure_to_altitude(
                  101425.0,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(altitude, -8.3210, -8.3209);

    /* 121023 Pa, -1524.0 meters. */
    altitude = science_pressure_to_altitude(
                  121023.0,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(altitude, -1523.981, -1523.980);

    /* Too low pressure. */
    altitude = science_pressure_to_altitude(
        22632.1,
        SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT(isnan(altitude));

    return (0);
}

static int test_pressure_from_altitude(struct harness_t *harness_p)
{
    float pressure;

    /* 22632.2 Pa, 11000.0 meters. */
    pressure = science_pressure_from_altitude(
                  10999.9,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(pressure, 22632.4, 22632.5);

    /* 46563.3 Pa, 6096.0 meters. */
    pressure = science_pressure_from_altitude(
                  6096.0,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(pressure, 46563.2, 46563.3);

    /* 101225 Pa, 8.3 meters. */
    pressure = science_pressure_from_altitude(
                  8.3,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(pressure, 101225.3, 101225.4);

    /* 101325 Pa, 0.0 meters. */
    pressure = science_pressure_from_altitude(
                  0.0,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT(pressure == 101325.0);

    /* 101425 Pa, -8.3 meters. */
    pressure = science_pressure_from_altitude(
                  -8.3,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(pressure, 101424.7, 101424.8);

    /* 121023 Pa, -1524.0 meters. */
    pressure = science_pressure_from_altitude(
                  -1524.0,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT_IN_RANGE(pressure, 121023.2, 121023.3);

    /* Too high altitude. */
    pressure = science_pressure_to_altitude(
                  11000.1,
                  SCIENCE_SEA_LEVEL_STANDARD_PRESSURE);
    BTASSERT(isnan(pressure));

    return (0);
}

static int test_mps_to_kmph(struct harness_t *harness_p)
{
    BTASSERT_IN_RANGE(science_mps_to_kmph(1.0), 3.5999, 3.6001);

    return (0);
}

static int test_mps_from_kmph(struct harness_t *harness_p)
{
    BTASSERT_IN_RANGE(science_mps_from_kmph(3.6), 0.9999, 1.0001);

    return (0);
}

static int test_mps_to_knots(struct harness_t *harness_p)
{
    BTASSERT_IN_RANGE(science_mps_to_knots(1.0), 1.94383, 1.94385);

    return (0);
}

static int test_mps_from_knots(struct harness_t *harness_p)
{
    BTASSERT_IN_RANGE(science_mps_from_knots(1.0), 0.51443, 0.51445);

    return (0);
}

static int test_mps_to_mph(struct harness_t *harness_p)
{
    BTASSERT_IN_RANGE(science_mps_to_mph(1.0), 2.23693, 2.23695);

    return (0);
}

static int test_mps_from_mph(struct harness_t *harness_p)
{
    BTASSERT_IN_RANGE(science_mps_from_mph(1.0), 0.44703, 0.44705);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_pressure_to_altitude, "test_pressure_to_altitude" },
        { test_pressure_from_altitude, "test_pressure_from_altitude" },
        { test_mps_to_kmph, "test_mps_to_kmph" },
        { test_mps_from_kmph, "test_mps_from_kmph" },
        { test_mps_to_knots, "test_mps_to_knots" },
        { test_mps_from_knots, "test_mps_from_knots" },
        { test_mps_to_mph, "test_mps_to_mph" },
        { test_mps_from_mph, "test_mps_from_mph" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
