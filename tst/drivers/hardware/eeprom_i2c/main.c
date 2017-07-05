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

#define EEPROM_I2C_ADDRESS 0x57
#define EEPROM_SIZE        32768

static struct eeprom_i2c_driver_t eeprom_i2c;
static struct i2c_driver_t i2c;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(eeprom_i2c_module_init() == 0);
    BTASSERT(eeprom_i2c_module_init() == 0);

    BTASSERT(i2c_init(&i2c,
                      &i2c_device[0],
                      I2C_BAUDRATE_100KBPS,
                      -1) == 0);
    BTASSERT(i2c_start(&i2c) == 0);
    BTASSERT(eeprom_i2c_init(&eeprom_i2c,
                             &i2c,
                             EEPROM_I2C_ADDRESS,
                             EEPROM_SIZE) == 0);

    return (0);
}

static int test_scan(struct harness_t *harness_p)
{
    int number_of_slaves_found;
    int address;

    number_of_slaves_found = 0;

    for (address = 0; address < 128; address++) {
        if (i2c_scan(&i2c, address) == 1) {
            std_printf(OSTR("Found slave with address 0x%02x.\r\n"),
                       address);
            number_of_slaves_found++;
        }
    }

    std_printf(OSTR("Number of slaves found: %d\r\n"),
               number_of_slaves_found);

    return (0);
}

static int test_read_write_sizes(struct harness_t *harness_p)
{
    int i;
    uint8_t write_buf[153];
    uint8_t read_buf[153];

    /* Write zeros to the EEPROM. */
    memset(&write_buf[0], 0, sizeof(write_buf));
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,
                              0,
                              &write_buf[0],
                              sizeof(write_buf)) == sizeof(write_buf));

    /* Read and verify the written data. */
    memset(&read_buf[0], -1, sizeof(read_buf));
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,
                             &read_buf[0],
                             0,
                             sizeof(read_buf)) ==  sizeof(read_buf));
    BTASSERTM(&read_buf[0], &write_buf[0], sizeof(write_buf));

    /* Write various sizes at various alignments. */
    for (i = 0; i < membersof(write_buf); i++) {
        write_buf[i] = (membersof(write_buf) - i);
    }

    BTASSERT(eeprom_i2c_write(&eeprom_i2c,   0,  &write_buf[0],   1) ==  1);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,   1,  &write_buf[1],   2) ==  2);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,   3,  &write_buf[3],   3) ==  3);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,   6,  &write_buf[6],   4) ==  4);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  10, &write_buf[10],   5) ==  5);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  15, &write_buf[15],   6) ==  6);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  21, &write_buf[21],   7) ==  7);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  28, &write_buf[28],   8) ==  8);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  36, &write_buf[36],   9) ==  9);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  45, &write_buf[45],  10) == 10);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  55, &write_buf[55],  11) == 11);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  66, &write_buf[66],  12) == 12);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  78, &write_buf[78],  13) == 13);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,  91, &write_buf[91],  14) == 14);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c, 105, &write_buf[105], 15) == 15);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c, 120, &write_buf[120], 16) == 16);
    BTASSERT(eeprom_i2c_write(&eeprom_i2c, 136, &write_buf[136], 17) == 17);

    /* Read and verify the written data. */
    memset(&read_buf[0], -1, sizeof(read_buf));

    BTASSERT(eeprom_i2c_read(&eeprom_i2c,  &read_buf[0],    0,  1) ==  1);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,  &read_buf[1],    1,  2) ==  2);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,  &read_buf[3],    3,  3) ==  3);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,  &read_buf[6],    6,  4) ==  4);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[10],   10,  5) ==  5);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[15],   15,  6) ==  6);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[21],   21,  7) ==  7);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[28],   28,  8) ==  8);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[36],   36,  9) ==  9);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[45],   45, 10) == 10);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[55],   55, 11) == 11);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[66],   66, 12) == 12);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[78],   78, 13) == 13);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[91],   91, 14) == 14);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[105], 105, 15) == 15);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[120], 120, 16) == 16);
    BTASSERT(eeprom_i2c_read(&eeprom_i2c, &read_buf[136], 136, 17) == 17);

    /* Verify that read and written data match. */
    BTASSERTM(&read_buf[0], &write_buf[0], sizeof(write_buf));

    return (0);
}

static int test_read_write_low_high(struct harness_t *harness_p)
{
    uint8_t byte;

    /* Lowest address. */
    byte = 1;
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,
                              0,
                              &byte,
                              sizeof(byte)) == sizeof(byte));

    byte = 0;
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,
                             &byte,
                             0,
                             sizeof(byte)) == sizeof(byte));
    BTASSERT(byte == 1);

    /* Highest address. */
    byte = 2;
    BTASSERT(eeprom_i2c_write(&eeprom_i2c,
                              EEPROM_SIZE - 1,
                              &byte,
                              sizeof(byte)) == sizeof(byte));

    byte = 0;
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,
                             &byte,
                             EEPROM_SIZE - 1,
                             sizeof(byte)) == sizeof(byte));

    return (0);
}

static int test_read_write_bad_address(struct harness_t *harness_p)
{
    uint8_t buf[2];

    memset(&buf[0], 0, sizeof(buf));

    /* Start address outside the EEPROM. */
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,
                             &buf[0],
                             EEPROM_SIZE,
                             sizeof(buf)) == -EINVAL);

    BTASSERT(eeprom_i2c_write(&eeprom_i2c,
                              EEPROM_SIZE,
                              &buf[0],
                              sizeof(buf)) == -EINVAL);

    /* End address outside the EEPROM. */
    BTASSERT(eeprom_i2c_read(&eeprom_i2c,
                             &buf[0],
                             EEPROM_SIZE - 1,
                             sizeof(buf)) == -EINVAL);

    BTASSERT(eeprom_i2c_write(&eeprom_i2c,
                              EEPROM_SIZE - 1,
                              &buf[0],
                              sizeof(buf)) == -EINVAL);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_scan, "test_scan" },
        { test_read_write_sizes, "test_read_write_sizes" },
        { test_read_write_low_high, "test_read_write_low_high" },
        { test_read_write_bad_address, "test_read_write_bad_address" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
