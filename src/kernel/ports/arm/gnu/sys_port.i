/**
 * @file arm/gnu/thrd_port.i
 * @version 0.6.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

ISR(sys_tick)
{
    sys_tick();
}

static int sys_port_module_init(void)
{
#if defined(MCU_FAMILY_SAM)
    /* Setup the system tick timer. */
    SAM_ST->LOAD = SYSTEM_TIMER_LOAD_RELOAD(10000000 / CONFIG_SYSTEM_TICK_FREQUENCY);
    SAM_ST->CTRL = (SYSTEM_TIMER_CTRL_TICKINT
                    | SYSTEM_TIMER_CTRL_ENABLE);
#endif

    /* Enable interrupts. */
    asm volatile("cpsie i" : : : "memory");

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

void sys_stop(int error)
{
    return (exit(error));
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

static float sys_port_interrupt_cpu_usage_get(void)
{
#if defined(MCU_FAMILY_SAM)
    return ((100.0 * sys.interrupt.time) /
            (SAM_TC0->CHANNEL[0].CV - sys.interrupt.start));
#else
    return (0);
#endif
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
#if defined(MCU_FAMILY_SAM)
    sys.interrupt.start = SAM_TC0->CHANNEL[0].CV;
    sys.interrupt.time = 0;
#endif
}
