/**
 * @file sd_stub.c
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

#include "simba.h"

static FILE *file_p = NULL;

int sd_init(struct sd_driver_t *self_p,
            struct spi_driver_t *spi_p)
{
    file_p = fopen("sdcard", "r+b");
    
    return (file_p == NULL);
}

int sd_start(struct sd_driver_t *self_p)
{
    return (0);
}

int sd_stop(struct sd_driver_t *self_p)
{
    return (0);
}

ssize_t sd_read_block(struct sd_driver_t *self_p,
                      void *dst_p,
                      uint32_t src_block)
{
    size_t block_start;

    /* Find given block. */
    block_start = (SD_BLOCK_SIZE * src_block);

    if (fseek(file_p, block_start, SEEK_SET) != 0) {
        return (1);
    }

    return (fread(dst_p, 1, SD_BLOCK_SIZE, file_p));
}

ssize_t sd_write_block(struct sd_driver_t *self_p,
                       uint32_t dst_block,
                       const void *src_p)
{
    size_t block_start;

    /* Find given block. */
    block_start = (SD_BLOCK_SIZE * dst_block);

    if (fseek(file_p, block_start, SEEK_SET) != 0) {
        return (1);
    }

    if (fwrite(src_p, 1, SD_BLOCK_SIZE, file_p) != SD_BLOCK_SIZE) {
        return (-1);
    }

    fflush(file_p);

    return (SD_BLOCK_SIZE);
}
