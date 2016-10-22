/**
 * @file kernel/types.h
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

#ifndef __KERNEL_TYPES_H__
#define __KERNEL_TYPES_H__

/**
 * Ignore unused function argument.
 *
 * An example of a function that does not use it's first argument
 * ``a``:
 *
 * @rst
 * .. code-block:: c
 *
 *    int foo(int a, int b)
 *    {
 *        UNUSED(a);
 *
 *        return (b);
 *    }
 * @endrst
 */
#define UNUSED(v) (void)(v)

/**
 * Create a string of an identifier using the pre-processor.
 */
#define STRINGIFY(x) STRINGIFY2(x)

/**
 * Used internally by `STRINGIFY()`.
 */
#define STRINGIFY2(x) #x

/**
 * Concatenate two tokens.
 */
#define TOKENPASTE(x, y) TOKENPASTE2(x, y)

/**
 * Used internally by `TOKENPASTE()`.
 */
#define TOKENPASTE2(x, y) x ## y

/**
 * Create a unique token.
 */
#define UNIQUE(x)  TOKENPASTE(x, TOKENPASTE(___, __LINE__))

/**
 * Get the number of elements in an array.
 *
 * As an example, the code below outputs ``number of members in foo =
 * 10``.
 *
 * @rst
 * .. code-block:: c
 *
 *    int foo[10];
 *
 *    std_printf(FSTR("number of members in foo = %d\\r\\n"),
 *               membersof(foo));
 * @endrst
 */
#define membersof(a) ((ssize_t)(sizeof(a) / sizeof((a)[0])))

#define container_of(ptr, type, member)                         \
    ({                                                          \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/**
 * Integer division that rounds the result up.
 */
#define DIV_CEIL(n, d) (((n) + (d) - 1) / d)

/**
 * Get the minimum value of the two.
 */
#ifndef MIN
#    define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/**
 * Get the maximum value of the two.
 */
#ifndef MAX
#    define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/**
 * Debug print of file and line.
 */
#define PRINT_FILE_LINE() std_printf(FSTR("%s:%d:\r\n"), __FILE__, __LINE__);

#if CONFIG_DEBUG == 1
#    define STD_PRINTF_DEBUG(...) std_printf(__VA_ARGS__)
#else
#    define STD_PRINTF_DEBUG(...)
#endif

#define _ASSERTFMT(fmt, ...) std_printf(FSTR(fmt "\n"), ##__VA_ARGS__);

/**
 * Assert given condition and call the system on fatal callback with
 * given value ``n`` on error.
 */
#if CONFIG_ASSERT == 1
#    define ASSERTN(cond, n, ...)                                         \
    if (!(cond)) {                                                      \
        std_printf(FSTR(__FILE__ ":%d: ASSERT: (" #cond ") " #__VA_ARGS__ "\r\n"), \
                   __LINE__);                                           \
        sys.on_fatal_callback(n);                                       \
    }
#else
#    define ASSERTN(cond, n, ...)
#endif

/**
 * Assert given condition and call the system on fatal callback with
 * value ``1`` on error.
 */
#define ASSERT(cond, ...) ASSERTN(cond, 1, __VA_ARGS__)

#define BIT(pos) (1 << (pos))

#define BITFIELD_SET(name, value)               \
    (((value) << name ## _POS) & name ## _MASK)

#define BITFIELD_GET(name, value)               \
    (((value) & name ## _MASK) >> name ## _POS)

#endif
