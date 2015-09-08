/**
 * @file kernel/types.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#ifndef __KERNEL_TYPES_H__
#define __KERNEL_TYPES_H__

#define UNUSED(v) (void)(v)

#define TOKENPASTE2(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE2(x, y)
#define UNIQUE(x)  TOKENPASTE(x, TOKENPASTE(___, __LINE__))

#define membersof(a) (sizeof(a) / sizeof((a)[0]))

#define container_of(ptr, type, member)                         \
    ({                                                          \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define DIV_CEIL(n, d) (((n) + (d) - 1) / d)

#endif
