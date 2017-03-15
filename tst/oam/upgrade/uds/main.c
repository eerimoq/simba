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

/* Memory ranges. */
#define APPLICATION_ADDRESS                      0x00000000
#define APPLICATION_SIZE                         0x20000000

static uint8_t inbuf[128];
static uint8_t outbuf[128];

static int test_bad_size(struct harness_t *self_p)
{
    struct upgrade_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        0, 0, 0, 0
    };

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    BTASSERT(upgrade_uds_handle_service(&uds) == -1);

    return (0);
}

static int test_unknown_service_id(struct harness_t *self_p)
{
    struct upgrade_uds_t uds;
    struct queue_t qin;
    struct queue_t qout;
    uint8_t input[] = {
        0, 0, 0, 1, 0xff
    };
    int32_t length;
    uint8_t response[3];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Service not supported. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0xff);
    BTASSERT(response[2] == 0x11);

    return (0);
}

static int test_read_data_by_identifier(struct harness_t *self_p)
{
    struct upgrade_uds_t uds;
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
    uint8_t response[3];
    char version[sizeof(STRINGIFY(VERSION))];
    uint16_t did;
    char time[16];
    
    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Bad did. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x22);
    BTASSERT(response[2] == 0x11);

    /* Bad did length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x22);
    BTASSERT(response[2] == 0x13);

    /* Bootloader version. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3 + sizeof(version));
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x62);
    BTASSERT(queue_read(&qout, &did, sizeof(did)) == sizeof(did));
    BTASSERT(ntohs(did) == 0xf000);
    BTASSERT(queue_read(&qout, version, sizeof(version)) == sizeof(version));
    BTASSERT(strcmp(STRINGIFY(VERSION), version) == 0);

    /* System time. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
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
    struct upgrade_uds_t uds;
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
    uint8_t response[3];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Bad length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x23);
    BTASSERT(response[2] == 0x13);

    /* Ok. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
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
    struct upgrade_uds_t uds;
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
    uint8_t response[3];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Bad length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x31);
    BTASSERT(response[2] == 0x13);

    /* Bad routine id. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x31);
    BTASSERT(response[2] == 0x11);

    /* Erase routine. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, 1) == 1);
    BTASSERT(code == 0x71);

    return (0);
}

static int test_diagnostic_session_control(struct harness_t *self_p)
{
    struct upgrade_uds_t uds;
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
    uint8_t response[3];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Bad length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x10);
    BTASSERT(response[2] == 0x13);

    /* Bad session. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x10);
    BTASSERT(response[2] == 0x31);

    /* Routine CALL: ok */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 1);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x50);

    return (0);
}

static int test_request_download(struct harness_t *self_p)
{
    struct upgrade_uds_t uds;
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
        /* /\* Bad address. *\/ */
        /* 0, 0, 0, 12, 0x34, */
        /* 0x00, */
        /* 0x04, 0x04, */
        /* 0xf0, 0x00, 0x00, 0x00, */
        /* 0x00, 0x00, 0x00, 0x01, */
        /* /\* Bad size. *\/ */
        /* 0, 0, 0, 12, 0x34, */
        /* 0x00, */
        /* 0x04, 0x04, */
        /* 0x00, 0x00, 0x00, 0x00, */
        /* 0xf0, 0x00, 0x00, 0x00, */
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
    uint8_t response[3];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Bad length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x34);
    BTASSERT(response[2] == 0x13);

    /* Bad field sizes. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x34);
    BTASSERT(response[2] == 0x31);

    /* Bad address and size length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x34);
    BTASSERT(response[2] == 0x13);

    /* /\* Bad address. *\/ */
    /* BTASSERT(upgrade_bootloader_uds_handle_service(&uds) == -1); */
    /* BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length)); */
    /* BTASSERT(ntohl(length) == 3); */
    /* BTASSERT(queue_read(&qout, response, 3) == 3); */
    /* BTASSERT(response[0] == 0x7f); */
    /* BTASSERT(response[1] == 0x34); */
    /* BTASSERT(response[2] == 0x31); */

    /* /\* Bad size. *\/ */
    /* BTASSERT(upgrade_bootloader_uds_handle_service(&uds) == -1); */
    /* BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length)); */
    /* BTASSERT(ntohl(length) == 3); */
    /* BTASSERT(queue_read(&qout, response, 3) == 3); */
    /* BTASSERT(response[0] == 0x7f); */
    /* BTASSERT(response[1] == 0x34); */
    /* BTASSERT(response[2] == 0x31); */

    /* Flash ok. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
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
    struct upgrade_uds_t uds;
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
        /* /\* Transfer too much data. *\/ */
        /* 0, 0, 0, 4, 0x36, 0x01, 0x5a, 0x5a, */
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
    uint8_t response[3];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Bad state. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x36);
    BTASSERT(response[2] == 0x22);

    /* Flash ok. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 6);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x74);
    BTASSERT(queue_read(&qout, &size, sizeof(size)) == sizeof(size));
    BTASSERT(size == 0x40);
    BTASSERT(queue_read(&qout, &max_size, sizeof(max_size)) == sizeof(max_size));
    BTASSERT(ntohl(max_size) == 4096);

    /* Bad length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x36);
    BTASSERT(response[2] == 0x13);

    /* /\* Transfer too much data. *\/ */
    /* BTASSERT(upgrade_uds_handle_service(&uds) == -1); */
    /* BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length)); */
    /* BTASSERT(ntohl(length) == 3); */
    /* BTASSERT(queue_read(&qout, response, 3) == 3); */
    /* BTASSERT(response[0] == 0x7f); */
    /* BTASSERT(response[1] == 0x36); */
    /* BTASSERT(response[2] == 0x31); */

    /* Transfer wrong sequence counter. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x36);
    BTASSERT(response[2] == 0x73);

    /* Transfer ok. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
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
    struct upgrade_uds_t uds;
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
    uint8_t response[3];

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));
    queue_write(&qin, input, sizeof(input));
    BTASSERT(upgrade_uds_init(&uds, &qin, &qout) == 0);

    /* Bad length. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x37);
    BTASSERT(response[2] == 0x13);

    /* Bad state. */
    BTASSERT(upgrade_uds_handle_service(&uds) == -1);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 3);
    BTASSERT(queue_read(&qout, response, 3) == 3);
    BTASSERT(response[0] == 0x7f);
    BTASSERT(response[1] == 0x37);
    BTASSERT(response[2] == 0x22);

    /* Flash ok. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 6);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x74);
    BTASSERT(queue_read(&qout, &size, sizeof(size)) == sizeof(size));
    BTASSERT(size == 0x40);
    BTASSERT(queue_read(&qout, &max_size, sizeof(max_size)) == sizeof(max_size));
    BTASSERT(ntohl(max_size) == 4096);

    /* Transfer ok. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
    BTASSERT(queue_read(&qout, &length, sizeof(length)) == sizeof(length));
    BTASSERT(ntohl(length) == 2);
    BTASSERT(queue_read(&qout, &code, sizeof(code)) == sizeof(code));
    BTASSERT(code == 0x76);
    BTASSERT(queue_read(&qout,
                        &block_sequence_counter,
                        sizeof(block_sequence_counter)) == sizeof(block_sequence_counter));
    BTASSERT(block_sequence_counter == 0x01);

    /* Transfer exit ok. */
    BTASSERT(upgrade_uds_handle_service(&uds) == 0);
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

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
