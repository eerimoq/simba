/**
 * @file linux/gnu/thrd_port.i
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

#include <pthread.h>

struct sys_port_t {
};

static void sys_port_lock()
{
}

static void sys_port_unlock()
{
}

static void sys_port_lock_irq()
{
}

static void sys_port_unlock_irq()
{
}

int sys_port_module_init(void)
{
    /* TODO: should be called from system tick interrupt. */
    sys_tick();
    return (0);
}

void sys_stop(int error)
{
    return (exit(error));
}

/* newlib stub functions. */
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
