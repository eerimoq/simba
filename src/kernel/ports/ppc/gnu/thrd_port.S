/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

/**
 * Perform a thread context switch.
 *
 * @param[in] lr Return address.
 * @param[in] r1 Stack address.
 * @param[in] r3 Thread to swap in.
 * @param[in] r4 Thread to swap out.
 */
        .globl    thrd_port_swap
        .type     thrd_port_swap,@function
        .align    4
thrd_port_swap:
        /* Store registers on the stack. lr is the return address. */
        e_add16i  %r1, %r1, -84

        e_stw     %r0, 88(%r1)
        e_stw     %r2, 84(%r1)
        se_mflr   %r2
        e_stw     %r2, 80(%r1)
        mfcr      %r2
        e_stw     %r2, 76(%r1)

        /* Store r13-r31 in a multi work operation. */
        e_stmw    %r13, 0(%r1)

        /* Save 'out_p' stack pointer. */
        e_stw     %r1, 8(%r4)

        /* Restore 'in_p' stack pointer. */
        e_lwz     %r1, 8(%r3)

        /* Load registers from the stack. Pop lr and continue execution. */
        e_lmw     %r13, 0(%r1)

        e_lwz     %r2, 76(%r1)
        mtcr      %r2
        e_lwz     %r2, 80(%r1)
        se_mtlr   %r2
        e_lwz     %r2, 84(%r1)
        e_lwz     %r0, 88(%r1)

        e_add16i  %r1, %r1, 84

        se_blr

/**
 * Thread entry function wrapper.
 */
        .globl    thrd_port_main
        .type     thrd_port_main,@function
        .align    4
thrd_port_main:
        /* Enable interrupts. */
        wrteei    1

        /* Call thread main function with argument. */
        se_mfar   %r3, %r14
        mtctr     %r13
        se_bctrl

        /* Call termination function. */
        e_bl      terminate
