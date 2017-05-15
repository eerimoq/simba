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

#if defined(FAMILY_SAM)
#    define SYSTEM_TIMER_LOAD_RELOAD_NOM 10000000
#else
#    define SYSTEM_TIMER_LOAD_RELOAD_NOM 20000000
#endif

ISR(sys_tick)
{
#if defined(FAMILY_STM32F2)
    /* Kick the watchdog. */
    STM32_IWDG->KR = 0xaaaa;
#endif

    sys_tick_isr();
}

static int sys_port_module_init(void)
{
    /* Setup the system tick timer. */
    ARM_ST->LOAD = SYSTEM_TIMER_LOAD_RELOAD(SYSTEM_TIMER_LOAD_RELOAD_NOM
                                            / CONFIG_SYSTEM_TICK_FREQUENCY);
    ARM_ST->CTRL = (SYSTEM_TIMER_CTRL_TICKINT
                    | SYSTEM_TIMER_CTRL_ENABLE);

    /* Enable interrupts. */
    asm volatile("cpsie i" : : : "memory");

    return (0);
}

__attribute__ ((noreturn))
static void sys_port_stop(int error)
{
    exit(error);
}

static void sys_port_panic_putc(char c)
{
}

__attribute__ ((noreturn))
static void sys_port_reboot()
{
#if defined(FAMILY_SAM)
    SAM_RSTC->CR = (RSTC_CR_KEY(0xa5)
                    | RSTC_CR_PERRST
                    | RSTC_CR_PROCRST);
#endif

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
    asm volatile("cpsid i" : : : "memory");
}

static void sys_port_unlock(void)
{
    asm volatile("cpsie i" : : : "memory");
}

static void sys_port_lock_isr(void)
{
}

static void sys_port_unlock_isr(void)
{
}

/* Newlib stub functions. */
void _exit(int status)
{
    while (1);
}

int _open(int file)
{
    return (-1);
}

int _close(int file)
{
    return (-1);
}

int _fstat(int file, void *st)
{
    return (-1);
}

int _isatty(int file)
{
    return (-1);
}

int _lseek(int file, int ptr, int dir)
{
    return (-1);
}

int _sbrk(int incr)
{
    return (-1);
}

int _read(int file, char *ptr, int len)
{
    return (-1);
}

int _write(int file, char *ptr, int len)
{
    return (-1);
}

int _kill(int pid)
{
    return (-1);
}

int _getpid()
{
    return (-1);
}

static cpu_usage_t sys_port_interrupt_cpu_usage_get(void)
{
#if defined(FAMILY_SAM)
    return (((cpu_usage_t)100 * sys.interrupt.time) /
            (SAM_TC0->CHANNEL[0].CV - sys.interrupt.start));
#else
    return (0);
#endif
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
#if defined(FAMILY_SAM)
    sys.interrupt.start = SAM_TC0->CHANNEL[0].CV;
    sys.interrupt.time = 0;
#endif
}
