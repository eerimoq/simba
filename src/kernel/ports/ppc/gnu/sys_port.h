/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#ifndef __KERNEL_SYS_PORT_H__
#define __KERNEL_SYS_PORT_H__

static inline uint32_t htonl(uint32_t v)
{
    return (v);
}

#define ntohl(v) htonl(v)

static inline uint16_t htons(uint16_t v)
{
    return (v);
}

#define ntohs(v) htons(v)

#define SYS_PORT_RESET_CAUSES                   \
    sys_reset_cause_lvd27_vreg_t,               \
        sys_reset_cause_lvd27_t,                \
        sys_reset_cause_lvd12_pd1_t,            \
        sys_reset_cause_lvd12_pd0_t,            \
        sys_reset_cause_flash_t,                \
        sys_reset_cause_lvd45_t,                \
        sys_reset_cause_cmu_fhl_t,              \
        sys_reset_cause_cmu_olr_t,              \
        sys_reset_cause_fmpll_t,                \
        sys_reset_cause_chkstop_t,              \
        sys_reset_cause_core_t

#define SYS_PORT_RESET_CAUSE_STRINGS_MAP        \
    "lvd27_vreg",                               \
        "lvd27",                                \
        "lvd12_pd1",                            \
        "lvd12_pd0",                            \
        "flash",                                \
        "lvd45",                                \
        "cmu_fhl",                              \
        "cmu_olr",                              \
        "fmpll",                                \
        "chkstop",                              \
        "core"

#endif
