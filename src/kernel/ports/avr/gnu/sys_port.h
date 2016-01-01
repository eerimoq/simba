/**
 * @file avr/gnu/sys_port.h
 * @version 0.1.0
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

#define FAR __flash

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

#define _ASSERTFMT(fmt, ...) std_printf(FSTR(fmt "\n"), ##__VA_ARGS__);

#define BTASSERT(cond, ...)                                             \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":%d: BTASSERT: " #cond), __LINE__); \
        _ASSERTFMT(__VA_ARGS__);                                        \
        exit(EBTASSERT);                                                \
    }

#if !defined(NASSERT)
#  define ASSERTN(cond, n, ...)                 \
  if (!(cond)) {                                \
    sys.on_fatal_callback(n);                   \
  }
#else
#  define ASSERTN(cond, n, ...)
#endif

#define ASSERT(cond, ...) ASSERTN(cond, 1, __VA_ARGS__)

#define SYS_SETTINGS_APP_BASE 0x100

#define PACKED __attribute__((packed))

#endif
