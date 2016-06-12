/**
 * @file inet/sys_arch.h
 * @version 0.6.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __INET_ARCH_SYS_ARCH_H__
#define __INET_ARCH_SYS_ARCH_H__

#include "simba.h"

typedef struct sem_t sys_sem_t;
typedef struct sem_t sys_mutex_t;
typedef struct thrd_t* sys_thread_t;
typedef int sys_prot_t;

typedef struct {
    int valid;
    struct queue_t queue;
} sys_mbox_t;

#define SYS_MBOX_NULL               ((uint32_t) NULL)
#define sys_mbox_valid(x)           ((*x).valid)
#define sys_mbox_set_invalid(x)     ( (*x).valid = 0 )

#endif
