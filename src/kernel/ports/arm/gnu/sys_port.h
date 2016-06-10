/**
 * @file arm/gnu/sys_port.i
 * @version 0.6.0
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

#include "types.h"

#define FAR

#define FSTR(s) s

#define _ASSERTFMT(fmt, ...) std_printf(FSTR(fmt "\n"), ##__VA_ARGS__);

#define BTASSERT(cond, ...)                                             \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":%d: BTASSERT: " #cond), __LINE__); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        exit(EBTASSERT);                                                \
    }

#if CONFIG_ASSERT == 1
#  define ASSERTN(cond, n, ...)                                         \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":%d: ASSERT: (" #cond ") " #__VA_ARGS__ "\r\n"), \
                   __LINE__);                                           \
        sys.on_fatal_callback(n);                                       \
    }
#else
#  define ASSERTN(cond, n, ...)
#endif

#define ASSERT(cond, ...) ASSERTN(cond, 1, __VA_ARGS__)

#define PACKED __attribute__((packed))

#endif
