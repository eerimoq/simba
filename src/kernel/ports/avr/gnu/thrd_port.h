/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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

#ifndef __KERNEL_THRD_PORT_H__
#define __KERNEL_THRD_PORT_H__

#define THRD_PORT_STACK(name, size)             \
    char name[sizeof(struct thrd_t) + (size)]

#define THRD_PORT_CONTEXT_STORE_ISR             \
    do {                                        \
        asm volatile ("push r0");               \
        asm volatile ("push r1");               \
        asm volatile ("push r18");              \
        asm volatile ("push r19");              \
        asm volatile ("push r20");              \
        asm volatile ("push r21");              \
        asm volatile ("push r22");              \
        asm volatile ("push r23");              \
        asm volatile ("push r24");              \
        asm volatile ("push r25");              \
        asm volatile ("push r26");              \
        asm volatile ("push r27");              \
        asm volatile ("push r30");              \
        asm volatile ("push r31");              \
        asm volatile ("in   r31, 0x3f");        \
        asm volatile ("push r31");              \
    } while (0);

#define THRD_PORT_CONTEXT_LOAD_ISR              \
    do {                                        \
        asm volatile ("pop r31");               \
        asm volatile ("out 0x3f, r31");         \
        asm volatile ("pop r31");               \
        asm volatile ("pop r30");               \
        asm volatile ("pop r27");               \
        asm volatile ("pop r26");               \
        asm volatile ("pop r25");               \
        asm volatile ("pop r24");               \
        asm volatile ("pop r23");               \
        asm volatile ("pop r22");               \
        asm volatile ("pop r21");               \
        asm volatile ("pop r20");               \
        asm volatile ("pop r19");               \
        asm volatile ("pop r18");               \
        asm volatile ("pop r1");                \
        asm volatile ("pop r0");                \
    } while (0);

struct thrd_port_context_t {
    uint8_t dummy;
    uint8_t r29;
    uint8_t r28;
    uint8_t r17;
    uint8_t r16;
    uint8_t r15;
    uint8_t r14;
    uint8_t r13;
    uint8_t r12;
    uint8_t r11;
    uint8_t r10;
    uint8_t r9;
    uint8_t r8;
    uint8_t r7;
    uint8_t r6;
    uint8_t r5;
    uint8_t r4;
    uint8_t r3;
    uint8_t r2;
#if defined(__AVR_3_BYTE_PC__)
    uint8_t pc_3rd_byte;
#endif
    uint8_t pcl;
    uint8_t pch;
};

struct thrd_port_t {
    struct thrd_port_context_t *context_p;
};

#endif
