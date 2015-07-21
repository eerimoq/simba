/**
 * @file linux/gnu/spin_port.i
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

#include <util/atomic.h>

static int spin_port_init(struct spin_lock_t *lock)
{
    return (0);
}

static int spin_port_destory(struct spin_lock_t *lock)
{
    return (0);
}

static void spin_port_lock(struct spin_lock_t *lock, spin_irq_t *irq)
{
    *irq = (SREG & 0x80);
    asm volatile ("cli" ::: "memory");
}

static void spin_port_unlock(struct spin_lock_t *lock, spin_irq_t *irq)
{
    __iRestore(irq);
}

static void spin_port_lock_irq(struct spin_lock_t *lock)
{
    UNUSED(lock);
}

static void spin_port_unlock_irq(struct spin_lock_t *lock)
{
    UNUSED(lock);
}
