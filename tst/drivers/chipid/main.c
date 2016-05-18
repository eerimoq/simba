/**
 * @file main.c
 * @version 0.5.0
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
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
