/*
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

#ifndef __SCIENCE_SCIENCE_H__
#define __SCIENCE_SCIENCE_H__

#include "simba.h"

#define SCIENCE_SEA_LEVEL_STANDARD_PRESSURE            101325

/**
 * Initialize the science module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int science_module_init(void);

/**
 * Convert given pressure to an its altitude.
 *
 * @param[in] pressure Pressure in Pascal.
 * @param[in] pressure_at_sea_level Sea level pressure in Pascal.
 *
 * @return Altitude in meters, or NaN if an error occurred.
 */
float science_pressure_to_altitude(float pressure,
                                   float pressure_at_sea_level);

/**
 * Convert given altitude to an its pressure.
 *
 * @param[in] altitude Altitude in meters.
 * @param[in] pressure_at_sea_level Sea level pressure in Pascal.
 *
 * @return Pressure in Pascal, or NaN if an error occurred.
 */
float science_pressure_from_altitude(float altitude,
                                     float pressure_at_sea_level);

/**
 * Convert given speed from m/s to km/h.
 *
 * @param[in] speed Speed in m/s.
 *
 * @return Speed in km/h, or NaN if an error occurred.
 */
float science_mps_to_kmph(float speed);

/**
 * Convert given speed from km/h to m/s.
 *
 * @param[in] speed Speed in km/h.
 *
 * @return Speed in m/s, or NaN if an error occurred.
 */
float science_mps_from_kmph(float speed);

/**
 * Convert given speed from m/s to knots.
 *
 * @param[in] speed Speed in m/s.
 *
 * @return Speed in knots, or NaN if an error occurred.
 */
float science_mps_to_knots(float speed);

/**
 * Convert given speed from knots to m/s.
 *
 * @param[in] speed Speed in knots.
 *
 * @return Speed in m/s, or NaN if an error occurred.
 */
float science_mps_from_knots(float speed);

/**
 * Convert given speed from m/s to mi/h.
 *
 * @param[in] speed Speed in m/s.
 *
 * @return Speed in mi/h, or NaN if an error occurred.
 */
float science_mps_to_mph(float speed);

/**
 * Convert given speed from mi/h to m/s.
 *
 * @param[in] speed Speed in mi/h.
 *
 * @return Speed in m/s, or NaN if an error occurred.
 */
float science_mps_from_mph(float speed);

#endif
