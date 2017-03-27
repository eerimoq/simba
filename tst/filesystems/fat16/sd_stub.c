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
