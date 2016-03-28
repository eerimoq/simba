/**
 * @file linux/gnu/sys_port.i
 * @version 0.3.0
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

#ifndef __KERNEL_SYS_PORT_H__
#define __KERNEL_SYS_PORT_H__

#define FAR

#define FSTR(s) s

#define _ASSERTFMT(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

#define BTASSERT(cond, ...)                                             \
    if (!(cond)) {                                                      \
        fprintf(stderr, __FILE__ ":%d: BTASSERT: %s ", __LINE__, #cond); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        exit(1);                                                        \
    }

#if !defined(NASSERT)
#  define ASSERTN(cond, n, ...)                                         \
    if (!(cond)) {                                                      \
        fprintf(stderr, __FILE__ ":%d: ASSERT: %s ", __LINE__, #cond);  \
        _ASSERTFMT(__VA_ARGS__);                                        \
        sys.on_fatal_callback(n);                                       \
    }
#else
#  define ASSERTN(cond, n, ...)
#endif

#define ASSERT(cond, ...) ASSERTN(cond, 1, __VA_ARGS__)

#define PACKED __attribute__((packed))

#endif
