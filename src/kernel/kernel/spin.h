/**
 * @file kernel/spin.h
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

#ifndef __KERNEL_SPIN_H__
#define __KERNEL_SPIN_H__

#include "simba.h"
#include "spin_port.h"

/**
 * Initialize module.
 * @return zero(0) or negative error code
 */
int spin_module_init(void);

/**
 * Initialize given spin lock.
 * @param[out] lock Spin lock to initialize.
 * @return zero(0) or negative error code.
 */
int spin_init(struct spin_lock_t *lock);

/**
 * Destory given spin lock.
 * @param[in] lock Spin lock to destroy.
 * @return zero(0) or negative error code.
 */
int spin_destory(struct spin_lock_t *lock);

/**
 * Disable interrupts and take given lock.
 * @param[in] lock Spin lock.
 * @param[out] irq Stored irq flags.
 */
void spin_lock(struct spin_lock_t *lock, spin_irq_t *irq);

/**
 * Release given lock and restore interrupt flags.
 * @param[in] lock Spin lock.
 * @param[out] irq Stored irq flags.
 */
void spin_unlock(struct spin_lock_t *lock, spin_irq_t *irq);

/**
 * Take given lock from interrupt context.
 * @param[in] lock Spin lock.
 */
void spin_lock_irq(struct spin_lock_t *lock);

/**
 * Release given lock from interrupt context.
 * @param[in] lock Spin lock.
 */
void spin_unlock_irq(struct spin_lock_t *lock);

#endif
