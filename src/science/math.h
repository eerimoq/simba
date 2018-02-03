/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Dan Moulding (logarithms)
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

#ifndef __SCIENCE_MATH_H__
#define __SCIENCE_MATH_H__

#include "simba.h"

#define MATH_PI 3.14159f

/**
 * Convert given angle from radians to degrees.
 *
 * @param[in] angle Angle in radians.
 *
 * @return Angle in degrees, or NaN if an error occurred.
 */
float math_radians_to_degrees(float value);

/**
 * Convert given angle from degrees to radians.
 *
 * @param[in] angle Angle in degrees.
 *
 * @return Angle in radians, or NaN if an error occurred.
 */
float math_degrees_to_radians(float value);

/**
 * Calculate the 2-logarithm of given value in given fixed point
 * precision.
 *
 * @param[in] x Value to calculate the 2-logarithm of.
 * @param[in] precision Fixed point precision in the range 1 to 31,
 *                      inclusive.
 *
 * @return 2-logarithm of given value x, in given precision.
 */
int32_t math_log2_fixed_point(uint32_t x, int precision);

/**
 * Calculate the natural logarithm of given value in given fixed point
 * precision.
 *
 * @param[in] x Value to calculate the natural logarithm of.
 * @param[in] precision Fixed point precision in the range 1 to 31,
 *                      inclusive.
 *
 * @return Natural logarithm of given value x, in given precision.
 */
int32_t math_ln_fixed_point(uint32_t x, int precision);

/**
 * Calculate the 10-logarithm of given value in given fixed point
 * precision.
 *
 * @param[in] x Value to calculate the 10-logarithm of.
 * @param[in] precision Fixed point precision in the range 1 to 31,
 *                      inclusive.
 *
 * @return 10-logarithm of given value x, in given precision.
 */
int32_t math_log10_fixed_point(uint32_t x, int precision);

#endif
