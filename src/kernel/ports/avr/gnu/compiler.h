/**
 * @file avr/gnu/compiler.h
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

#ifndef __KERNEL_COMPILER_H__
#define __KERNEL_COMPILER_H__

#if __cplusplus

#include <avr/pgmspace.h>

#define FAR

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

#define FAR __flash

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
