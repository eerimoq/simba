/**
 * @file main.c
 * @version 7.0.0
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

#include "bootloader.h"

/* Memory ranges. */
#define APPLICATION_ADDRESS                      0x00000000
#define APPLICATION_SIZE                         0x20000000

extern int bootloader_cmd_application_load_kermit(int argc,
                                                  const char *argv[],
                                                  void *out_p,
                                                  void *in_p,
                                                  void *arg_p);

static uint8_t inbuf[128];
static uint8_t outbuf[128];
static char buf[128];

static struct flash_driver_t flash;

static int test_send_file_kermit(struct harness_t *self_p)
{
    struct queue_t qin;
    struct queue_t qout;
    struct bootloader_console_t console;

    static char input[] =
        "kermit -ir\r\n"
        "\x01""9 S~/ @-#Y3~^>J)0___F\"U1@?\r"
        "\x01.!FVERSION.TXTL\r"
        "\x01*\"D0.3.0#JO\r"
        "\x01##ZB\r"
        "\x01#$B+\r";

    static char output[] =
        "\x01""0 Y~!  -#N1N\" !~(\r"
        "\x01#!Y?\r"
        "\x01#\"Y@\r"
        "\x01##YA\r"
        "\x01#$YB\r"
        "File transfer completed successfully.\r\n";

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));

    bootloader_console_module_init();
    bootloader_console_init(&qin,
                            &qout,
                            0,
                            128,
                            &flash);

    queue_write(&qin, input, sizeof(input) - 1);
    strcpy(buf, "/application/load_kermit");
    BTASSERT(fs_call(buf, &qin, &qout, &console) == 0);

    queue_read(&qout, buf, sizeof(output) - 1);
    BTASSERT(memcmp(output, buf, sizeof(output) - 1) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_send_file_kermit, "test_send_file_kermit" },
        { NULL, NULL }
    };

    sys_start();

#if defined(MCU_SAM_3X8E)
    /* Initialize the flash memory objects. */
    flash_module_init();
    flash_init(&flash, &flash_device[0]);
#endif

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
