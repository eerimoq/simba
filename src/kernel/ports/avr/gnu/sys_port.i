/**
 * @file avr/gnu/sys_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#include <avr/interrupt.h>
#include <avr/eeprom.h>

/* Calculate system tick timer configuration parameters from
   cpu frequency and desired system tick frequency. Select
   closest match above desired system tick frequency.*/
#define CPU_CYCLES_PER_SYS_TICK (F_CPU / SYS_TICK_FREQUENCY)
#if (CPU_CYCLES_PER_SYS_TICK == 0)
#    error "SYS_TICK_FREQUENCY is too high."
#elif (CPU_CYCLES_PER_SYS_TICK < 256)
#    define CLOCK_SELECT _BV(CS00)
#    define TCNT0_MAX (CPU_CYCLES_PER_SYS_TICK - 1)
#elif (CPU_CYCLES_PER_SYS_TICK < (8 * 256))
#    define CLOCK_SELECT _BV(CS01)
#    define TCNT0_MAX (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 8) - 1)
#elif (CPU_CYCLES_PER_SYS_TICK < (64 * 256))
#    define CLOCK_SELECT (_BV(CS01) | _BV(CS00))
#    define TCNT0_MAX (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 64) - 1)
#elif (CPU_CYCLES_PER_SYS_TICK < (256 * 256))
#    define CLOCK_SELECT _BV(CS02)
#    define TCNT0_MAX (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 256) - 1)
#elif (CPU_CYCLES_PER_SYS_TICK < (1024 * 256))
#    define CLOCK_SELECT (_BV(CS02) | _BV(CS00))
#    define TCNT0_MAX (DIV_CEIL(CPU_CYCLES_PER_SYS_TICK, 1024) - 1)
#else
#    error "SYS_TICK_FREQUENCY is too low."
#endif

ISR(TIMER0_COMPA_vect)
{
    sys_tick();
}

static int sys_port_module_init(void)
{
    /* Start periodic system tick timer. An interrupt is generated when
       TCNT0 matches OCR0A. CTC timer mode is used. */
    TCCR0A = _BV(WGM01);
    TCCR0B = CLOCK_SELECT;
    TCNT0 = 0;
    OCR0A = TCNT0_MAX;
    TIMSK0 = _BV(OCIE0A);

    /* Enable interrupts. */
    asm volatile ("sei");

    /* System status area in EEPROM. */
    eeprom_write_dword((uint32_t *)0x0, -1);
    eeprom_write_dword((uint32_t *)0x4, MCUSR);

    return (0);
}

static void sys_port_lock(void)
{
    asm volatile ("cli" ::: "memory");
}

static void sys_port_unlock(void)
{
    asm volatile ("sei" ::: "memory");
}

static void sys_port_lock_irq(void)
{
}

static void sys_port_unlock_irq(void)
{
}

void sys_stop(int error)
{
    eeprom_write_dword(0x0, error);
    exit(error);
}

static float sys_port_interrupt_cpu_usage_get(void)
{
    return (0.0f);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}
