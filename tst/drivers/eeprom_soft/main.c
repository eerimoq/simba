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

#if defined(ARCH_LINUX)
#    define DEVICE_INDEX                         0
#    define FLASH_ADDRESS                   0x0000
#    define FLASH_SIZE                       0x800
#    define CHUNK_SIZE                       0x100

static uint8_t flash_buf[FLASH_SIZE];

#elif defined(FAMILY_SPC5)
#    define DEVICE_INDEX                         1
#    define FLASH_ADDRESS      SPC5_DFLASH_ADDRESS
#    define FLASH_SIZE                      0x8000
#    define CHUNK_SIZE                      0x1000
#else
#    error "Unsupported target."
#endif

static struct eeprom_soft_block_t blocks[2] = {
    {
        .address = FLASH_ADDRESS,
        .size = FLASH_SIZE / 2
    },
    {
        .address = FLASH_ADDRESS + FLASH_SIZE / 2,
        .size = FLASH_SIZE / 2
    }
};

static struct flash_driver_t flash;
static struct eeprom_soft_driver_t eeprom_soft;

static int test_init(struct harness_t *harness_p)
{
    struct flash_device_t *dev_p;

    BTASSERT(eeprom_soft_module_init() == 0);
    BTASSERT(eeprom_soft_module_init() == 0);

    dev_p = &flash_device[DEVICE_INDEX];

    BTASSERT(flash_init(&flash, dev_p) == 0);
    BTASSERT(flash_erase(&flash, blocks[0].address, blocks[0].size) == 0);
    BTASSERT(flash_erase(&flash, blocks[1].address, blocks[1].size) == 0);
    BTASSERT(eeprom_soft_init(&eeprom_soft,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              CHUNK_SIZE) == 0);

    return (0);
}

static int test_format_mount(struct harness_t *harness_p)
{
    BTASSERT(eeprom_soft_mount(&eeprom_soft) == -1);
    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);
    BTASSERT(eeprom_soft_mount(&eeprom_soft) == 0);

    return (0);
}

static int test_read_write_sizes(struct harness_t *harness_p)
{
    int i;
    uint8_t buf[55];

    /* Read various sizes at various alignments from a blank
       EEPROM. */
    memset(&buf[0], 0, sizeof(buf));

    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[0],  0,  1) ==  1);
    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[1],  1,  2) ==  2);
    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[3],  3,  3) ==  3);
    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[6],  6,  4) ==  4);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[10], 10,  5) ==  5);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[15], 15,  6) ==  6);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[21], 21,  7) ==  7);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[28], 28,  8) ==  8);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[36], 36,  9) ==  9);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[45], 45, 10) == 10);

    for (i = 0; i < membersof(buf); i++) {
        BTASSERT(buf[i] == 0xff, "[%d]: %02x", i, buf[i]);
    }

    /* Write various sizes at various alignments. */
    for (i = 0; i < membersof(buf); i++) {
        buf[i] = i;
    }

    BTASSERT(eeprom_soft_write(&eeprom_soft,  0,  &buf[0],  1) ==  1);
    BTASSERT(eeprom_soft_write(&eeprom_soft,  1,  &buf[1],  2) ==  2);
    BTASSERT(eeprom_soft_write(&eeprom_soft,  3,  &buf[3],  3) ==  3);
    BTASSERT(eeprom_soft_write(&eeprom_soft,  6,  &buf[6],  4) ==  4);
    BTASSERT(eeprom_soft_write(&eeprom_soft, 10, &buf[10],  5) ==  5);
    BTASSERT(eeprom_soft_write(&eeprom_soft, 15, &buf[15],  6) ==  6);
    BTASSERT(eeprom_soft_write(&eeprom_soft, 21, &buf[21],  7) ==  7);
    BTASSERT(eeprom_soft_write(&eeprom_soft, 28, &buf[28],  8) ==  8);
    BTASSERT(eeprom_soft_write(&eeprom_soft, 36, &buf[36],  9) ==  9);
    BTASSERT(eeprom_soft_write(&eeprom_soft, 45, &buf[45], 10) == 10);

    /* Read and verify the written data. */
    memset(&buf[0], -1, sizeof(buf));

    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[0],  0,  1) ==  1);
    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[1],  1,  2) ==  2);
    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[3],  3,  3) ==  3);
    BTASSERT(eeprom_soft_read(&eeprom_soft,  &buf[6],  6,  4) ==  4);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[10], 10,  5) ==  5);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[15], 15,  6) ==  6);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[21], 21,  7) ==  7);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[28], 28,  8) ==  8);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[36], 36,  9) ==  9);
    BTASSERT(eeprom_soft_read(&eeprom_soft, &buf[45], 45, 10) == 10);

    for (i = 0; i < membersof(buf); i++) {
        BTASSERT(buf[i] == i, "[%d]: %02x", i, buf[i]);
    }

    return (0);
}

static int test_read_write_low_high(struct harness_t *harness_p)
{
    uint8_t byte;

    /* Lowest address. */
    byte = 1;
    BTASSERT(eeprom_soft_write(&eeprom_soft,
                               0,
                               &byte,
                               sizeof(byte)) == sizeof(byte));

    byte = 0;
    BTASSERT(eeprom_soft_read(&eeprom_soft,
                              &byte,
                              0,
                              sizeof(byte)) == sizeof(byte));
    BTASSERT(byte == 1);

    /* Highest address. */
    byte = 2;
    BTASSERT(eeprom_soft_write(&eeprom_soft,
                               CHUNK_SIZE - 9,
                               &byte,
                               sizeof(byte)) == sizeof(byte));

    byte = 0;
    BTASSERT(eeprom_soft_read(&eeprom_soft,
                              &byte,
                              CHUNK_SIZE - 9,
                              sizeof(byte)) == sizeof(byte));

    return (0);
}

static int test_read_write_bad_address(struct harness_t *harness_p)
{
    uint8_t buf[2];

    memset(&buf[0], 0, sizeof(buf));

    /* Start address outside the EEPROM. */
    BTASSERT(eeprom_soft_read(&eeprom_soft,
                              &buf[0],
                              CHUNK_SIZE - 8,
                              sizeof(buf)) == -EINVAL);

    BTASSERT(eeprom_soft_write(&eeprom_soft,
                               CHUNK_SIZE - 8,
                               &buf[0],
                               sizeof(buf)) == -EINVAL);

    /* End address outside the EEPROM. */
    BTASSERT(eeprom_soft_read(&eeprom_soft,
                              &buf[0],
                              CHUNK_SIZE - 9,
                              sizeof(buf)) == -EINVAL);

    BTASSERT(eeprom_soft_write(&eeprom_soft,
                               CHUNK_SIZE - 9,
                               &buf[0],
                               sizeof(buf)) == -EINVAL);

    return (0);
}

static int test_mount_corrupt_data(struct harness_t *harness_p)
{
    uint8_t byte;

    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);

    byte = 0x01;
    BTASSERT(flash_write(&flash,
                         FLASH_ADDRESS + 8,
                         &byte,
                         sizeof(byte)) == sizeof(byte));

    BTASSERT(eeprom_soft_mount(&eeprom_soft) == -1);

    return (0);
}

#if defined(ARCH_LINUX)

static int test_mount_corrupt_header_valid(struct harness_t *harness_p)
{
    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);

    /* Overwrite parts of the valid flag. */
    flash_buf[7] = 1;

    BTASSERT(eeprom_soft_mount(&eeprom_soft) == -1);

    return (0);
}

static int test_mount_corrupt_header_crc(struct harness_t *harness_p)
{
    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);

    /* Overwrite parts of the crc. */
    flash_buf[0] = 1;

    BTASSERT(eeprom_soft_mount(&eeprom_soft) == -1);

    return (0);
}

static int test_mount_flash_read_fails(struct harness_t *harness_p)
{
    ssize_t res;

    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);

    res = -1;
    harness_mock_write("flash_read(): return (res)", &res, sizeof(res));
    BTASSERT(eeprom_soft_mount(&eeprom_soft) == -1);

    return (0);
}

static int test_mount_after_write(struct harness_t *harness_p)
{
    uint8_t byte;

    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);
    BTASSERT(eeprom_soft_mount(&eeprom_soft) == 0);

    byte = 1;
    BTASSERT(eeprom_soft_write(&eeprom_soft,
                               0,
                               &byte,
                               sizeof(byte)) == sizeof(byte));

    BTASSERT(eeprom_soft_mount(&eeprom_soft) == 0);

    return (0);
}

static int test_write_flash_read_fails_blank_chunk(struct harness_t *harness_p)
{
    uint8_t byte;
    ssize_t res;

    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);
    BTASSERT(eeprom_soft_mount(&eeprom_soft) == 0);

    /* Flash read fails checking blank chunk. */
    res = -1;
    harness_mock_write("flash_read(): return (res)", &res, sizeof(res));

    byte = 1;
    BTASSERT(eeprom_soft_write(&eeprom_soft,
                               0,
                               &byte,
                               sizeof(byte)) == sizeof(byte));

    return (0);
}

static int test_write_blank_chunk_not_blank(struct harness_t *harness_p)
{
    uint8_t byte;

    BTASSERT(eeprom_soft_format(&eeprom_soft) == 0);
    BTASSERT(eeprom_soft_mount(&eeprom_soft) == 0);

    /* Overwrite a byte in the first blank chunk. */
    flash_buf[CHUNK_SIZE] = 0;

    byte = 1;
    BTASSERT(eeprom_soft_write(&eeprom_soft,
                               0,
                               &byte,
                               sizeof(byte)) == sizeof(byte));

    return (0);
}

static int test_format_flash_erase_fails(struct harness_t *harness_p)
{
    int res;

    /* Flash erase fails. */
    res = -1;
    harness_mock_write("flash_erase(): return (res)", &res, sizeof(res));

    BTASSERT(eeprom_soft_format(&eeprom_soft) == -1);

    return (0);
}

int __wrap_flash_module_init(void)
{
    return (0);
}

int __wrap_flash_init(struct flash_driver_t *self_p,
                      struct flash_device_t *dev_p)
{
    BTASSERT(self_p != NULL);
    BTASSERT(dev_p != NULL);

    memset(&flash_buf[0], 0, sizeof(flash_buf));

    return (0);
}

ssize_t __wrap_flash_read(struct flash_driver_t *self_p,
                          void *dst_p,
                          uintptr_t src,
                          size_t size)
{
    BTASSERT(self_p != NULL);
    BTASSERTI(src, <, FLASH_SIZE);
    BTASSERTI(size, <=, CHUNK_SIZE);

    ssize_t res;
    int res2;

    res2 = harness_mock_read("flash_read(): return (res)",
                            &res,
                            sizeof(res));

    if (res2 == -1) {
        memcpy(dst_p, &flash_buf[src], size);
        res = size;
    }

    return (res);
}

ssize_t __wrap_flash_write(struct flash_driver_t *self_p,
                           uintptr_t dst,
                           const void *src_p,
                           size_t size)
{
    BTASSERT(self_p != NULL);
    BTASSERT(dst < FLASH_SIZE);
    BTASSERT(size <= CHUNK_SIZE);

    memcpy(&flash_buf[dst], src_p, size);

    return (size);
}

int __wrap_flash_erase(struct flash_driver_t *self_p,
                       uintptr_t addr,
                       size_t size)
{
    BTASSERT(self_p != NULL);
    BTASSERT(addr < FLASH_SIZE);
    BTASSERT(size <= FLASH_SIZE);

    int res;
    int res2;

    res2 = harness_mock_read("flash_erase(): return (res)",
                            &res,
                            sizeof(res));

    if (res2 == -1) {
        memset(&flash_buf[addr], 0xff, size);
        res = 0;
    }

    return (res);
}

#endif

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_format_mount, "test_format_mount" },
        { test_read_write_sizes, "test_read_write_sizes" },
        { test_read_write_low_high, "test_read_write_low_high" },
        { test_read_write_bad_address, "test_read_write_bad_address" },
        { test_mount_corrupt_data, "test_mount_corrupt_data" },
#if defined(ARCH_LINUX)
        { test_mount_corrupt_header_valid, "test_mount_corrupt_header_valid" },
        { test_mount_corrupt_header_crc, "test_mount_corrupt_header_crc" },
        { test_mount_flash_read_fails, "test_mount_flash_read_fails" },
        { test_mount_after_write, "test_mount_after_write" },
        {
            test_write_flash_read_fails_blank_chunk,
            "test_write_flash_read_fails_blank_chunk"
        },
        { test_write_blank_chunk_not_blank, "test_write_blank_chunk_not_blank" },
        { test_format_flash_erase_fails, "test_format_flash_erase_fails" },
#endif
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
