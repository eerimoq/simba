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

ISR(phys_timer)
{
    ARM64_MSR("cntp_tval_el0", 192000);
    ARM64_MSR("cntp_ctl_el0", ARM64_CNTP_CTL_ENABLE);
    sys_tick_isr();
}

static int sys_port_module_init(void)
{
    /* Setup the system tick timer. */
    mcu_configure_interrupt(VIRT_IRQ_PHYS_TIMER, 0);
    ARM64_MSR("cntp_tval_el0", 192000);
    ARM64_MSR("cntp_ctl_el0", ARM64_CNTP_CTL_ENABLE);

    sys_unlock();

    return (0);
}

static void sys_port_stop(int error)
{
    while (1);
}

static void sys_port_panic_putc(char c)
{
    while (VIRT_UART_PL011->FR & VIRT_UART_PL011_FR_TXFF);

    VIRT_UART_PL011->DR = c;
}

static void sys_port_reboot()
{
    while(1);
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
     asm volatile ("msr daifset, (1 << 1)" : : : "memory");
}

static void sys_port_unlock(void)
{
    asm volatile ("msr daifclr, (1 << 1)" : : : "memory");
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
