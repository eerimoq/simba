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

#if defined(BOARD_ARDUINO_DUE)
#    define SPI_DEVICE &pin_d8_dev
#else
#    define SPI_DEVICE &pin_d6_dev
#endif

static struct spi_driver_t spi;
static struct sd_driver_t sd;

static uint8_t buf[SD_BLOCK_SIZE];

static int test_init(struct harness_t *harness_p)
{
    int res;

#if defined(ARCH_LINUX)
    BTASSERT(system("./create_sdcard_linux.sh") == 0);
#endif

    BTASSERT(spi_init(&spi,
                      &spi_device[0],
                      SPI_DEVICE,
                      SPI_MODE_MASTER,
                      SPI_SPEED_250KBPS,
                      0,
                      0) == 0);

    BTASSERT(sd_init(&sd, &spi) == 0);
    BTASSERT((res = sd_start(&sd)) == 0, ", res = %d\r\n", res);

    return (0);
}

static int test_read_cid(struct harness_t *harness_p)
{
    struct sd_cid_t cid;
    static const char *mdt_month[16] = {
        "0",
        "Jan", "Feb", "Mar", "Apr",
        "May", "June", "July", "Aug",
        "Sep", "Oct", "Nov", "Dec",
        "13", "14", "15"
    };

    /* Read CID and print it to stdout. */
    BTASSERT(sd_read_cid(&sd, &cid) == sizeof(cid));

    std_printf(FSTR("cid {\r\n"
                    "    manufacturer id = 0x%02x\r\n"
                    "    application id = '%c%c'\r\n"
                    "    product name = '%c%c%c%c%c'\r\n"
                    "    product revision = %02x\r\n"
                    "    product serial number = %lu\r\n"
                    "    manufacturing date = %s %u\r\n"
                    "    crc checksum = 0x%02x\r\n"
                    "}\r\n"),
               (unsigned int)cid.mid,
               cid.oid[0], cid.oid[1],
               cid.pnm[0], cid.pnm[1], cid.pnm[2], cid.pnm[3], cid.pnm[4],
               cid.prv,
               cid.psn,
               mdt_month[ntohs(cid.mdt) & 0xf],
               2000 + ((ntohs(cid.mdt) >> 4) & 0xff),
               cid.crc);

    return (0);
}

static int test_read_csd(struct harness_t *harness_p)
{
    union sd_csd_t csd;

    /* Read CSD and print it to stdout. */
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
                        "    crc = 0x%02x\r\n"
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
    int i, block, res;

    /* Write to and read from the first five blocks. */
    for (block = 0; block < 5; block++) {
        /* Write reference data to given block and verify that it was
           written. */
        for (i = 0; i < membersof(buf); i++) {
            buf[i] = ((block + i) & 0xff);
        }

        BTASSERT((res = sd_write_block(&sd, block, buf)) == SD_BLOCK_SIZE,
                 ", res = %d\r\n", res);
        memset(buf, 0, sizeof(buf));
        BTASSERT((res = sd_read_block(&sd, buf, block)) == SD_BLOCK_SIZE,
                 ", res = %d\r\n", res);

        for (i = 0; i < membersof(buf); i++) {
            BTASSERT(buf[i] == ((block + i) & 0xff));
        }

        /* Write zeros to given block and verify that it was
           written. */
        for (i = 0; i < membersof(buf); i++) {
            buf[i] = 0;
        }

        memset(buf, 0, sizeof(buf));
        BTASSERT((res = sd_write_block(&sd, block, buf)) == SD_BLOCK_SIZE,
                 ", res = %d\r\n", res);
        memset(buf, -1, sizeof(buf));
        BTASSERT((res = sd_read_block(&sd, buf, block)) == SD_BLOCK_SIZE,
                 ", res = %d\r\n", res);

        for (i = 0; i < membersof(buf); i++) {
            BTASSERT(buf[i] == 0);
        }
    }

    return (0);
}

static int test_write_performance(struct harness_t *harness_p)
{
    int i, block, res;
    struct time_t start, stop, diff;
    float seconds;
    unsigned long bytes_per_seconds;
    int number_of_blocks = 32;

    /* Write reference data to given block and verify that it was
       written. */
    for (i = 0; i < membersof(buf); i++) {
        buf[i] = (i & 0xff);
    }

    time_get(&start);

    /* Write to and read from the first five blocks. */
    for (block = 0; block < number_of_blocks; block++) {
        BTASSERT((res = sd_write_block(&sd, block, buf)) == SD_BLOCK_SIZE,
                 ", res = %d\r\n", res);
    }

    time_get(&stop);
    time_subtract(&diff, &stop, &start);
    seconds = (diff.seconds + diff.nanoseconds / 1000000000.0f);
    bytes_per_seconds = ((SD_BLOCK_SIZE * number_of_blocks) / seconds);

    std_printf(FSTR("Wrote 32 blocks of %d bytes in %f s (%lu bytes/s).\r\n"),
               SD_BLOCK_SIZE,
               seconds,
               bytes_per_seconds);

    return (0);
}

static int test_read_performance(struct harness_t *harness_p)
{
    int i, block, res;
    struct time_t start, stop, diff;
    float seconds;
    unsigned long bytes_per_seconds;
    int number_of_blocks = 32;

    /* Write reference data to given block and verify that it was
       written. */
    for (i = 0; i < membersof(buf); i++) {
        buf[i] = (i & 0xff);
    }

    time_get(&start);

    /* Write to and read from the first five blocks. */
    for (block = 0; block < number_of_blocks; block++) {
        BTASSERT((res = sd_read_block(&sd, buf, block)) == SD_BLOCK_SIZE,
                 ", res = %d\r\n", res);
    }

    time_get(&stop);
    time_subtract(&diff, &stop, &start);
    seconds = (diff.seconds + diff.nanoseconds / 1000000000.0f);
    bytes_per_seconds = ((SD_BLOCK_SIZE * number_of_blocks) / seconds);

    std_printf(FSTR("Read 32 blocks of %d bytes in %f s (%lu bytes/s).\r\n"),
               SD_BLOCK_SIZE,
               seconds,
               bytes_per_seconds);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_read_cid, "test_read_cid" },
        { test_read_csd, "test_read_csd" },
        { test_read_write, "test_read_write" },
        { test_write_performance, "test_write_performance" },
        { test_read_performance, "test_read_performance" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    BTASSERT(sd_stop(&sd) == 0);

    return (0);
}
