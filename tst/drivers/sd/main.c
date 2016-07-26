/**
 * @file main.c
 * @version 3.0.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

static struct spi_driver_t spi;
static struct sd_driver_t sd;

static char reference[SD_BLOCK_SIZE];
static char zeros[SD_BLOCK_SIZE];
static char buf[SD_BLOCK_SIZE];

static int test_read_cid_csd(struct harness_t *harness_p)
{
    struct sd_cid_t cid;
    union sd_csd_t csd;

    /* Read CID and print it to stdout. */
    BTASSERT(sd_read_cid(&sd, &cid) == sizeof(cid));

    std_printf(FSTR("cid {\r\n"
                    "    manufacturer id = %u\r\n"
                    "    application id = '%c%c'\r\n"
                    "    product name = '%c%c%c%c%c'\r\n"
                    "    product revision = %u\r\n"
                    "    product serial number = %lu\r\n"
                    "    manufacturing date = %u\r\n"
                    "    crc checksum = %x\r\n"
                    "}\r\n"),
               (unsigned int)cid.mid,
               cid.oid[0], cid.oid[1],
               cid.pnm[0], cid.pnm[1], cid.pnm[2], cid.pnm[3], cid.pnm[4],
               cid.prv,
               cid.psn,
               cid.mdt,
               cid.crc);

    BTASSERT(sd_read_csd(&sd, &csd) == sizeof(csd));

    switch (csd.v1.csd_structure) {

    case SD_CSD_STRUCTURE_V1:
        std_printf(FSTR("csd {\r\n"
                        "    csd_structure = %u\r\n"
                        "    taac = %u\r\n"
                        "    nsac = %u\r\n"
                        "    tran_speed = %u\r\n"
                        "    ccc = %u\r\n"
                        "    read_bl_len = %u\r\n"
                        "    read_bl_partial = %u\r\n"
                        "    write_blk_misalign = %u\r\n"
                        "    read_blk_misalign = %u\r\n"
                        "    dsr_imp = %u\r\n"
                        "    c_size = %u\r\n"
                        "    vdd_r_curr_min = %u\r\n"
                        "    vdd_r_curr_max = %u\r\n"
                        "    vdd_w_curr_min = %u\r\n"
                        "    vdd_w_cur_max = %u\r\n"
                        "    c_size_mult = %u\r\n"
                        "    erase_blk_en = %u\r\n"
                        "    sector_size = %u\r\n"
                        "    wp_grp_size = %u\r\n"
                        "    wp_grp_enable = %u\r\n"
                        "    r2w_factor = %u\r\n"
                        "    write_bl_len = %u\r\n"
                        "    write_bl_partial = %u\r\n"
                        "    file_format_grp = %u\r\n"
                        "    copy = %u\r\n"
                        "    perm_write_protect = %u\r\n"
                        "    tmp_write_protect = %u\r\n"
                        "    file_format = %u\r\n"
                        "    crc = %u\r\n"
                        "}\r\n"),
                   csd.v1.csd_structure,
                   csd.v1.taac,
                   csd.v1.nsac,
                   csd.v1.tran_speed,
                   SD_CCC(&csd.v1),
                   csd.v1.read_bl_len,
                   csd.v1.read_bl_partial,
                   csd.v1.write_blk_misalign,
                   csd.v1.read_blk_misalign,
                   csd.v1.dsr_imp,
                   SD_C_SIZE(&csd.v1),
                   csd.v1.vdd_r_curr_min,
                   csd.v1.vdd_r_curr_max,
                   csd.v1.vdd_w_curr_min,
                   csd.v1.vdd_w_curr_max,
                   SD_C_SIZE_MULT(&csd.v1),
                   csd.v1.erase_blk_en,
                   SD_SECTOR_SIZE(&csd.v1),
                   csd.v1.wp_grp_size,
                   csd.v1.wp_grp_enable,
                   csd.v1.r2w_factor,
                   SD_WRITE_BL_LEN(&csd.v1),
                   csd.v1.write_bl_partial,
                   csd.v1.file_format_grp,
                   csd.v1.copy,
                   csd.v1.perm_write_protect,
                   csd.v1.tmp_write_protect,
                   csd.v1.file_format,
                   csd.v1.crc);
        break;

    case SD_CSD_STRUCTURE_V2:
        std_printf(FSTR("csd {\r\n"
                        "    csd_structure = %u\r\n"
                        "    taac = %u\r\n"
                        "    nsac = %u\r\n"
                        "    tran_speed = %u\r\n"
                        "    ccc = %u\r\n"
                        "    read_bl_len = %u\r\n"
                        "    dsr_imp = %u\r\n"
                        "    read_blk_misalign = %u\r\n"
                        "    write_blk_misalign = %u\r\n"
                        "    read_bl_partial = %u\r\n"
                        "    c_size = %u\r\n"
                        "    sector_size = %u\r\n"
                        "    erase_blk_en = %u\r\n"
                        "    wp_grp_size = %u\r\n"
                        "    write_bl_len = %u\r\n"
                        "    r2w_factor = %u\r\n"
                        "    wp_grp_enable = %u\r\n"
                        "    write_partial = %u\r\n"
                        "    file_format = %u\r\n"
                        "    tmp_write_protect = %u\r\n"
                        "    perm_write_protect = %u\r\n"
                        "    copy = %u\r\n"
                        "    file_format_grp = %u\r\n"
                        "    crc = %u\r\n"
                        "}\r\n"),
                   csd.v2.csd_structure,
                   csd.v2.taac,
                   csd.v2.nsac,
                   csd.v2.tran_speed,
                   SD_CCC(&csd.v2),
                   csd.v2.read_bl_len,
                   csd.v2.dsr_imp,
                   csd.v2.read_blk_misalign,
                   csd.v2.write_blk_misalign,
                   csd.v2.read_bl_partial,
                   SD_C_SIZE(&csd.v2),
                   SD_SECTOR_SIZE(&csd.v2),
                   csd.v2.erase_blk_en,
                   csd.v2.wp_grp_size,
                   SD_WRITE_BL_LEN(&csd.v2),
                   csd.v2.r2w_factor,
                   csd.v2.wp_grp_enable,
                   csd.v2.write_bl_partial,
                   csd.v2.file_format,
                   csd.v2.tmp_write_protect,
                   csd.v2.perm_write_protect,
                   csd.v2.copy,
                   csd.v2.file_format_grp,
                   csd.v2.crc);
        break;

    default:
        std_printf(FSTR("Unsuported csd structure %d.\r\n"),
                   csd.v1.csd_structure);
        break;
    }

    return (0);
}

static int test_read_write(struct harness_t *harness_p)
{
    int i, block;

    memset(zeros, 0, sizeof(zeros));

    /* Erase, write and read the first block. */
    for (block = 0; block < 5; block++) {
        std_printf(FSTR("block = %d\r\n"), block);

        for (i = 0; i < membersof(reference); i++) {
            reference[i] = (block + i);
        }

        BTASSERT(sd_write_block(&sd, 0, reference) == SD_BLOCK_SIZE);
        memset(buf, 0, sizeof(buf));
        BTASSERT(sd_read_block(&sd, buf, block) == SD_BLOCK_SIZE);
        BTASSERT(memcmp(buf, reference, SD_BLOCK_SIZE) == 0);

        /* Write zeros to the block. */
        BTASSERT(sd_write_block(&sd, 0, zeros) == SD_BLOCK_SIZE);
        memset(buf, -1, sizeof(buf));
        BTASSERT(sd_read_block(&sd, buf, block) == SD_BLOCK_SIZE);
        BTASSERT(memcmp(buf, zeros, SD_BLOCK_SIZE) == 0);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_cid_csd, "test_read_cid_csd" },
        { test_read_write, "test_read_write" },
        { NULL, NULL }
    };

    sys_start();

#if defined(ARCH_LINUX)
    BTASSERT(system("./create_sdcard_linux.sh") == 0);
#endif

    BTASSERT(spi_init(&spi,
                      &spi_device[0],
                      &pin_d53_dev,
                      SPI_MODE_MASTER,
                      SPI_SPEED_500KBPS,
                      0,
                      1) == 0);

    BTASSERT(sd_init(&sd, &spi) == 0);
    BTASSERT(sd_start(&sd) == 0);

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    BTASSERT(sd_stop(&sd) == 0);

    return (0);
}
