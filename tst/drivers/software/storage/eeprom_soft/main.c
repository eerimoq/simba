/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "drivers/storage/flash_mock.h"

struct header_t {
    uint32_t crc;
    uint16_t revision;
    uint16_t valid;
};

static const struct eeprom_soft_block_t blocks[2] = {
    { .address = 0x1000, .size = 0x800 },
    { .address = 0x2000, .size = 0x800 }
};
static struct flash_driver_t flash;

static int write_calculate_chunk_crc_blank(uintptr_t address,
                                           size_t size)
{
    uint8_t buf[8];

    memset(&buf[0], -1, sizeof(buf));

    mock_write_flash_read_seq(&buf[0],
                              address + 8,
                              sizeof(buf),
                              sizeof(buf),
                              (size - 8) / sizeof(buf));

    return (0);
}

static int write_write_header_blank(uintptr_t address,
                                    uint16_t revision,
                                    uint16_t valid)
{
    struct header_t header;

    write_calculate_chunk_crc_blank(address, 512);

    header.crc = 0xa3295e14;
    header.revision = revision;
    header.valid = valid;

    mock_write_flash_write(address,
                           &header,
                           sizeof(header),
                           sizeof(header));

    return (0);
}

static int write_read_header(uintptr_t address,
                             uint32_t crc,
                             uint16_t revision,
                             uint16_t valid)
{
    struct header_t header;

    header.crc = crc;
    header.revision = revision;
    header.valid = valid;

    mock_write_flash_read(&header,
                          address,
                          sizeof(header),
                          sizeof(header));

    return (0);
}

static int write_read_header_blank(uintptr_t address,
                                   uint16_t revision,
                                   uint16_t valid)
{
    return (write_read_header(address,
                              0xa3295e14,
                              revision,
                              valid));
}

static int write_mount_read_chunks(struct header_t headers[2][4])
{
    int i;
    int j;

    for (i = 0; i < membersof(blocks); i++) {
        for (j = 0; j < membersof(headers[0]); j++) {
            write_read_header(blocks[i].address + j * 512,
                              headers[i][j].crc,
                              headers[i][j].revision,
                              headers[i][j].valid);
        }
    }

    return (0);
}

static int write_mount_block_0_chunk_0_latest(void)
{
    struct header_t headers[2][4] = {
        /* Block 0. */
        {
            { .crc = 0xa3295e14, .revision = 0x0000, .valid = 0xa5c3 },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff }
        },
        /* Block 1. */
        {
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff }
        }
    };

    write_mount_read_chunks(headers);

    return (0);
}

static int write_get_blank_chunk_block_0_chunk_1_blank()
{
    uint8_t data;

    data = 0xff;

    mock_write_flash_read_seq(&data,
                              blocks[0].address + 512,
                              sizeof(data),
                              sizeof(data),
                              512);

    return (0);
}

static int test_module_init(void)
{
    mock_write_flash_module_init(0);
    BTASSERT(eeprom_soft_module_init() == 0);

    mock_write_flash_module_init(-1);
    BTASSERT(eeprom_soft_module_init() == -1);

    return (0);
}

static int test_format(void)
{
    struct eeprom_soft_driver_t eeprom;
    int i;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    for (i = 0; i < membersof(blocks); i++) {
        mock_write_flash_erase(blocks[i].address,
                               blocks[i].size,
                               0);
    }

    write_write_header_blank(blocks[0].address,
                             0,
                             0xa5c3);

    BTASSERT(eeprom_soft_format(&eeprom) == 0);

    return (0);
}

static int test_format_erase_fail(void)
{
    struct eeprom_soft_driver_t eeprom;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    mock_write_flash_erase(blocks[0].address,
                           blocks[0].size,
                           -1);

    BTASSERT(eeprom_soft_format(&eeprom) == -1);

    return (0);
}

static int test_mount(void)
{
    struct eeprom_soft_driver_t eeprom;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    /* Block (revision 0) is latest and valid. */
    write_mount_block_0_chunk_0_latest();
    write_read_header_blank(blocks[0].address,
                                 0,
                                 0xa5c3);
    write_calculate_chunk_crc_blank(blocks[0].address,
                                    512);

    BTASSERT(eeprom_soft_mount(&eeprom) == 0);

    return (0);
}

static int test_mount_flash_read_fail_all_chunks(void)
{
    struct eeprom_soft_driver_t eeprom;
    struct header_t header;
    int i;
    int j;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    for (i = 0; i < membersof(blocks); i++) {
        for (j = 0; j < 4; j++) {
            mock_write_flash_read(&header,
                                  blocks[i].address + j * 512,
                                  sizeof(header),
                                  -1);
        }
    }

    BTASSERT(eeprom_soft_mount(&eeprom) == -1);

    return (0);
}

static int test_mount_revision_1_later_than_0(void)
{
    struct eeprom_soft_driver_t eeprom;
    struct header_t headers[2][4] = {
        /* Block 0. */
        {
            { .crc = 0xa3295e14, .revision = 0x0000, .valid = 0xa5c3 },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xa3295e14, .revision = 0x0001, .valid = 0xa5c3 },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff }
        },
        /* Block 1. */
        {
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff }
        }
    };

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    write_mount_read_chunks(headers);
    write_read_header_blank(blocks[0].address + 1024,
                            1,
                            0xa5c3);
    write_calculate_chunk_crc_blank(blocks[0].address + 1024,
                                    512);

    BTASSERT(eeprom_soft_mount(&eeprom) == 0);

    return (0);
}

static int test_mount_revision_1_later_than_33000(void)
{
    struct eeprom_soft_driver_t eeprom;
    struct header_t headers[2][4] = {
        /* Block 0. */
        {
            { .crc = 0xa3295e14, .revision = 65000,  .valid = 0xa5c3 },
            { .crc = 0xa3295e14, .revision = 33000,  .valid = 0xa5c3 },
            { .crc = 0xa3295e14, .revision = 0x0001, .valid = 0xa5c3 },
            { .crc = 0xa3295e14, .revision = 48000,  .valid = 0xa5c3 }
        },
        /* Block 1. */
        {
            { .crc = 0xa3295e14, .revision = 44912,  .valid = 0xa5c3 },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff },
            { .crc = 0xffffffff, .revision = 0xffff, .valid = 0xffff }
        }
    };

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    write_mount_read_chunks(headers);
    write_read_header_blank(blocks[0].address + 1024,
                                 1,
                                 0xa5c3);
    write_calculate_chunk_crc_blank(blocks[0].address + 1024,
                                         512);

    BTASSERT(eeprom_soft_mount(&eeprom) == 0);

    return (0);
}

static int test_mount_is_valid_chunk_flash_read_fail(void)
{
    struct eeprom_soft_driver_t eeprom;
    struct header_t header;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    write_mount_block_0_chunk_0_latest();

    /* Header read of latest chunk fails. */
    mock_write_flash_read(&header,
                          blocks[0].address,
                          sizeof(header),
                          -1);

    BTASSERT(eeprom_soft_mount(&eeprom) == -1);

    return (0);
}

static int test_mount_is_valid_chunk_not_valid(void)
{
    struct eeprom_soft_driver_t eeprom;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    write_mount_block_0_chunk_0_latest();
    write_read_header_blank(blocks[0].address,
                            0,
                            0xffff);

    BTASSERT(eeprom_soft_mount(&eeprom) == -1);

    return (0);
}

static int test_mount_is_valid_chunk_calculate_crc_fail(void)
{
    struct eeprom_soft_driver_t eeprom;
    uint8_t buf[8];

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    write_mount_block_0_chunk_0_latest();
    write_read_header_blank(blocks[0].address,
                            0,
                            0xa5c3);

    /* Flash read of the CRC fails. */
    mock_write_flash_read(&buf[0],
                          blocks[0].address + 8,
                          sizeof(buf),
                          -1);

    BTASSERT(eeprom_soft_mount(&eeprom) == -1);

    return (0);
}

static int test_mount_is_valid_chunk_crc_mismatch(void)
{
    struct eeprom_soft_driver_t eeprom;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    write_mount_block_0_chunk_0_latest();
    write_read_header(blocks[0].address,
                      0x12345678, /* Does not match a blank chunk
                                     CRC. */
                      0,
                      0xa5c3);
    write_calculate_chunk_crc_blank(blocks[0].address,
                                         512);

    BTASSERT(eeprom_soft_mount(&eeprom) == -1);

    return (0);
}

static int test_write(void)
{
    struct eeprom_soft_driver_t eeprom;
    uint8_t data;
    uint8_t buf[8];

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    write_mount_block_0_chunk_0_latest();
    write_read_header_blank(blocks[0].address,
                            0,
                            0xa5c3);
    write_calculate_chunk_crc_blank(blocks[0].address,
                                    512);

    BTASSERT(eeprom_soft_mount(&eeprom) == 0);

    write_get_blank_chunk_block_0_chunk_1_blank();

    memset(&buf[0], -1, sizeof(buf));

    mock_write_flash_read_seq(&buf[0],
                              blocks[0].address + 8,
                              sizeof(buf),
                              sizeof(buf),
                              (512 - 8) / 8);

    data = 1;
    buf[0] = data;

    mock_write_flash_write(blocks[0].address + 512 + 8,
                           &buf[0],
                           sizeof(buf),
                           sizeof(buf));

    memset(&buf[0], -1, sizeof(buf));

    mock_write_flash_write_seq(blocks[0].address + 512 + 16,
                               &buf[0],
                               sizeof(buf),
                               sizeof(buf),
                               (512 - 16) / sizeof(buf));

    write_write_header_blank(blocks[0].address + 512,
                             1,
                             0xa5c3);

    BTASSERTI(eeprom_soft_write(&eeprom,
                                0,
                                &data,
                                sizeof(data)), ==, 1);

    return (0);
}

static int test_vwrite(void)
{
    struct eeprom_soft_driver_t eeprom;
    uint8_t buf[504];
    char part_1[] = "012";
    char part_2[] = "3456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLM";
    char part_3[] = "NOPQRSTUVWXYZ";
    struct iov_uintptr_t dst[3];
    struct iov_t src[3];
    ssize_t size;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    /* Mount. */
    write_mount_block_0_chunk_0_latest();
    write_read_header_blank(blocks[0].address,
                            0,
                            0xa5c3);
    write_calculate_chunk_crc_blank(blocks[0].address,
                                    512);

    BTASSERT(eeprom_soft_mount(&eeprom) == 0);

    /* Write. */
    write_get_blank_chunk_block_0_chunk_1_blank();
    memset(&buf[0], -1, sizeof(buf));
    mock_write_flash_read_seq2(&buf[0],
                               blocks[0].address + 8,
                               sizeof(buf),
                               8,
                               sizeof(buf) / 8);
    memcpy(&buf[33],
           &part_1[0],
           strlen(part_1));
    memcpy(&buf[33 + strlen(part_1)],
           &part_2[0],
           strlen(part_2));
    memcpy(&buf[33 + strlen(part_1) + strlen(part_2)],
           &part_3[0],
           strlen(part_3));
    mock_write_flash_write_seq2(blocks[0].address + 512 + 8,
                                &buf[0],
                                sizeof(buf),
                                8,
                                sizeof(buf) / 8);
    write_write_header_blank(blocks[0].address + 512,
                             1,
                             0xa5c3);

    dst[0].address = 33;
    dst[0].size = strlen(part_1);
    dst[1].address = (33 + strlen(part_1));
    dst[1].size = strlen(part_2);
    dst[2].address = (33 + strlen(part_1) + strlen(part_2));
    dst[2].size = strlen(part_3);

    src[0].buf_p = part_1;
    src[1].buf_p = part_2;
    src[2].buf_p = part_3;

    size = iov_uintptr_size(&dst[0], membersof(dst));

    BTASSERT(eeprom_soft_vwrite(&eeprom,
                                &dst[0],
                                &src[0],
                                membersof(dst)) == size);

    return (0);
}

static int test_read_write_after_failed_mount(void)
{
    struct eeprom_soft_driver_t eeprom;
    uint8_t data;

    BTASSERT(eeprom_soft_init(&eeprom,
                              &flash,
                              &blocks[0],
                              membersof(blocks),
                              512) == 0);

    /* Mount fails (with chunk CRC mismatch). */
    write_mount_block_0_chunk_0_latest();
    write_read_header(blocks[0].address,
                      0x12345678, /* Does not match a blank chunk
                                     CRC. */
                      0,
                      0xa5c3);
    write_calculate_chunk_crc_blank(blocks[0].address,
                                    512);

    BTASSERT(eeprom_soft_mount(&eeprom) == -1);

    /* Write should fail. */
    BTASSERT(eeprom_soft_write(&eeprom,
                               0,
                               &data,
                               sizeof(data)) == -ENOTMOUNTED);

    /* Read should fail. */
    BTASSERT(eeprom_soft_read(&eeprom,
                              &data,
                              0,
                              sizeof(data)) == -ENOTMOUNTED);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_module_init, "test_module_init" },
        { test_format, "test_format" },
        { test_format_erase_fail, "test_format_erase_fail" },
        { test_mount, "test_mount" },
        {
            test_mount_flash_read_fail_all_chunks,
            "test_mount_flash_read_fail_all_chunks"
        },
        {
            test_mount_revision_1_later_than_0,
            "test_mount_revision_1_later_than_0"
        },
        {
            test_mount_revision_1_later_than_33000,
            "test_mount_revision_1_later_than_33000"
        },
        {
            test_mount_is_valid_chunk_flash_read_fail,
            "test_mount_is_valid_chunk_flash_read_fail"
        },
        {
            test_mount_is_valid_chunk_not_valid,
            "test_mount_is_valid_chunk_not_valid"
        },
        {
            test_mount_is_valid_chunk_calculate_crc_fail,
            "test_mount_is_valid_chunk_calculate_crc_fail"
        },
        {
            test_mount_is_valid_chunk_crc_mismatch,
            "test_mount_is_valid_chunk_crc_mismatch"
        },
        { test_write, "test_write" },
        { test_vwrite, "test_vwrite" },
        {
            test_read_write_after_failed_mount,
            "test_read_write_after_failed_mount"
        },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
