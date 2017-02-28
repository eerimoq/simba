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

#define WOE_MASK                            0x00040000

.extern sys_unlock
.extern terminate

/**
 * Perform a thread context switch. Must be called by call4/8/12
 * instructions (so called windowed ABI).
 *
 * @param[in] a0 Return address.
 * @param[in] a1 Stack address.
 * @param[in] a2 Thread to swap in.
 * @param[in] a3 Thread to swap out.
 */
    .globl  thrd_port_swap
    .type   thrd_port_swap,@function
    .align  4
thrd_port_swap:
    entry   a1, 32

    /* Store registers on the stack. The windowed ABI stores most
       registers automatically. */
    rsr     a6, PS
    s32i    a0, a1, 0
    s32i    a6, a1, 4

    /* a2 and a3 are clobbered by xthal_window_spill_nw, so store the
       in and out thread pointers in a4 and a5. */
    mov     a4, a2
    mov     a5, a3

   /* The register window for the out thread must be stored in the
      save area on the stack for each frame. Underflow exceptions will
      restore the register window when registers are accessed. */
    movi    a7, ~WOE_MASK /* Disable register window overflow
                             detection. */
    and     a7, a6, a7
    wsr     a7, PS
    rsync
    call0   xthal_window_spill_nw

    /* Store swapped out-threads' stack address in the thread
       context. */
    s32i    a1, a5, 8

    /* Load swapped in-threads' stack address from the thread
       context. */
    l32i    a1, a4, 8

    /* Restore registers from the in-thread stack. */
    l32i    a6, a1, 4
    l32i    a0, a1, 0
    wsr     a6, PS

    retw.n

/**
 * Entry function of all threads except the main thread. Calls the
 * thread main function with argument.
 */
    .globl  thrd_port_main
    .type   thrd_port_main,@function
    .literal_position
    .align  4
thrd_port_main:
    //call4   sys_unlock

    /* Call the thread main function with argument. The main function
       and its argument is stored on the stack in the spawn
       function. */
    mov     a6, a2
    callx4   a3

    /* Thread termination. */
    call4   terminate
