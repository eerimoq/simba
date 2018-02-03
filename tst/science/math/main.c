/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

struct value_t {
    int integer;
    int decimals;
};

static uint32_t value2uint32(struct value_t *value_p,
                             int precision)
{
    return ((value_p->integer << precision) | (value_p->decimals));
}

static int32_t value2int32(struct value_t *value_p,
                           int precision)
{
    return ((value_p->integer << precision) | (value_p->decimals));
}

static int test_radians_to_degrees(void)
{
    BTASSERT(math_radians_to_degrees(0) == 0);
    BTASSERT(math_radians_to_degrees(MATH_PI) == 180);

    return (0);
}

static int test_degrees_to_radians(void)
{
    BTASSERT(math_degrees_to_radians(0) == 0);
    BTASSERT(math_degrees_to_radians(180) == MATH_PI);

    return (0);
}

static int test_log2_fixed_point(void)
{
    struct {
        struct value_t x;
        struct value_t y;
        int precision;
    } datas[] = {
        /* Precision 1. */
        { {          0,     0 }, { 1073741824,       0 },  1 },
        { {          1,     0 }, {          0,       0 },  1 },
        { {          2,     0 }, {          1,       0 },  1 },
        { {          4,     0 }, {          2,       0 },  1 },
        { {          8,     0 }, {          3,       0 },  1 },
        { {         16,     0 }, {          4,       0 },  1 },
        { {         21,     0 }, {          4,       0 },  1 },
        { {         32,     0 }, {          5,       0 },  1 },
        { { 2147483647,     0 }, {         30,       1 },  1 },
        { { 2147483647,     1 }, {         30,       1 },  1 },

        /* Precision 2. */
        { {          0,     0 }, {  536870912,       0 },  2 },
        { {          1,     0 }, {          0,       0 },  2 },
        { {          2,     0 }, {          1,       0 },  2 },
        { {          4,     0 }, {          2,       0 },  2 },
        { {          8,     0 }, {          3,       0 },  2 },
        { {         16,     0 }, {          4,       0 },  2 },
        { {         21,     0 }, {          4,       1 },  2 },
        { {         32,     0 }, {          5,       0 },  2 },

        /* Precision 16. */
        { {          0,     0 }, {     32768,         0 }, 16 },
        { {          0,     5 }, {       -14,     21097 }, 16 },
        { {          0,   600 }, {        -7,     14995 }, 16 },
        { {          1,     0 }, {         0,         0 }, 16 },
        { {          2,     0 }, {         1,         0 }, 16 },
        { {          3,     0 }, {         1,     38336 }, 16 },
        { {          4,     0 }, {         2,         0 }, 16 },
        { {          8,     0 }, {         3,         0 }, 16 },
        { {          8,  5000 }, {         3,       896 }, 16 },
        { {         16,     0 }, {         4,         0 }, 16 },
        { {         21,     0 }, {         4,     25710 }, 16 },
        { {         32,     0 }, {         5,         0 }, 16 },
        { {      65535,     0 }, {        15,     65534 }, 16 },
        { {      65535, 65535 }, {        15,     65535 }, 16 }
    };
    int i;
    uint32_t x;
    int32_t y;
    int precision;

    for (i = 0; i < membersof(datas); i++) {
        precision = datas[i].precision;
        x = value2uint32(&datas[i].x, precision);
        y = math_log2_fixed_point(x, precision);
        BTASSERTI(y, ==, value2int32(&datas[i].y, precision));
    }

    return (0);
}

static int test_ln_fixed_point(void)
{
    struct {
        struct value_t x;
        struct value_t y;
        int precision;
    } datas[] = {
        /* Precision 16. */
        { {          0,     0 }, {     42822,     62468 }, 16 },
        { {          1,     0 }, {         0,         0 }, 16 },
        { {          2,     0 }, {         0,     45426 }, 16 },
        { {          3,     0 }, {         1,      6462 }, 16 },
        { {          4,     0 }, {         1,     25316 }, 16 },
        { {          8,     0 }, {         2,      5206 }, 16 },
        { {          8,  5000 }, {         2,      5827 }, 16 },
        { {         16,     0 }, {         2,     50632 }, 16 },
        { {         21,     0 }, {         3,      2917 }, 16 },
        { {         32,     0 }, {         3,     30522 }, 16 },
        { {      65535,     0 }, {        11,      5920 }, 16 },
        { {      65535, 65535 }, {        11,      5920 }, 16 }
    };
    int i;
    uint32_t x;
    int32_t y;
    int precision;

    for (i = 0; i < membersof(datas); i++) {
        precision = datas[i].precision;
        x = value2uint32(&datas[i].x, precision);
        y = math_ln_fixed_point(x, precision);
        BTASSERTI(y, ==, value2int32(&datas[i].y, precision));
    }

    return (0);
}

static int test_log10_fixed_point(void)
{
    struct {
        struct value_t x;
        struct value_t y;
        int precision;
    } datas[] = {
        /* Precision 16. */
        { {          0,     0 }, {     55671,     55647 }, 16 },
        { {          1,     0 }, {         0,         0 }, 16 },
        { {          2,     0 }, {         0,     19728 }, 16 },
        { {          3,     0 }, {         0,     31268 }, 16 },
        { {          4,     0 }, {         0,     39456 }, 16 },
        { {          8,     0 }, {         0,     59184 }, 16 },
        { {          8,  5000 }, {         0,     59454 }, 16 },
        { {         16,     0 }, {         1,     13377 }, 16 },
        { {         21,     0 }, {         1,     21116 }, 16 },
        { {         32,     0 }, {         1,     33105 }, 16 },
        { {      65535,     0 }, {         4,     53508 }, 16 },
        { {      65535, 65535 }, {         4,     53508 }, 16 }
    };
    int i;
    uint32_t x;
    int32_t y;
    int precision;

    for (i = 0; i < membersof(datas); i++) {
        precision = datas[i].precision;
        x = value2uint32(&datas[i].x, precision);
        y = math_log10_fixed_point(x, precision);
        BTASSERTI(y, ==, value2int32(&datas[i].y, precision));
    }

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_radians_to_degrees, "test_radians_to_degrees" },
        { test_degrees_to_radians, "test_degrees_to_radians" },
        { test_log2_fixed_point, "test_log2_fixed_point" },
        { test_ln_fixed_point, "test_ln_fixed_point" },
        { test_log10_fixed_point, "test_log10_fixed_point" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
