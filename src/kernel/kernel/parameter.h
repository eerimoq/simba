/**
 * @file kernel/parameter.h
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

#ifndef __KERNEL_PARAMETER_H__
#define __KERNEL_PARAMETER_H__

#include "simba.h"

/* Add a command to the file system with given callback. */
#if defined(__SIMBA_GEN__)
#    define FS_PARAMETER_DEFINE(path, name, type, default_value) \
    ..fs_parameter.. path #name #type
#else
#    define FS_PARAMETER_DEFINE(path, name, type, default_value)   \
    type fs_parameter_ ## name = default_value;                 \
    FS_PARAMETER_CMD(name, type)
#endif

/* Get and set parameter value. */
#define FS_PARAMETER(name) fs_parameter_ ## name

#endif
