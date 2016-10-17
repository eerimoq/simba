/**
 * @file esp/gnu/compiler.h
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

#ifndef __KERNEL_COMPILER_H__
#define __KERNEL_COMPILER_H__

/* Put far data in RAM for now. */
#define FAR

typedef const FAR char *far_string_t;

/**
 * @brief No special storage for string literals.
 */
#define FSTR(s) s

#define PACKED __attribute__((packed))

#endif
