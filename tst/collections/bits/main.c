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

static int test_insert(struct harness_t *harness)
{
    uint32_t dst;
    uint32_t src;
    int pos;
    int size;
 
    /* Insert a one at LSB. */
    dst = 0xffffffff;
    src = 0x0;
    pos = 0;
    size = 1;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0xfffffffe);
 
    /* Insert a one at MSB. */
    dst = 0xffffffff;
    src = 0x0;
    pos = 31;
    size = 1;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0x7fffffff);
 
    /* Insert in the middle. */
    dst = 0xffffffff;
    src = 0x34;
    pos = 17;
    size = 5;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0xffe9ffff);
 
    /* Insert the maximum 31 bits. */
    dst = 0xffffffff;
    src = 0x12345678;
    pos = 0;
    size = 31;
    BTASSERT(bits_insert_32(dst, pos, size, src) == 0x92345678);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_insert, "test_insert" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
