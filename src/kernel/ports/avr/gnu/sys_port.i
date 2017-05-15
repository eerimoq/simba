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

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

/**
 * Calculate system tick timer configuration parameters from cpu
 * frequency and desired system tick frequency. Select closest match
 * above desired system tick frequency.
 */
#define CPU_CYCLES_PER_SYS_TICK (F_CPU / CONFIG_SYSTEM_TICK_FREQUENCY)
#if (CPU_CYCLES_PER_SYS_TICK == 0)
#    error "CONFIG_SYSTEM_TICK_FREQUENCY is too high."
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
#    error "CONFIG_SYSTEM_TICK_FREQUENCY is too low."
#endif

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

ISR(TIMER0_COMPA_vect)
{
    sys_tick_isr();
}

static int sys_port_module_init(void)
{
    /* Start periodic system tick timer. An interrupt is generated
       when TCNT0 matches OCR0A. CTC timer mode is used. */
    TCCR0A = _BV(WGM01);
    TCCR0B = CLOCK_SELECT;
    TCNT0 = 0;
    OCR0A = TCNT0_MAX;
    TIMSK0 = _BV(OCIE0A);

    /* Enable interrupts. */
    asm volatile ("sei");

    return (0);
}

__attribute__ ((noreturn))
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

__attribute__ ((noreturn))
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

static int sys_port_get_time_into_tick()
{
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
