/**
 * @file drivers/canif.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms_p-> of the GNU Lesser General Public
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

#ifndef __DRIVERS_CANIF_H__
#define __DRIVERS_CANIF_H__

#include "simba.h"

#define CANIF_ID_INVALID           0xffffffff
#define CANIF_ID_EXTENDED_MASK     0x80000000
#define CANIF_ID_IS_EXTENDED(id)   ((id & CANIF_ID_EXTENDED_MASK) != 0)

struct canif_frame_t {
    uint32_t id;        /* Frame ID. */
    int size;           /* Number of bytes in data array. */
    int rtr;            /* Remote transmission request. */
    uint32_t timestamp; /* Receive timestamp. */
    uint8_t data[8];    /* Payload. */
};

struct canif_filter_t {
    int id;             /* Frame ID. */
};

struct canif_t {
    int (*read)(void *drv_p, struct canif_frame_t *frame_p);
    int (*write)(void *drv_p, const struct canif_frame_t *frame_p);
};

#endif
