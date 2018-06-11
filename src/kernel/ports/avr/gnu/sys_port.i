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

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include "types_port.h"

#if CONFIG_START_CONSOLE_DEVICE_INDEX == 0
#    define CONSOLE_UCSRNA                             UCSR0A
#    define CONSOLE_UDRN                                 UDR0
#elif CONFIG_START_CONSOLE_DEVICE_INDEX == 1
#    define CONSOLE_UCSRNA                             UCSR1A
#    define CONSOLE_UDRN                                 UDR1
#elif CONFIG_START_CONSOLE_DEVICE_INDEX == 2
#    define CONSOLE_UCSRNA                             UCSR2A
#    define CONSOLE_UDRN                                 UDR2
#elif CONFIG_START_CONSOLE_DEVICE_INDEX == 3
#    define CONSOLE_UCSRNA                             UCSR3A
#    define CONSOLE_UDRN                                 UDR3
#else
#    error "Bad console UART index."
#endif

ISR(TIMER1_COMPA_vect)
{
    /* Clear the overflow flag. */
    TIFR1 = _BV(TOV1);

    sys_tick_isr();
}

static int sys_port_module_init(void)
{
    /* Start periodic system tick timer. An interrupt is generated
       when TCNT1 matches OCR1A. CTC timer mode is used. */
    TCCR1A = 0;
    TCCR1B = (_BV(WGM12) | CLOCK_SELECT);
    TCNT1 = 0;
    OCR1A = TCNT1_MAX;
    TIMSK1 = _BV(OCIE1A);

    /* Enable interrupts. */
    asm volatile ("sei");

    return (0);
}

static void sys_port_stop(int error)
{
#if defined(BOARD_ARDUINO_PRO_MICRO)

    /* Handle USB device driver interrupts to reset the baord and
       enter the bootloader. */
    while (1);

#endif

    exit(error);
}

static void sys_port_panic_putc(char c)
{
#if !defined(BOARD_ARDUINO_PRO_MICRO)
    /* Wait for the transmission buffer to be empty. */
    while ((CONSOLE_UCSRNA & _BV(UDRE0)) == 0);

    CONSOLE_UDRN = c;
#endif
}

static void sys_port_reboot()
{
    sys_lock();

    /* Use the watchdog to reboot the system. */
    wdt_enable(1);

    while (1);
}

static int sys_port_backtrace(void **buf_pp, size_t size)
{
    return (0);
}

static long sys_port_get_time_into_tick()
{
    long cpu_cycles;

    cpu_cycles = (CPU_CYCLES_PER_TIMER_TICK * (uint32_t)TCNT1);

    /* Did the counter wrap around to zero, but the system tick
       interrupt handler has not been executed? */
    if (TIFR1 & _BV(TOV1)) {
        cpu_cycles += CPU_CYCLES_PER_SYS_TICK;
    }

    return (1000ul * (cpu_cycles / (F_CPU / 1000000ul)));
}

static void sys_port_lock(void)
{
    asm volatile ("cli" ::: "memory");
}

static void sys_port_unlock(void)
{
    asm volatile ("sei" ::: "memory");
}

static void sys_port_lock_isr(void)
{
}

static void sys_port_unlock_isr(void)
{
}

static cpu_usage_t sys_port_interrupt_cpu_usage_get(void)
{
    return (0);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}
