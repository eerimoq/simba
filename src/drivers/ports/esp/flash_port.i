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

#include "espressif/spi_flash.h"

#define FLASH_SECTOR_SIZE  SPI_FLASH_SEC_SIZE
#define FLASH_SECTOR_ALIGN 0xfffff000

extern char __rom_size;

static int flash_port_module_init(void)
{
    return (0);
}

static ssize_t flash_port_read(struct flash_driver_t *self_p,
                               void *dst_p,
                               uintptr_t src,
                               size_t size)
{
    ASSERTN(src < (uintptr_t)&__rom_size, EINVAL);
    ASSERTN(size <= ((uintptr_t)&__rom_size - src), EINVAL);

    uintptr_t aligned_src_begin;
    uintptr_t aligned_src_end;
    uint32_t alignment_data;
    uint32_t n;
    uint32_t left;
    uintptr_t dst;
    uint32_t buf[64];

    aligned_src_begin = (src + 3) & 0xfffffffc;
    aligned_src_end = (src + size) & 0xfffffffc;
    dst = (uintptr_t)dst_p;

    if (aligned_src_end < aligned_src_begin) {
        aligned_src_end = aligned_src_begin;
    }

    /* Read up to 3 bytes before the flash alignment begin address. */
    if (src < aligned_src_begin) {
        n = (aligned_src_begin - src);

        if (spi_flash_read(aligned_src_begin - 4,
                           &alignment_data,
                           4) != SPI_FLASH_RESULT_OK) {
            return (-1);
        }

        memcpy(dst_p, ((char *)&alignment_data) + 4 - n, n);
        dst += n;
    }

    /* Read the aligned data. */
    if (aligned_src_end > aligned_src_begin) {
        if ((dst & 0x3) == 0) {
            n = (aligned_src_end - aligned_src_begin);

            if (spi_flash_read(aligned_src_begin,
                               (uint32_t *)dst,
                               n) != SPI_FLASH_RESULT_OK) {
                return (-1);
            }

            dst += n;
        } else {
            /* The data in RAM is not aligned to a 4b boundary. */
            left = (aligned_src_end - aligned_src_begin);

            while (left > 0) {
                n = MIN(left, sizeof(buf));

                if (spi_flash_read(aligned_src_begin,
                                   &buf[0],
                                   n) != SPI_FLASH_RESULT_OK) {
                    return (-1);
                }

                memcpy((void *)dst, buf, n);
                left -= n;
                dst += n;
                aligned_src_begin += n;
            }
        }
    }

    /* Read up to 3 bytes after the flash alignment end address. */
    if ((src + size) > aligned_src_end) {
        n = (src + size - aligned_src_end);

        if (spi_flash_read(aligned_src_end,
                           &alignment_data,
                           4) != SPI_FLASH_RESULT_OK) {
            return (-1);
        }

        memcpy(dst_p + size - n, &alignment_data, n);
    }

    return (size);
}

static ssize_t flash_port_write(struct flash_driver_t *self_p,
                                uintptr_t dst,
                                const void *src_p,
                                size_t size)
{
    ASSERTN(dst < (uintptr_t)&__rom_size, EINVAL);
    ASSERTN(size <= ((uintptr_t)&__rom_size - dst), EINVAL);

    uintptr_t aligned_dst_begin;
    uintptr_t aligned_dst_end;
    uintptr_t src;
    uint32_t alignment_data;
    uint32_t n;
    uint32_t left;
    uint32_t buf[64];

    aligned_dst_begin = (dst + 3) & 0xfffffffc;
    aligned_dst_end = (dst + size) & 0xfffffffc;
    src = (uintptr_t)src_p;

    if (aligned_dst_end < aligned_dst_begin) {
        aligned_dst_end = aligned_dst_begin;
    }

    /* Write up to 3 bytes before flash alignment begin address. */
    if (dst < aligned_dst_begin) {
        n = (aligned_dst_begin - dst);
        alignment_data = 0xffffffff;
        memcpy((void *)((uintptr_t)&alignment_data + 4 - n), src_p, n);

        if (spi_flash_write(aligned_dst_begin - 4,
                            &alignment_data,
                            sizeof(alignment_data)) != SPI_FLASH_RESULT_OK) {
            return (-1);
        }

        src += n;
    }

    /* Write the aligned data. */
    if (aligned_dst_begin < aligned_dst_end) {
        if ((src & 0x3) == 0) {
            /* The data in RAM is aligned to a 4b boundary. */
            n = (aligned_dst_end - aligned_dst_begin);

            if (spi_flash_write(aligned_dst_begin,
                                (uint32_t *)src,
                                n) != SPI_FLASH_RESULT_OK) {
                return (-1);
            }

            src += n;
        } else {
            /* The data in RAM is not aligned to a 4b boundary. */
            left = (aligned_dst_end - aligned_dst_begin);

            while (left > 0) {
                n = MIN(left, sizeof(buf));
                memcpy(buf, (uint32_t *)src, n);

                if (spi_flash_write(aligned_dst_begin,
                                    &buf[0],
                                    n) != SPI_FLASH_RESULT_OK) {
                    return (-1);
                }

                left -= n;
                src += n;
                aligned_dst_begin += n;
            }
        }
    }

    /* Write up to 3 bytes after flash alignment end address. */
    if ((dst + size) > aligned_dst_end) {
        alignment_data = 0xffffffff;
        memcpy(&alignment_data, (void *)src, (dst + size - aligned_dst_end));

        if (spi_flash_write(aligned_dst_end,
                            &alignment_data,
                            sizeof(alignment_data)) != SPI_FLASH_RESULT_OK) {
            return (-1);
        }
    }

    return (size);
}

static int flash_port_erase(struct flash_driver_t *self_p,
                            uintptr_t addr,
                            uint32_t size)
{
    ASSERTN(addr < (uintptr_t)&__rom_size, EINVAL);
    ASSERTN(size <= ((uintptr_t)&__rom_size - addr), EINVAL);

    uintptr_t aligned_addr;
    int first_sector;
    int number_of_sectors;
    int i;

    aligned_addr = (addr & FLASH_SECTOR_ALIGN);
    first_sector = (addr / FLASH_SECTOR_SIZE);
    number_of_sectors = DIV_CEIL(addr - aligned_addr + size,
                                 FLASH_SECTOR_SIZE);

    for (i = 0; i < number_of_sectors; i++) {
        if (spi_flash_erase_sector(first_sector + i) != SPI_FLASH_RESULT_OK) {
            return (-1);
        }
    }

    return (0);
}
