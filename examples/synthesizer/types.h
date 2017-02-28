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
