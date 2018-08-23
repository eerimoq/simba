/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

#define TYPE_SIZE                                           2
#define SIZE_SIZE                                           2
#define MAXIMUM_PAYLOAD_SIZE                             1024
#define CRC_SIZE                                            2

#define PAYLOAD_OFFSET                (TYPE_SIZE + SIZE_SIZE)

/* Command types. */
#define COMMAND_TYPE_FAILED                                -1
#define COMMAND_TYPE_PING                                   1
#define COMMAND_TYPE_ERASE                                  2
#define COMMAND_TYPE_READ                                   3
#define COMMAND_TYPE_WRITE                                  4

#define PIC32_ETAP_FASTDATA ((volatile uint32_t *) 0xff200000)

static struct flash_driver_t flash;
static uint8_t buf[PAYLOAD_OFFSET + MAXIMUM_PAYLOAD_SIZE + CRC_SIZE + 2];

static ssize_t fast_data_read(uint8_t *buf_p, size_t size)
{
    uint32_t data;
    size_t number_of_words;
    size_t i;

    number_of_words = DIV_CEIL(size, 4);

    for (i = 0; i < number_of_words; i++) {
        data = *PIC32_ETAP_FASTDATA;
        buf_p[4 * i + 0] = (data >> 24);
        buf_p[4 * i + 1] = (data >> 16);
        buf_p[4 * i + 2] = (data >> 8);
        buf_p[4 * i + 3] = (data >> 0);
    }

    return (size);
}

static ssize_t fast_data_write(uint8_t *buf_p, size_t size)
{
    uint32_t data;
    size_t number_of_words;
    size_t i;

    number_of_words = DIV_CEIL(size, 4);

    for (i = 0; i < number_of_words; i++) {
        data = ((buf_p[4 * i + 0] << 24)
                | (buf_p[4 * i + 1] << 16)
                | (buf_p[4 * i + 2] << 8)
                | (buf_p[4 * i + 3] << 0));
        *PIC32_ETAP_FASTDATA = data;
    }

    return (size);
}

static int is_ram_range(uint32_t address, uint32_t size)
{
    return ((address >= PIC32MM_RAM_ADDRESS)
            && (((address + size) <= PIC32MM_RAM_END)));
}

static int is_flash_range(uint32_t address, uint32_t size)
{
    return ((address >= PIC32MM_FLASH_ADDRESS)
            && (((address + size) <= PIC32MM_FLASH_END)));
}

static int is_sfrs_range(uint32_t address, uint32_t size)
{
    return ((address >= PIC32MM_SFRS_ADDRESS)
            && (((address + size) <= PIC32MM_SFRS_END)));
}

static int is_boot_flash_range(uint32_t address, uint32_t size)
{
    return ((address >= PIC32MM_BOOT_FLASH_ADDRESS)
            && (((address + size) <= PIC32MM_BOOT_FLASH_END)));
}

static int is_configuration_bits_range(uint32_t address, uint32_t size)
{
    return ((address >= PIC32MM_CONFIGURATION_BITS_ADDRESS)
            && (((address + size) <= PIC32MM_CONFIGURATION_BITS_END)));
}

static ssize_t handle_erase(uint8_t *buf_p, size_t size)
{
    uint32_t address;

    if (size < 8) {
        return (-EMSGSIZE);
    }

    address = (buf_p[0] << 24 | (buf_p[1] << 16) | (buf_p[2] << 8) | buf_p[3]);
    size = (buf_p[4] << 24 | (buf_p[5] << 16) | (buf_p[6] << 8) | buf_p[7]);

    if (size == 0) {
        return (-EINVAL);
    }

    if (is_flash_range(address, size) || is_boot_flash_range(address, size)) {
        return (flash_erase(&flash, address, size));
    } else {
        return (-ERANGE);
    }
}

static ssize_t handle_read(uint8_t *buf_p, size_t size)
{
    uint32_t address;
    size_t i;
    uint8_t *dst_p;
    uint8_t *src_p;

    if (size != 8) {
        return (-EMSGSIZE);
    }

    address = (buf_p[0] << 24 | (buf_p[1] << 16) | (buf_p[2] << 8) | buf_p[3]);
    size = (buf_p[4] << 24 | (buf_p[5] << 16) | (buf_p[6] << 8) | buf_p[7]);

    if (size > MAXIMUM_PAYLOAD_SIZE) {
        return (-EINVAL);
    }

    if (!(is_flash_range(address, size)
          || is_boot_flash_range(address, size)
          || is_ram_range(address, size)
          || is_configuration_bits_range(address, size)
          || is_sfrs_range(address, size))) {
        return (-EINVAL);
    }

    dst_p = buf_p;
    src_p = (uint8_t *)address;

    for (i = 0; i < size; i++) {
        dst_p[i] = src_p[i];
    }

    return (size);
}

static ssize_t handle_write(uint8_t *buf_p, size_t size)
{
    uint32_t address;
    ssize_t res;

    if (size < 8) {
        return (-EMSGSIZE);
    }

    address = (buf_p[0] << 24 | (buf_p[1] << 16) | (buf_p[2] << 8) | buf_p[3]);
    size = (buf_p[4] << 24 | (buf_p[5] << 16) | (buf_p[6] << 8) | buf_p[7]);

    if (size > MAXIMUM_PAYLOAD_SIZE) {
        return (-EINVAL);
    }

    if (is_flash_range(address, size) || is_boot_flash_range(address, size)) {
        res = flash_write(&flash, address, &buf_p[8], size);
    } else {
        res = -ERANGE;
    }

    return (res == size ? 0 : res);
}

static ssize_t handle_command(uint8_t *buf_p, size_t size)
{
    ssize_t res;
    int type;

    type = ((buf_p[0] << 8) | buf_p[1]);

    switch (type) {

    case COMMAND_TYPE_PING:
        res = 0;
        break;

    case COMMAND_TYPE_ERASE:
        res = handle_erase(&buf[PAYLOAD_OFFSET], size);
        break;

    case COMMAND_TYPE_READ:
        res = handle_read(&buf[PAYLOAD_OFFSET], size);
        break;

    case COMMAND_TYPE_WRITE:
        res = handle_write(&buf[PAYLOAD_OFFSET], size);
        break;

    default:
        res = -ENOCOMMAND;
        break;
    }

    return (res);
}

static ssize_t read_command_request(uint8_t *buf_p)
{
    ssize_t size;
    uint16_t crc;

    /* Read type and size. */
    fast_data_read(&buf_p[0], PAYLOAD_OFFSET);

    size = ((buf_p[2] << 8) | buf_p[3]);

    if (size > MAXIMUM_PAYLOAD_SIZE) {
        return (-EINVAL);
    }

    fast_data_read(&buf_p[4], size + 2);
    crc = crc_ccitt(0xffff, &buf_p[0], PAYLOAD_OFFSET + size);

    if (crc != ((buf_p[size + 4] << 8) | buf_p[size + 5])) {
        buf_p[6] = (crc >> 8);
        buf_p[7] = crc;

        return (-EBADCRC);
    }

    return (size);
}

static void write_command_response(uint8_t *buf_p, ssize_t size)
{
    uint16_t crc;

    /* Failure. */
    if (size < 0) {
        buf_p[0] = (COMMAND_TYPE_FAILED >> 8);
        buf_p[1] = COMMAND_TYPE_FAILED;
        buf_p[4] = (size >> 24);
        buf_p[5] = (size >> 16);
        buf_p[6] = (size >> 8);
        buf_p[7] = (size >> 0);
        size = 4;
    }

    buf_p[2] = (size >> 8);
    buf_p[3] = size;

    size += PAYLOAD_OFFSET;

    crc = crc_ccitt(0xffff, buf_p, size);

    buf_p[size] = (crc >> 8);
    buf_p[size + 1] = crc;
    size += CRC_SIZE;

    fast_data_write(buf_p, size);
}

int main()
{
    ssize_t size;

    flash_module_init();
    flash_init(&flash, &flash_device[0]);

    while (1) {
        size = read_command_request(&buf[0]);

        if (size >= 0) {
            size = handle_command(&buf[0], size);
        }

        write_command_response(&buf[0], size);
    }

    return (0);
}
