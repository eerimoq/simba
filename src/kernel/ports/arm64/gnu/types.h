/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#ifndef __KERNEL_PORTS_TYPES_H__
#define __KERNEL_PORTS_TYPES_H__

/**
 * Get specified field of a PSR.
 */
#define ARM64_MRS(reg) ({                       \
            uint64_t UNIQUE(value);             \
            asm volatile("mrs %0, " reg         \
                         : "=r" (UNIQUE(value)) \
                         :                      \
                         : "memory");           \
            UNIQUE(value);                      \
        })

/**
 * Set specified field of a PSR.
 */
#define ARM64_MSR(reg, value)                   \
    asm volatile("msr " reg ", %0"              \
                 :                              \
                 : "r" (value)                  \
                 : "memory")

#define ARM64_CNTP_CTL_ENABLE                   (1 << 0)

#endif
