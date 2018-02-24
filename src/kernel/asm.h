/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

#ifndef __KERNEL_ASM_H__
#define __KERNEL_ASM_H__

/**
 * Beginning of function with given name.
 */
.macro ASM_FUNC_BEGIN, name, align
        .globl  \name
        .type   \name,@function
        .align  \align
        .size   \name, .L_\name\()_end - \name
\name:
.endm

/**
 * Beginning of weak function with given name.
 */
.macro ASM_FUNC_WEAK_BEGIN, name, align
        .globl  \name
        .type   \name,@function
        .weak   \name
        .align  \align
        .size   \name, .L_\name\()_end - \name
\name:
.endm

/**
 * Beginning of local function with given name.
 */
.macro ASM_FUNC_LOCAL_BEGIN, name, align
        .type   \name,@function
        .align  \align
        .size   \name, .L_\name\()_end - \name
\name:
.endm

/**
 * End of function with given name.
 */
.macro ASM_FUNC_END, name
        .L_\name\()_end:
.endm

/**
 * Beginning of object with given name.
 */
.macro ASM_OBJ_BEGIN, name, align
        .globl  \name
        .type   \name,@object
        .align  \align
        .size   \name, .L_\name\()_end - \name
\name:
.endm

/**
 * End of object with given name.
 */
.macro ASM_OBJ_END, name
        .L_\name\()_end:
.endm

#endif
