/**
 * @file main.c
 * @version 6.0.0
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

static uint8_t inbuf[128];
static uint8_t outbuf[128];

static struct flash_driver_t flash;

static int test_bad_size(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        0, 0, 0, 0
    };

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    BTASSERT(bootloader_uds_handle_service(&uds) == -1);

    return (0);
}

static int test_unknown_service_id(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        0, 0, 0, 1, 0xff
    };
    int32_t length;
    uint8_t code;

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x11);

    return (0);
}

static int test_read_data_by_identifier(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        /* Bad did. */
        0, 0, 0, 3, 0x22, 0x00, 0x00,
        /* Bad did length. */
        0, 0, 0, 2, 0x22, 0x00,
        /* Bootloader version. */
        0, 0, 0, 3, 0x22, 0xf0, 0x00,
        /* System time. */
        0, 0, 0, 3, 0x22, 0xf0, 0x01
    };
    int32_t length;
    uint8_t code;
    char version[sizeof(STRINGIFY(VERSION))];
    uint16_t did;
    char time[16];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    /* Bad did. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x11);

    /* Bad did length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Bootloader version. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3 + sizeof(version));
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x62);
    BTASSERT(queue_read(&qout, &did, sizeof(did)) == sizeof(did));
    BTASSERT(ntohs(did) == 0xf000);
    BTASSERT(queue_read(&qout, version, sizeof(version)) == sizeof(version));
    BTASSERT(strcmp(STRINGIFY(VERSION), version) == 0);

    /* System time. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    length = ntohl(length);
    fprintf(stderr, "%d\n", length);
    BTASSERT(length > 3);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x62);
    BTASSERT(queue_read(&qout, &did, sizeof(did)) == sizeof(did));
    BTASSERT(ntohs(did) == 0xf001);
    BTASSERT(queue_read(&qout, time, length - 3) == length - 3);
    std_printf(FSTR("System time: %s\n"), time);

    return (0);
}

static int test_read_memory_by_address(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        /* Bad length. */
        0, 0, 0, 3, 0x23, 0x00, 0x00,
        /* Ok. */
        0, 0, 0, 5, 0x23, 0x00, 0x00, 0x00, 0x00
    };
    int32_t length;
    uint32_t address;
    uint8_t value;
    uint8_t code;

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    /* Bad length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Ok. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 6);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x63);
    BTASSERT(queue_read(&qout, &address, sizeof(address)) == sizeof(address));
    BTASSERT(address == 0x00000000);
    BTASSERT(queue_read(&qout, &value, sizeof(value)) == sizeof(value));
    BTASSERT(value == 0x01);

    return (0);
}

static int test_routine_control(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        /* Bad length. */
        0, 0, 0, 1, 0x31,
        /* Bad routine id. */
        0, 0, 0, 4, 0x31, 0x01, 0x00, 0x00,
        /* Start erase routine. */
        0, 0, 0, 4, 0x31, 0x01, 0xff, 0x00,
    };
    int32_t length;
    uint8_t code;

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    /* Bad length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Bad routine id. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x11);

    /* Erase routine. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x71);

    return (0);
}

static int test_diagnostic_session_control(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        /* Bad length. */
        0, 0, 0, 1, 0x10,
        /* Bad session. */
        0, 0, 0, 2, 0x10, 0x02,
        /* Default session. */
        0, 0, 0, 2, 0x10, 0x01
    };
    int32_t length;
    uint8_t code;

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    /* Bad length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Bad session. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x31);

    /* Routine CALL: ok */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x50);

    return (0);
}

static int test_request_download(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        /* Bad length. */
        0, 0, 0, 2, 0x34,
        0x00,
        /* Bad field sizes. */
        0, 0, 0, 4, 0x34,
        0x00,
        0x04, 0x03,
        /* Bad address and size length. */
        0, 0, 0, 11, 0x34,
        0x00,
        0x04, 0x04,
        0xf0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00,
        /* Bad address. */
        0, 0, 0, 12, 0x34,
        0x00,
        0x04, 0x04,
        0xf0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
        /* Bad size. */
        0, 0, 0, 12, 0x34,
        0x00,
        0x04, 0x04,
        0x00, 0x00, 0x00, 0x00,
        0xf0, 0x00, 0x00, 0x00,
        /* Flash ok. */
        0, 0, 0, 12, 0x34,
        0x00,
        0x04, 0x04,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01
    };
    int32_t length;
    uint8_t code;
    uint8_t size;
    uint32_t max_size;

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    /* Bad length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Bad field sizes. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x31);

    /* Bad address and size length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Bad address. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x31);

    /* Bad size. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x31);

    /* Flash ok. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 6);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x74);
    BTASSERT(queue_read(&qout, &size, sizeof(size)) == sizeof(size));
    BTASSERT(size == 0x40);
    BTASSERT(queue_read(&qout, &max_size, sizeof(max_size)) == sizeof(max_size));
    BTASSERT(ntohl(max_size) == 4096);

    return (0);
}

static int test_transfer_data(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        /* Bad state. */
        0, 0, 0, 1, 0x36,
        /* Flash ok. */
        0, 0, 0, 12, 0x34,
        0x00,
        0x04, 0x04,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
        /* Bad length. */
        0, 0, 0, 1, 0x36,
        /* Transfer too much data. */
        0, 0, 0, 4, 0x36, 0x01, 0x5a, 0x5a,
        /* Transfer wrong sequence number. */
        0, 0, 0, 3, 0x36, 0x02, 0x5a,
        /* Transfer ok. */
        0, 0, 0, 3, 0x36, 0x01, 0x5a
    };
    int32_t length;
    uint8_t code;
    uint8_t size;
    uint8_t block_sequence_counter;
    uint32_t max_size;

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    /* Bad state. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x22);

    /* Flash ok. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 6);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x74);
    BTASSERT(queue_read(&qout, &size, sizeof(size)) == sizeof(size));
    BTASSERT(size == 0x40);
    BTASSERT(queue_read(&qout, &max_size, sizeof(max_size)) == sizeof(max_size));
    BTASSERT(ntohl(max_size) == 4096);

    /* Bad length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Transfer too much data. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x31);

    /* Transfer wrong sequence counter. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x73);

    /* Transfer ok. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 2);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x76);
    BTASSERT(queue_read(&qout,
                        &block_sequence_counter,
                        sizeof(block_sequence_counter)) == sizeof(block_sequence_counter));
    BTASSERT(block_sequence_counter == 0x01);

    return (0);
}

static int test_request_transfer_exit(struct harness_t *self_p)
{
    struct bootloader_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        /* Bad length. */
        0, 0, 0, 2, 0x37, 0x00,
        /* Bad state. */
        0, 0, 0, 1, 0x37,
        /* Flash ok. */
        0, 0, 0, 12, 0x34,
        0x00,
        0x04, 0x04,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
        /* Transfer ok. */
        0, 0, 0, 3, 0x36, 0x01, 0x5a,
        /* Transfer exit ok. */
        0, 0, 0, 1, 0x37
    };
    int32_t length;
    uint8_t code;
    uint8_t size;
    uint32_t max_size;
    uint8_t block_sequence_counter;

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    bootloader_uds_init(&uds,
                        &qin,
                        &qout,
                        APPLICATION_ADDRESS,
                        APPLICATION_SIZE,
                        &flash);

    /* Bad length. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x13);

    /* Bad state. */
    BTASSERT(bootloader_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x22);

    /* Flash ok. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 6);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x74);
    BTASSERT(queue_read(&qout, &size, sizeof(size)) == sizeof(size));
    BTASSERT(size == 0x40);
    BTASSERT(queue_read(&qout, &max_size, sizeof(max_size)) == sizeof(max_size));
    BTASSERT(ntohl(max_size) == 4096);

    /* Transfer ok. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 2);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x76);
    BTASSERT(queue_read(&qout,
                        &block_sequence_counter,
                        sizeof(block_sequence_counter)) == sizeof(block_sequence_counter));
    BTASSERT(block_sequence_counter == 0x01);

    /* Transfer exit ok. */
    BTASSERT(bootloader_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x77);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_bad_size, "test_bad_size" },
        { test_unknown_service_id, "test_unknown_service_id" },
        { test_read_data_by_identifier, "test_read_data_by_identifier" },
        { test_read_memory_by_address, "test_read_memory_by_address" },
        { test_routine_control, "test_routine_control" },
        { test_diagnostic_session_control, "test_diagnostic_session_control" },
        { test_request_download, "test_request_download" },
        { test_transfer_data, "test_transfer_data" },
        { test_request_transfer_exit, "test_request_transfer_exit" },
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
