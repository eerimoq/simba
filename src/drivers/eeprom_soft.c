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

/**
 * Valid pattern in the header.
 */
#define VALID_PATTERN                                  0xa5c3

struct chunk_header_t {
    uint32_t crc;
    int16_t revision;
    uint16_t valid;
} PACKED;

/**
 * Calculate the crc of the chunk at given address.
 */
static uint32_t calculate_chunk_crc(struct eeprom_soft_driver_t *self_p,
                                    uint32_t *crc_p,
                                    uintptr_t address)
{
    ssize_t size;
    uint8_t buf[8];
    size_t offset;

    offset = sizeof(struct chunk_header_t);
    *crc_p = 0;

    while (offset < self_p->chunk_size) {
        size = flash_read(self_p->flash_p,
                          &buf[0],
                          address + offset,
                          sizeof(buf));

        if (size != sizeof(buf)) {
            return (-1);
        }

        *crc_p = crc_32(*crc_p, &buf[0], sizeof(buf));
        offset += sizeof(buf);
    }

    return (0);
}

/**
 * Check if chunk at given address is valid.
 */
static int is_valid_chunk(struct eeprom_soft_driver_t *self_p,
                          uintptr_t address)
{
    struct chunk_header_t header;
    ssize_t res;
    uint32_t crc;

    res = flash_read(self_p->flash_p,
                     &header,
                     address,
                     sizeof(header));

    if (res != sizeof(header)) {
        return (0);
    }

    /* Check the valid flag. */
    if (header.valid != VALID_PATTERN) {
        return (0);
    }

    /* Check the CRC. */
    if (calculate_chunk_crc(self_p, &crc, address) != 0) {
        return (-1);
    }

    if (crc != header.crc) {
        return (0);
    }

    return (1);
}

/**
 * Check if a revision is later than another.
 */
static int is_later_revision(int16_t revision_1, int16_t revision_2)
{
    if (revision_1 > revision_2) {
        return ((revision_1 - revision_2) < 0x4000);
    } else {
        return (!((revision_2 - revision_1) < 0x4000));
    }
}

/**
 * Check if given chunk is blank.
 */
static int is_blank_chunk(struct eeprom_soft_driver_t *self_p,
                          uintptr_t address)
{
    ssize_t size;
    int i;
    uint8_t byte;

    for (i = 0; i < self_p->chunk_size; i++) {
        size = flash_read(self_p->flash_p,
                          &byte,
                          address + i,
                          sizeof(byte));

        if (size != sizeof(byte)) {
            return (-1);
        }

        if (byte != 0xff) {
            return (0);
        }
    }

    return (1);
}

/**
 * Get a black chunk.
 */
static int get_blank_chunk(struct eeprom_soft_driver_t *self_p,
                           const struct eeprom_soft_block_t **block_pp,
                           uintptr_t *chunk_address_p)
{
    int res;
    const struct eeprom_soft_block_t *block_p;
    uintptr_t chunk_address;

    res = -1;

    /* First check if the next chunk in current block is blank. */
    block_p = self_p->current.block_p;
    chunk_address = self_p->current.chunk_address;
    chunk_address += self_p->chunk_size;

    if ((chunk_address < (block_p->address + block_p->size))
        && (is_blank_chunk(self_p, chunk_address) == 1)) {
        res = 0;
    } else {
        block_p++;

        if (block_p == &self_p->blocks_p[self_p->number_of_blocks]) {
            block_p = &self_p->blocks_p[0];
        }

        if (flash_erase(self_p->flash_p,
                        block_p->address,
                        block_p->size) != 0) {
            return (-1);
        }

        chunk_address = block_p->address;
        res = 0;
    }

    if (res == 0) {
        *block_pp = block_p;
        *chunk_address_p = chunk_address;
    }

    return (res);
}

/**
 * Write the header to flash for given chunk.
 */
static int write_header(struct eeprom_soft_driver_t *self_p,
                        uintptr_t chunk_address,
                        int16_t revision)
{
    ssize_t size;
    struct chunk_header_t header;

    if (calculate_chunk_crc(self_p, &header.crc, chunk_address) != 0) {
        return (-1);
    }

    header.revision = revision;
    header.valid = VALID_PATTERN;

    size = flash_write(self_p->flash_p,
                       chunk_address,
                       &header,
                       sizeof(header));

    if (size != sizeof(header)) {
        return (-1);
    }

    return (0);
}

int eeprom_soft_module_init()
{
    return (flash_module_init());
}

int eeprom_soft_init(struct eeprom_soft_driver_t *self_p,
                     struct flash_driver_t *flash_p,
                     const struct eeprom_soft_block_t *blocks_p,
                     int number_of_blocks,
                     size_t chunk_size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(flash_p != NULL, EINVAL);
    ASSERTN(blocks_p != NULL, EINVAL);
    ASSERTN(number_of_blocks >= 2, EINVAL);

    self_p->flash_p = flash_p;
    self_p->blocks_p = blocks_p;
    self_p->number_of_blocks = number_of_blocks;
    self_p->chunk_size = chunk_size;
    self_p->current.block_p = NULL;
    self_p->current.chunk_address = 0xffffffff;

    return (0);
}

int eeprom_soft_format(struct eeprom_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int i;

    for (i = 0; i < self_p->number_of_blocks; i++) {
        if (flash_erase(self_p->flash_p,
                        self_p->blocks_p[i].address,
                        self_p->blocks_p[i].size) != 0) {
            return (-1);
        }
    }

    return (write_header(self_p, self_p->blocks_p[0].address, 0));
}

int eeprom_soft_mount(struct eeprom_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int i;
    int j;
    uintptr_t chunk_address;
    const struct eeprom_soft_block_t *block_p;
    int number_of_chunks;
    struct chunk_header_t header;
    ssize_t size;
    int16_t latest_revision;
    const struct eeprom_soft_block_t *latest_block_p;
    uintptr_t latest_chunk_address;

    /* Find the most recently written chunk, as given by the
       revision. */
    latest_revision = -1;

    for (i = 0; i < self_p->number_of_blocks; i++) {
        block_p = &self_p->blocks_p[i];
        number_of_chunks = (block_p->size / self_p->chunk_size);

        for (j = 0; j < number_of_chunks; j++) {
            chunk_address = (block_p->address + j * self_p->chunk_size);

            /* Read the header. */
            size = flash_read(self_p->flash_p,
                              &header,
                              chunk_address,
                              sizeof(header));

            if (size != sizeof(header)) {
                continue;
            }

            /* Check the valid flag. */
            if (header.valid != VALID_PATTERN) {
                continue;
            }

            /* Revision must be a positive integer. */
            if (header.revision & 0x8000) {
                continue;
            }

            /* Keep track of the latest revision chunk. */
            if (is_later_revision(header.revision, latest_revision) == 1) {
                latest_revision = header.revision;
                latest_block_p = block_p;
                latest_chunk_address = chunk_address;
            }
        }
    }

    /* Make sure the chunk is valid. */
    if (latest_revision != -1) {
        if (is_valid_chunk(self_p, latest_chunk_address) != 1) {
            return (-1);
        }

        self_p->current.block_p = latest_block_p;
        self_p->current.chunk_address = latest_chunk_address;
        self_p->current.revision = latest_revision;

        return (0);
    }

    return (-1);
}

ssize_t eeprom_soft_read(struct eeprom_soft_driver_t *self_p,
                         void *dst_p,
                         uintptr_t src,
                         size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dst_p != NULL, EINVAL);

    if (src >= self_p->chunk_size - 8) {
        return (-EINVAL);
    }

    if (src + size > self_p->chunk_size - 8) {
        return (-EINVAL);
    }

    src += (self_p->current.chunk_address + 8);

    return (flash_read(self_p->flash_p, dst_p, src, size));
}

ssize_t eeprom_soft_write(struct eeprom_soft_driver_t *self_p,
                          uintptr_t dst,
                          const void *src_p,
                          size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    const struct eeprom_soft_block_t *block_p;
    uint8_t buf[8];
    uintptr_t chunk_address;
    int16_t revision;
    uintptr_t offset;
    const uint8_t *u8_src_p;
    int overwrite_index;
    size_t overwrite_size;

    if (dst >= self_p->chunk_size - 8) {
        return (-EINVAL);
    }

    if (dst + size > self_p->chunk_size - 8) {
        return (-EINVAL);
    }

    if (get_blank_chunk(self_p, &block_p, &chunk_address) != 0) {
        return (-1);
    }

    /* Write to new chunk. */
    u8_src_p = src_p;

    for (offset = 0; offset < self_p->chunk_size - 8; offset += 8) {
        /* Read from old chunk. */
        if (flash_read(self_p->flash_p,
                       &buf[0],
                       self_p->current.chunk_address + offset + 8,
                       sizeof(buf)) != sizeof(buf)) {
            return (-1);
        }

        /* Overwrite given data region. */
        if ((dst < offset + 8) && (dst + size > offset)) {
            if (dst <= offset) {
                overwrite_index = 0;
            } else {
                overwrite_index = (dst - offset);
            }

            if (dst + size >= offset + 8) {
                overwrite_size = 8;
            } else {
                overwrite_size = (dst + size - offset);
            }

            overwrite_size -= overwrite_index;

            memcpy(&buf[overwrite_index], u8_src_p, overwrite_size);
            u8_src_p += overwrite_size;
        }

        /* Write to new chunk. */
        if (flash_write(self_p->flash_p,
                        chunk_address + offset + 8,
                        &buf[0],
                        sizeof(buf)) != sizeof(buf)) {
            return (-1);
        }
    }

    revision = (self_p->current.revision + 1);
    revision &= 0x7fff;

    if (write_header(self_p, chunk_address, revision) != 0) {
        return (-1);
    }

    /* Update the object with new chunk information. */
    self_p->current.block_p = block_p;
    self_p->current.chunk_address = chunk_address;
    self_p->current.revision++;

    return (size);
}
