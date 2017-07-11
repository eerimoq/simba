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

#ifndef __KERNEL_TYPES_PORT_H__
#define __KERNEL_TYPES_PORT_H__

/**
 * Calculate system tick timer configuration parameters from cpu
 * frequency and desired system tick frequency. Select closest match
 * above desired system tick frequency.
 */
#define CPU_CYCLES_PER_SYS_TICK (F_CPU / CONFIG_SYSTEM_TICK_FREQUENCY)

#if (CPU_CYCLES_PER_SYS_TICK == 0)
#    error "CONFIG_SYSTEM_TICK_FREQUENCY is too high."
#elif (CPU_CYCLES_PER_SYS_TICK < (256 * 256))
#    define CLOCK_SELECT       _BV(CS00)
#    define TCNT1_MAX          (CPU_CYCLES_PER_SYS_TICK - 1)
#    define CPU_CYCLES_PER_TIMER_TICK 1ul
#elif (CPU_CYCLES_PER_SYS_TICK < (8 * 256 * 256))
#    define CLOCK_SELECT       _BV(CS01)
#    define TCNT1_MAX          (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 8) - 1)
#    define CPU_CYCLES_PER_TIMER_TICK 8ul
#elif (CPU_CYCLES_PER_SYS_TICK < (64 * 256 * 256))
#    define CLOCK_SELECT       (_BV(CS01) | _BV(CS00))
#    define TCNT1_MAX          (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 64) - 1)
#    define CPU_CYCLES_PER_TIMER_TICK 64ul
#elif (CPU_CYCLES_PER_SYS_TICK < (256 * 256 * 256))
#    define CLOCK_SELECT       _BV(CS02)
#    define TCNT1_MAX          (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 256) - 1)
#    define CPU_CYCLES_PER_TIMER_TICK 256ul
#elif (CPU_CYCLES_PER_SYS_TICK < (1024 * 256 * 256))
#    define CLOCK_SELECT       (_BV(CS02) | _BV(CS00))
#    define TCNT1_MAX          (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 1024) - 1)
#    define CPU_CYCLES_PER_TIMER_TICK 1024ul
#else
#    error "CONFIG_SYSTEM_TICK_FREQUENCY is too low."
#endif

#endif
