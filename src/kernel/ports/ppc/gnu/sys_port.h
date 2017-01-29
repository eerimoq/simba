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

#ifndef __KERNEL_SYS_PORT_H__
#define __KERNEL_SYS_PORT_H__

#define _ASSERTFMT(fmt, ...) std_printf(FSTR(fmt "\n"), ##__VA_ARGS__);

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

static inline uint32_t htonl(uint32_t v)
{
    return (v);
}

#define ntohl(v) htonl(v)

static inline uint16_t htons(uint16_t v)
{
    return (v);
}

#define ntohs(v) htons(v)

#endif
