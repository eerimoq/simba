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

static int spin_port_init(struct spin_lock_t *lock)
{
    return (pthread_mutex_init(&lock->mutex, NULL));
}

static int spin_port_destory(struct spin_lock_t *lock)
{
    return (pthread_mutex_destroy(&lock->mutex));
}

static void spin_port_lock(struct spin_lock_t *lock, spin_irq_t *irq)
{
    pthread_mutex_lock(&lock->mutex);
}

static void spin_port_unlock(struct spin_lock_t *lock, spin_irq_t *irq)
{
    pthread_mutex_unlock(&lock->mutex);
}

static void spin_port_lock_irq(struct spin_lock_t *lock)
{
    pthread_mutex_lock(&lock->mutex);
}

static void spin_port_unlock_irq(struct spin_lock_t *lock)
{
    pthread_mutex_unlock(&lock->mutex);
}
