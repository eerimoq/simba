/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#define I2C_SLAVE_ADDRESS          0x01

struct i2c_driver_t i2c;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(i2c_init(&i2c,
                      &i2c_0_dev,
                      -1,
                      I2C_SLAVE_ADDRESS) == 0);
    BTASSERT(i2c_slave_start(&i2c) == 0);

    return (0);
}

static int test_slave_write(struct harness_t *harness_p)
{
    uint8_t buf[3];

    /* Write 3 bytes to the master. */
    buf[0] = 5;
    buf[1] = 3;
    buf[2] = 1;

    BTASSERT(i2c_slave_write(&i2c, buf, 3) == 3);

    return (0);
}

static int test_slave_read(struct harness_t *harness_p)
{
    uint8_t buf[5];

    /* Read 5 bytes from the master. */
    memset(buf, 0, sizeof(buf));

    BTASSERT(i2c_slave_read(&i2c, buf, 5) == 5);

    BTASSERT(buf[0] == 1);
    BTASSERT(buf[1] == 3);
    BTASSERT(buf[2] == 5);
    BTASSERT(buf[3] == 7);
    BTASSERT(buf[4] == 9);

    return (0);
}

static int test_echo(struct harness_t *harness_p)
{
    uint8_t value;

    /* Read ping from the master . */
    value = 0;

    BTASSERT(i2c_slave_read(&i2c, &value, 1) == 1);

    /* Write pong to the master (ping + 1). */
    value++;

    BTASSERT(i2c_slave_write(&i2c, &value, 1) == 1);

    return (0);
}

static int test_mem_read(struct harness_t *harness_p)
{
    uint8_t address;
    uint8_t value;

    BTASSERT(i2c_slave_read(&i2c, &address, 1) == 1);
    BTASSERT(address == 0x34)
    value = 0x54;
    BTASSERT(i2c_slave_write(&i2c, &value, 1) == 1);

    return (0);
}

static int test_stop(struct harness_t *harness_p)
{
    BTASSERT(i2c_slave_stop(&i2c) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_slave_write, "test_slave_write" },
        { test_slave_read, "test_slave_read" },
        { test_echo, "test_echo" },
        { test_mem_read, "test_mem_read" },
        { test_stop, "test_stop" },
        { NULL, NULL }
    };

    sys_start();
    i2c_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
