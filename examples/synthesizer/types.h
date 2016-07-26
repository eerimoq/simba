/**
 * @file types.h
 * @version 3.0.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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
