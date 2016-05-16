/**
 * @file kernel/types.h
 * @version 0.4.0
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

/** Create a string of an identifier using the pre-processor. */
#define STRINGIFY(x) STRINGIFY2(x)

/** Used by `STRINGIFY()`. */
#define STRINGIFY2(x) #x

/** Concatenate two tokens. */
#define TOKENPASTE(x, y) TOKENPASTE2(x, y)

/** Used by `TOKENPASTE()`. */
#define TOKENPASTE2(x, y) x ## y

/** Create a unique token. */
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
#define membersof(a) (sizeof(a) / sizeof((a)[0]))

#define container_of(ptr, type, member)                         \
    ({                                                          \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/** Integer division that rounds the result up. */
#define DIV_CEIL(n, d) (((n) + (d) - 1) / d)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define BCD_ENCODE(decoded) (decoded)
#define BCD_DECODE(encoded) (encoded)

/** Debug print of file and line. */
#define PRINT_FILE_LINE() std_printf(FSTR("%s:%d:\r\n"), __FILE__, __LINE__);

#endif
