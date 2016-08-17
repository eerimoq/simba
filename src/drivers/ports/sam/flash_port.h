/**
 * @file drivers/ports/sam/flash_port.h
 * @version 7.0.0
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

#ifndef __DRIVERS_FLASH_PORT_H__
#define __DRIVERS_FLASH_PORT_H__

#include <io.h>

struct flash_device_bank_t {
    int index;
    volatile struct sam_eefc_t *regs_p;
    uint32_t begin;
    uint32_t end;
    uint32_t page_size;
};

struct flash_device_t {
    struct flash_device_bank_t banks[2];
    struct sem_t sem;
};

struct flash_driver_t {
    struct flash_device_t *dev_p;
};

#endif
