/**
 * @file chipid_port.i
 * @version 4.0.0
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

#include "simba.h"

int chipid_port_read(struct chipid_t *id_p)
{
    uint32_t cidr;
    
    cidr = SAM_CHIPID->CIDR;
        
    id_p->version = CHIPID_CIDR_VERSION_GET(cidr);
    id_p->embedded_processor = CHIPID_CIDR_EPROC_GET(cidr);
    id_p->non_volatile_program_memory_size[0] = CHIPID_CIDR_NVPSIZ_GET(cidr);
    id_p->non_volatile_program_memory_size[1] = CHIPID_CIDR_NVPSIZ2_GET(cidr);
    id_p->internal_sram_size = CHIPID_CIDR_SRAMSIZ_GET(cidr);
    id_p->architecture = CHIPID_CIDR_ARCH_GET(cidr);
    id_p->non_volatile_program_memory_type = CHIPID_CIDR_NVPTYP_GET(cidr);

    return (0);
}
