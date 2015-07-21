/**
 * @file spin.c
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

#include "simba.h"

#include "spin_port.i"

int spin_module_init(void)
{
    return (0);
}

int spin_init(struct spin_lock_t *lock_p)
{
    return (spin_port_init(lock_p));
}

int spin_destory(struct spin_lock_t *lock_p)
{
    return (spin_port_destory(lock_p));
}

void spin_lock(struct spin_lock_t *lock_p, spin_irq_t *irq_p)
{
    spin_port_lock(lock_p, irq_p);
}

void spin_unlock(struct spin_lock_t *lock_p, spin_irq_t *irq_p)
{
    spin_port_unlock(lock_p, irq_p);
}

void spin_lock_irq(struct spin_lock_t *lock_p)
{
    spin_port_lock_irq(lock_p);
}

void spin_unlock_irq(struct spin_lock_t *lock_p)
{
    spin_port_unlock_irq(lock_p);
}
