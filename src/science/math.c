/*
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

#include "simba.h"

/* Inverse log base 2 of e. */
#define INV_LOG2_E_Q1DOT31  UINT64_C(0x58b90bfc)

/* Inverse log base 2 of 10. */
#define INV_LOG2_10_Q1DOT31 UINT64_C(0x268826a1)

#if CONFIG_FLOAT == 1

float math_radians_to_degrees(float value)
{
    return (value * 180.0f / MATH_PI);
}

float math_degrees_to_radians(float value)
{
    return (value * MATH_PI / 180.0f);
}

#endif

/* This implementation is based on Clay. S. Turner's fast binary
   logarithm algorithm[1], and Dan Moulding's C implementation found
   here: https://github.com/dmoulding/log2fix. */
int32_t math_log2_fixed_point(uint32_t x, int precision)
{
    ASSERTN((precision > 0) && (precision < 32), EINVAL);

    int32_t b;
    int32_t y;
    uint64_t z;
    size_t i;

    b = (1 << (precision - 1));
    y = 0;

    if (x == 0) {
        return (INT32_MIN);
    }

    while (x < (1 << precision)) {
        x <<= 1;
        y -= (1 << precision);
    }

    while (x >= (2 << precision)) {
        x >>= 1;
        y += (1 << precision);
    }

    z = x;

    for (i = 0; i < precision; i++) {
        z = ((z * z) >> precision);

        if (z >= (2 << precision)) {
            z >>= 1;
            y += b;
        }

        b >>= 1;
    }

    return (y);
}

int32_t math_ln_fixed_point(uint32_t x, int precision)
{
    uint64_t y;

    y = math_log2_fixed_point(x, precision) * INV_LOG2_E_Q1DOT31;

    return (y >> 31);
}

int32_t math_log10_fixed_point(uint32_t x, int precision)
{
    uint64_t y;

    y = math_log2_fixed_point(x, precision) * INV_LOG2_10_Q1DOT31;

    return (y >> 31);
}
