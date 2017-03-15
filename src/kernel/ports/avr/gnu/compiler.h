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

#ifndef __KERNEL_COMPILER_H__
#define __KERNEL_COMPILER_H__

#include "far.h"

#if __cplusplus

#include <avr/pgmspace.h>

#define __PROGMEM PROGMEM

typedef const PROGMEM class prog_str *far_string_t;

#define FSTR(s)                                                         \
    (__extension__(							\
                   {                                                    \
                       static const char __c[] __PROGMEM = (s);         \
                       (far_string_t) &__c[0];                        \
                   }                                                    \
                                                                        ))  
#else

typedef const FAR char *far_string_t;

/**
 * @brief Stores string literal in FAR memory.
 */
#define FSTR(s)                                                         \
    (__extension__(							\
                   {                                                    \
                       static FAR const char __c[] = (s);               \
                       __c;                                             \
                   }                                                    \
                                                                        ))

#endif

#define RAM_CODE

#define PACKED __attribute__((packed))

#endif
