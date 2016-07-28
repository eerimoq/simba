/**
 * @file drivers/ports/sam/chipid_port.h
 * @version 3.1.1
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you chipid redistribute it and/or
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

#ifndef __DRIVERS_CHIPID_PORT_H__
#define __DRIVERS_CHIPID_PORT_H__

struct chipid_t {
    int version;
    int embedded_processor;
    int non_volatile_program_memory_size[2];
    int internal_sram_size;
    int architecture;
    int non_volatile_program_memory_type;
};

#endif
