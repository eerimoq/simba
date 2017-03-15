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

static int test_read(struct harness_t *harness_p)
{
    struct chipid_t id;

    BTASSERT(chipid_read(&id) == 0);

    std_printf(FSTR("version = %d\r\n"
                    "embedded_processor = %d\r\n"
                    "non_volatile_program_memory_size[0] = %d\r\n"
                    "non_volatile_program_memory_size[1] = %d\r\n"
                    "internal_sram_size = %d\r\n"
                    "architecture = %d\r\n"
                    "non_volatile_program_memory_type = %d\r\n"),
               id.version,
               id.embedded_processor,
               id.non_volatile_program_memory_size[0],
               id.non_volatile_program_memory_size[1],
               id.internal_sram_size,
               id.architecture,
               id.non_volatile_program_memory_type);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_read, "test_read" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
