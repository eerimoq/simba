/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __TYPES_H__
#define __TYPES_H__

#include "simba.h"

typedef int32_t q20_11_t;

/* Convert a floating point value to a fixed point Q20.11 value. */
#define FLOAT_TO_Q20_11(value) (q20_11_t)(value * 2048.0)

/* Convert an integer value to a fixed point Q20.11 value. */
#define INT_TO_Q20_11(value) (q20_11_t)(value << 11)

/* Convert a fixed point Q20.11 value to a float value. */
#define Q20_11_TO_FLOAT(value) ((float)(value) / 2048.0)

/* Convert a fixed point Q20.11 value to an integer value. */
#define Q20_11_TO_INT(value) (int32_t)((value >> 11))

typedef int32_t q11_20_t;

/* Convert a floating point value to a fixed point Q11.20 value. */
#define FLOAT_TO_Q11_20(value) (q11_20_t)(value * 1048576.0)

/* Convert an integer value to a fixed point Q11.20 value. */
#define INT_TO_Q11_20(value) (q11_20_t)(value << 20)

/* Convert a fixed point Q11.20 value to a float value. */
#define Q11_20_TO_FLOAT(value) ((float)(value) / 1048576.0)

/* Convert a fixed point Q11.20 value to an integer value. */
#define Q11_20_TO_INT(value) (int32_t)((value >> 20))

#endif
