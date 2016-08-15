/**
 * @file arch/cc.h
 * @version 6.0.0
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

#ifndef __INET_ARCH_CC_H__
#define __INET_ARCH_CC_H__

#include <stdint.h>
#include "kernel/types.h"
#include "sys_port.h"

/* Types based on stdint.h */
typedef uint8_t            u8_t;
typedef int8_t             s8_t;
typedef uint16_t           u16_t;
typedef int16_t            s16_t;
typedef uint32_t           u32_t;
typedef int32_t            s32_t;
typedef uintptr_t          mem_ptr_t;

/* Define (sn)printf formatters for these lwIP types */
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"
#define SZT_F "lu"

/* ARM/LPC17xx is little endian only */
#if !defined(ARCH_LINUX)
#    define BYTE_ORDER LITTLE_ENDIAN
#endif

/* GCC tools (CodeSourcery) */
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(fld) fld
#define ALIGNED(n)  __attribute__((aligned (n)))

/* Used with IP headers only */
#define LWIP_CHKSUM_ALGORITHM   1

#define LWIP_PLATFORM_DIAG(msg) { std_printf msg; }
#define LWIP_PLATFORM_ASSERT(flag) { ASSERT(flag); }

#define LWIP_PLATFORM_HTONS(x)      htons(x)
#define LWIP_PLATFORM_HTONL(x)      htonl(x)

#endif
