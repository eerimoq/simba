/**
 * @file ports/linux/sd_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

static int sd_port_init(struct sd_driver_t *drv_p)
{
    drv_p->file_p = fopen("sdcard", "r+b");

    return (drv_p->file_p == NULL);
}

static ssize_t sd_port_read(struct sd_driver_t *drv_p,
                            void *dst_p,
                            uint32_t src_block)
{
    size_t block_start;

    /* Find given block. */
    block_start = (SD_BLOCK_SIZE * src_block);

    if (fseek(drv_p->file_p, block_start, SEEK_SET) != 0) {
        return (1);
    }

    return (fread(dst_p, 1, SD_BLOCK_SIZE, drv_p->file_p));
}

static ssize_t sd_port_write(struct sd_driver_t *drv_p,
                             uint32_t dst_block,
                             const void *src_p)
{
    size_t block_start;

    /* Find given block. */
    block_start = (SD_BLOCK_SIZE * dst_block);

    if (fseek(drv_p->file_p, block_start, SEEK_SET) != 0) {
        return (1);
    }

    return (fwrite(src_p, 1, SD_BLOCK_SIZE, drv_p->file_p));
}
