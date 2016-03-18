/**
 * @file kernel/counter.h
 * @version 0.2.0
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

#ifndef __KERNEL_COUNTER_H__
#define __KERNEL_COUNTER_H__

#include "simba.h"

/**
 * Define a 64 bit debug counter with given name and file system path.
 *
 * @param[in] path Path of the counter in the debug file system.
 * @param[in] name Counter name.
 */
#if defined(__SIMBA_GEN__)
#    define FS_COUNTER_DEFINE(path, name) ..fs_counter.. path ..fs_separator.. #name
#else
#    define FS_COUNTER_DEFINE(path, name)          \
    long long counter_ ## name = 0;             \
    FS_COUNTER_CMD(name)
#endif

/**
 * Get the counter.
 *
 * @param[in] name Counter name. The same name as specified in
 *                 `FS_COUNTER_DEFINE()`.
 *
 * @return The counter.
 */
#define FS_COUNTER(name) counter_ ## name

/**
 * Increment a counter with given value.
 *
 * @param[in] name Counter name. The same name as specified in
 *                 `FS_COUNTER_DEFINE()`.
 * @param[in] value Value to add to given counter.
 */
#define FS_COUNTER_INC(name, value) FS_COUNTER(name) += (value)

#endif
