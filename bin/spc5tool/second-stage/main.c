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

#define TYPE_SIZE                                           1
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

static struct flash_driver_t cflash;
static struct flash_driver_t dflash;
static uint8_t buf[PAYLOAD_OFFSET + MAXIMUM_PAYLOAD_SIZE + CRC_SIZE];

static int is_cflash_range(uint32_t address, uint32_t size)
{
    return ((address >= SPC5_CFLASH_ADDRESS)
            && (((address + size) <=
                 (SPC5_CFLASH_ADDRESS + SPC5_CFLASH_SIZE))));
}

static int is_dflash_range(uint32_t address, uint32_t size)
{
    return ((address >= SPC5_DFLASH_ADDRESS)
            && (((address + size) <=
                 (SPC5_DFLASH_ADDRESS + SPC5_DFLASH_SIZE))));
}

static ssize_t handle_erase(uint8_t *buf_p, size_t size)
{
    uint32_t address;

    if (size < 8) {
        return (-1);
    }

    address = (buf_p[0] << 24 | (buf_p[1] << 16) | (buf_p[2] << 8) | buf_p[3]);
    size = (buf_p[4] << 24 | (buf_p[5] << 16) | (buf_p[6] << 8) | buf_p[7]);

    if (size == 0) {
        return (-1);
    }

    if (is_cflash_range(address, size)) {
        return (flash_erase(&cflash, address, size));
    } else if (is_dflash_range(address, size)) {
        return (flash_erase(&dflash, address, size));
    } else {
        return (-1);
    }
}

static ssize_t handle_read(uint8_t *buf_p, size_t size)
{
    uint32_t address;
    size_t i;
    uint8_t *dst_p;
    uint8_t *src_p;

    if (size != 8) {
        return (-1);
    }

    address = (buf_p[0] << 24 | (buf_p[1] << 16) | (buf_p[2] << 8) | buf_p[3]);
    size = (buf_p[4] << 24 | (buf_p[5] << 16) | (buf_p[6] << 8) | buf_p[7]);

    if (size > (MAXIMUM_PAYLOAD_SIZE - 8)) {
        return (-1);
    }

    if (!is_cflash_range(address, size) && !is_dflash_range(address, size)) {
        return (-1);
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
        return (-1);
    }

    address = (buf_p[0] << 24 | (buf_p[1] << 16) | (buf_p[2] << 8) | buf_p[3]);
    size = (buf_p[4] << 24 | (buf_p[5] << 16) | (buf_p[6] << 8) | buf_p[7]);

    if (size > (MAXIMUM_PAYLOAD_SIZE - 8)) {
        return (-1);
    }

    if (is_cflash_range(address, size)) {
        res = flash_write(&cflash, address, &buf_p[8], size);
    } else if (is_dflash_range(address, size)) {
        res = flash_write(&dflash, address, &buf_p[8], size);
    } else {
        res = -1;
    }

    return (res == size ? 0 : -1);
}

static ssize_t handle_command(uint8_t *buf_p, size_t size)
{
    ssize_t res;
    int type;

    type = buf[0];

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
        res = -1;
        break;
    }

    return (res);
}

static ssize_t read_command_request(uint8_t *buf_p)
{
    ssize_t size;
    uint16_t crc;

    /* Read type and size. */
    uart_device_read(&uart_device[0], &buf_p[0], PAYLOAD_OFFSET);

    size = ((buf_p[1] << 8) | buf_p[2]);

    if (size > MAXIMUM_PAYLOAD_SIZE) {
        return (-1);
    }

    uart_device_read(&uart_device[0], &buf_p[3], size + 2);
    crc = crc_ccitt(0xffff, &buf_p[0], PAYLOAD_OFFSET + size);

    if (crc != ((buf_p[size + 3] << 8) | buf_p[size + 4])) {
        return (-1);
    }

    return (size);
}

static void write_command_response(uint8_t *buf_p, ssize_t size)
{
    uint16_t crc;

    /* Failure. */
    if (size < 0) {
        buf_p[0] = COMMAND_TYPE_FAILED;
        size = 0;
    }

    buf_p[1] = (size >> 8);
    buf_p[2] = size;

    size += PAYLOAD_OFFSET;

    crc = crc_ccitt(0xffff, buf_p, size);

    buf_p[size] = (crc >> 8);
    buf_p[size + 1] = crc;
    size += CRC_SIZE;

    uart_device_write(&uart_device[0], buf_p, size);
}

int main()
{
    ssize_t size;

    flash_module_init();

    uart_device_start(&uart_device[0], BAUDRATE);

    flash_init(&cflash, &flash_device[0]);
    flash_init(&dflash, &flash_device[1]);

    while (1) {
        size = read_command_request(&buf[0]);

        if (size >= 0) {
            size = handle_command(&buf[0], size);
        }

        write_command_response(&buf[0], size);
    }

    return (0);
}
