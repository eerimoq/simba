/**
 * @file kernel/core.h
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

#ifndef __KERNEL_CORE_H__
#define __KERNEL_CORE_H__

#include "simba.h"
#include "core_port.h"

/**
 * Lock core by disabling interrupts.
 * @return Interrupt mask
 */
core_irq_t core_lock(void);

/**
 * Unlock core by restoring interrupts.
 */
void core_unlock(core_irq_t irq);

#endif
