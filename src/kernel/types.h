/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef BIT
#    define BIT(pos) (1 << (pos))
#endif

#define BITFIELD_SET(name, value)               \
    (((value) << name ## _POS) & name ## _MASK)

#define BITFIELD_GET(name, value)               \
    (((value) & name ## _MASK) >> name ## _POS)

typedef uint8_t u8_t;
typedef int8_t s8_t;
typedef uint16_t u16_t;
typedef int16_t s16_t;
typedef uint32_t u32_t;
typedef int32_t s32_t;

#endif
