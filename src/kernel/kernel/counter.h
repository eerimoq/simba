/**
 * @file kernel/counter.h
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

#ifndef __KERNEL_COUNTER_H__
#define __KERNEL_COUNTER_H__

#include "simba.h"

/* Counter definition macros. */
#if defined(__SIMBA_GEN__)
#    define COUNTER_DEFINE(path, name) ..fs_counter.. path #name
#else
#    define COUNTER_DEFINE(path, name)          \
    long long counter_ ## name = 0;             \
    FS_COUNTER_CMD(name)
#endif

#define COUNTER(name) counter_ ## name

/* Increment given counter. */
#define COUNTER_INC(name, value) COUNTER(name) += (value)

#endif
