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

#if CONFIG_EEPROM_SOFT == 1

/**
 * Valid pattern in the header.
 */
#define VALID_PATTERN                                  0xa5c3

#define CHUNK_HEADER_SIZE        sizeof(struct chunk_header_t)

struct chunk_header_t {
    uint32_t crc;
    uint16_t revision;
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
#if CONFIG_EEPROM_SOFT_CRC == CONFIG_EEPROM_SOFT_CRC_32
    uint32_t crc = 0;
#elif CONFIG_EEPROM_SOFT_CRC == CONFIG_EEPROM_SOFT_CRC_CCITT
    uint16_t crc = 0xffff;
#endif

    offset = CHUNK_HEADER_SIZE;

    while (offset < self_p->chunk_size) {
        size = flash_read(self_p->flash_p,
                          &buf[0],
                          address + offset,
                          sizeof(buf));

        if (size != sizeof(buf)) {
            return (-1);
        }

#if CONFIG_EEPROM_SOFT_CRC == CONFIG_EEPROM_SOFT_CRC_32
        crc = crc_32(crc, &buf[0], sizeof(buf));
#elif CONFIG_EEPROM_SOFT_CRC == CONFIG_EEPROM_SOFT_CRC_CCITT
        crc = crc_ccitt(crc, &buf[0], sizeof(buf));
#endif

        offset += sizeof(buf);

#if CONFIG_PREEMPTIVE_SCHEDULER == 0
        thrd_yield();
#endif
    }

    *crc_p = crc;

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
static int is_later_revision(uint16_t revision_1, uint16_t revision_2)
{
    if (revision_1 > revision_2) {
        return ((revision_1 - revision_2) < 0x8000);
    } else {
        return (!((revision_2 - revision_1) < 0x8000));
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

#if CONFIG_PREEMPTIVE_SCHEDULER == 0
        thrd_yield();
#endif
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
                        block_p->size) == 0) {
            chunk_address = block_p->address;
            res = 0;
        }
    }

    *block_pp = block_p;
    *chunk_address_p = chunk_address;

    return (res);
}

/**
 * Write the header to flash for given chunk.
 */
static int write_header(struct eeprom_soft_driver_t *self_p,
                        uintptr_t chunk_address,
                        uint16_t revision)
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

static ssize_t write_inner(struct eeprom_soft_driver_t *self_p,
                           uintptr_t dst,
                           const void *src_p,
                           size_t size)
{
    const struct eeprom_soft_block_t *block_p;
    uint8_t buf[8];
    uintptr_t chunk_address;
    uint16_t revision;
    uintptr_t offset;
    const uint8_t *u8_src_p;
    int overwrite_index;
    size_t overwrite_size;

    if (dst >= self_p->eeprom_size) {
        return (-EINVAL);
    }

    if (dst + size > self_p->eeprom_size) {
        return (-EINVAL);
    }

    if (get_blank_chunk(self_p, &block_p, &chunk_address) != 0) {
        return (-1);
    }

    /* Write to new chunk. */
    u8_src_p = src_p;

    for (offset = 0; offset < self_p->eeprom_size; offset += sizeof(buf)) {
        /* Read from old chunk. */
        if (flash_read(self_p->flash_p,
                       &buf[0],
                       self_p->current.chunk_address + CHUNK_HEADER_SIZE + offset,
                       sizeof(buf)) != sizeof(buf)) {
            return (-1);
        }

        /* Overwrite given data region. */
        if ((dst < offset + sizeof(buf)) && (dst + size > offset)) {
            if (dst <= offset) {
                overwrite_index = 0;
            } else {
                overwrite_index = (dst - offset);
            }

            if (dst + size >= offset + sizeof(buf)) {
                overwrite_size = sizeof(buf);
            } else {
                overwrite_size = (dst + size - offset);
            }

            overwrite_size -= overwrite_index;

            memcpy(&buf[overwrite_index], u8_src_p, overwrite_size);
            u8_src_p += overwrite_size;
        }

        /* Write to new chunk. */
        if (flash_write(self_p->flash_p,
                        chunk_address + CHUNK_HEADER_SIZE + offset,
                        &buf[0],
                        sizeof(buf)) != sizeof(buf)) {
            return (-1);
        }

#if CONFIG_PREEMPTIVE_SCHEDULER == 0
        thrd_yield();
#endif
    }

    revision = (self_p->current.revision + 1);

    if (write_header(self_p, chunk_address, revision) != 0) {
        return (-1);
    }

    /* Update the object with new chunk information. */
    self_p->current.block_p = block_p;
    self_p->current.chunk_address = chunk_address;
    self_p->current.revision = revision;

    return (size);
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
    self_p->eeprom_size = (chunk_size - CHUNK_HEADER_SIZE);
    self_p->current.block_p = NULL;
    self_p->current.chunk_address = 0xffffffff;

#if CONFIG_EEPROM_SOFT_SEMAPHORE == 1
    sem_init(&self_p->sem, 0, 1);
#endif

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

    int res;
    int i;
    int j;
    uintptr_t chunk_address;
    const struct eeprom_soft_block_t *block_p;
    int number_of_chunks;
    struct chunk_header_t header;
    ssize_t size;
    uint16_t latest_revision;
    const struct eeprom_soft_block_t *latest_block_p;
    uintptr_t latest_chunk_address;

    res = -1;
    latest_block_p = NULL;

    /* Find the most recently written chunk, as given by the
       revision. */
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

            /* Keep track of the latest revision chunk. */
            if ((latest_block_p == NULL)
                || (is_later_revision(header.revision, latest_revision) == 1)) {
                latest_revision = header.revision;
                latest_block_p = block_p;
                latest_chunk_address = chunk_address;
            }
        }
    }

    /* Make sure the chunk is valid. */
    if (latest_block_p != NULL) {
        if (is_valid_chunk(self_p, latest_chunk_address) == 1) {
            self_p->current.block_p = latest_block_p;
            self_p->current.chunk_address = latest_chunk_address;
            self_p->current.revision = latest_revision;
            res = 0;
        }
    }

    return (res);
}

ssize_t eeprom_soft_read(struct eeprom_soft_driver_t *self_p,
                         void *dst_p,
                         uintptr_t src,
                         size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dst_p != NULL, EINVAL);

    ssize_t res;

    if (src >= self_p->eeprom_size) {
        return (-EINVAL);
    }

    if (src + size > self_p->eeprom_size) {
        return (-EINVAL);
    }

#if CONFIG_EEPROM_SOFT_SEMAPHORE == 1
    sem_take(&self_p->sem, NULL);
#endif

    src += (self_p->current.chunk_address + CHUNK_HEADER_SIZE);
    res = flash_read(self_p->flash_p, dst_p, src, size);

#if CONFIG_EEPROM_SOFT_SEMAPHORE == 1
    sem_give(&self_p->sem, 1);
#endif

    return (res);
}

ssize_t eeprom_soft_write(struct eeprom_soft_driver_t *self_p,
                          uintptr_t dst,
                          const void *src_p,
                          size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    ssize_t res;

#if CONFIG_EEPROM_SOFT_SEMAPHORE == 1
    sem_take(&self_p->sem, NULL);
#endif

    res = write_inner(self_p, dst, src_p, size);

#if CONFIG_EEPROM_SOFT_SEMAPHORE == 1
    sem_give(&self_p->sem, 1);
#endif

    return (res);
}

#endif
