/*
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

#include "simba.h"

static void (*irq_table[128])(void);

/**
 * Do nothing if no interrupt service routine is installed in the
 * interrupt vector.
 */
ISR(none)
{
    sys_panic(FSTR("ISR(none)"));
}

ESR(none)
{
    sys_panic(FSTR("ESR(none)"));
}

ESR(sync)
{
    sys_panic(FSTR("ESR(sync)"));
}

ESR(irq)
{
    uint32_t ia;

    ia = VIRT_GIC_CPU->IA;

    /* Call the interrupt handler. */
    irq_table[ia & 0x3ff]();

    VIRT_GIC_CPU->EOI = ia;
}

ESR(fiq)
{
    sys_panic(FSTR("ESR(fiq)"));
}

ESR(serror)
{
    sys_panic(FSTR("ESR(serror)"));
}

void isr_virt_timer(void)         __attribute__ ((weak, alias("isr_none")));
void isr_phys_timer(void)         __attribute__ ((weak, alias("isr_none")));
void isr_pl011(void)              __attribute__ ((weak, alias("isr_none")));
void isr_virtio_net(void)         __attribute__ ((weak, alias("isr_none")));
void isr_virtio_blk(void)         __attribute__ ((weak, alias("isr_none")));
void isr_virtio_con(void)         __attribute__ ((weak, alias("isr_none")));

int rand()
{
    return (0);
}

void xvisor_virt_v8_init(void)
{
    int i;

    for (i = 32 / 16; i < 96 / 16; i++) {
        VIRT_GIC_DIST->CONFIG[i] = 0;
    }

    for (i = 32 / 4; i < 96 / 4; i++) {
        VIRT_GIC_DIST->TARGET[i] = 0x01010101;
    }

    VIRT_GIC_DIST->CTRL = 1;
    VIRT_GIC_CPU->PRIMASK = 0xf0;
    VIRT_GIC_CPU->CTRL = 1;
}

static void (*irq_table[128])(void) = {
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_virt_timer,
    isr_none,
    isr_none,
    isr_phys_timer,
    isr_none,
    isr_none,
    isr_pl011,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_virtio_net,
    isr_virtio_blk,
    isr_virtio_con,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none,
    isr_none
};
