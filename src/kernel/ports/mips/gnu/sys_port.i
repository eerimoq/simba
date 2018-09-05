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

extern int main();

#define SYS_TICK_COUNT           (F_CPU / CONFIG_SYSTEM_TICK_FREQUENCY / 2)

/* #if CONFIG_START_CONSOLE_DEVICE_INDEX == 0 */
/* #    define CONSOLE_UART_REGS                  SPC5_LINFLEX_0 */
/* #elif CONFIG_START_CONSOLE_DEVICE_INDEX == 1 */
/* #    define CONSOLE_UART_REGS                  SPC5_LINFLEX_1 */
/* #elif CONFIG_START_CONSOLE_DEVICE_INDEX == 2 */
/* #    define CONSOLE_UART_REGS                  SPC5_LINFLEX_2 */
/* #else */
/* #    error "Bad console UART index." */
/* #endif */

ISR(core_timer)
{
    uint32_t compare;

    pic32mm_reg_write(&PIC32MM_INT->IFS[0].CLR, 1);

    /* Setup next timeout. */
    compare = pic32mm_mfc0(11, 0);
    compare += SYS_TICK_COUNT;
    pic32mm_mtc0(11, 0, compare);

    sys_tick_isr();
}

#if CONFIG_SYS_RESET_CAUSE == 1

static void read_and_clear_reset_cause(void)
{
}

#endif

static int sys_port_module_init(void)
{
    pic32mm_reg_write(&PIC32MM_INT->IPC[0], 7 << 2);
    pic32mm_reg_write(&PIC32MM_INT->IFS[0].CLR, 1);
    pic32mm_reg_write(&PIC32MM_INT->IEC[0].SET, 1);

    /* Start the system tick timer. */
    pic32mm_mtc0(9, 0, 0);
    pic32mm_mtc0(11, 0, SYS_TICK_COUNT);

#if CONFIG_SYS_RESET_CAUSE == 1
    read_and_clear_reset_cause();
#endif

    return (0);
}

static void sys_port_stop(int error)
{
    while (1);
}

static void sys_port_panic_putc(char c)
{
    /* Write the next byte. */
    pic32mm_reg_write(&PIC32MM_UART2->TXREG, c);

    /* Wait for the byte to be transmitted. */
    while ((pic32mm_reg_read(&PIC32MM_UART2->STA) & BIT(8)) == 0);
}

static void sys_port_reboot()
{
#if CONFIG_WATCHDOG == 1
    watchdog_start_ms(1, NULL);
#endif

    while (1);
}

static int sys_port_backtrace(void **buf_pp, size_t size)
{
    return (-ENOSYS);
}

static int32_t sys_port_get_time_into_tick()
{
    return (0);
}

static void sys_port_lock(void)
{
    asm volatile("di");
}

static void sys_port_unlock(void)
{
    asm volatile("ei");
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

int rand()
{
    return (0);
}
