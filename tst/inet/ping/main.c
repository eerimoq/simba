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

#define BUFFER_SIZE 128

extern void socket_stub_input(void *buf_p, size_t size);
extern void socket_stub_output(void *buf_p, size_t size);

static char qoutbuf[BUFFER_SIZE];
static QUEUE_INIT_DECL(qout, qoutbuf, sizeof(qoutbuf));

static int test_host_by_ip_address(struct harness_t *harness_p)
{
    struct inet_ip_addr_t address;
    struct time_t round_trip_time, timeout;
    uint8_t request[8];
    uint8_t reply[28];

    timeout.seconds = 1;
    timeout.nanoseconds = 0;
    
    /* Prepare the socket stub with the reply packet. The first 20
       bytes in the reply are the IP header. */
    reply[20] = 0;
    reply[21] = 0;
    reply[22] = 0xff;
    reply[23] = 0xfe;
    reply[24] = 0;
    reply[25] = 0;
    reply[26] = 0;
    reply[27] = 1;
    socket_stub_input(reply, sizeof(reply));

    /* Perform the ping. */
    address.number = 0x1;
    BTASSERT(ping_host_by_ip_address(&address,
                                     &timeout,
                                     &round_trip_time) == 0);

    /* Check the request send by the ping module. */
    socket_stub_output(request, sizeof(request));
    BTASSERT(request[0] == 8);
    BTASSERT(request[1] == 0);
    BTASSERT(request[2] == 0xf7);
    BTASSERT(request[3] == 0xfe);
    BTASSERT(request[4] == 0);
    BTASSERT(request[5] == 0);
    BTASSERT(request[6] == 0);
    BTASSERT(request[7] == 1);

    std_printf(FSTR("round trip time: %lu s %lu ns\r\n"),
               round_trip_time.seconds,
               round_trip_time.nanoseconds);

    return (0);
}

static int test_bad_reply_crc(struct harness_t *harness_p)
{
    struct inet_ip_addr_t address;
    struct time_t round_trip_time, timeout;
    uint8_t request[8];
    uint8_t reply[28];

    timeout.seconds = 1;
    timeout.nanoseconds = 0;
    
    /* Prepare the socket stub with the reply packet. The first 20
       bytes in the reply are the IP header. */
    reply[20] = 0;
    reply[21] = 0;
    reply[22] = 0xff;
    reply[23] = 0xfe;
    reply[24] = 0;
    reply[25] = 1;
    reply[26] = 0;
    reply[27] = 1;
    socket_stub_input(reply, sizeof(reply));

    /* Perform the ping. */
    address.number = 0x1;
    BTASSERT(ping_host_by_ip_address(&address,
                                     &timeout,
                                     &round_trip_time) == -1);

    /* Check the request send by the ping module. */
    socket_stub_output(request, sizeof(request));
    BTASSERT(request[0] == 8);
    BTASSERT(request[1] == 0);
    BTASSERT(request[2] == 0xf7);
    BTASSERT(request[3] == 0xfd);
    BTASSERT(request[4] == 0);
    BTASSERT(request[5] == 1);
    BTASSERT(request[6] == 0);
    BTASSERT(request[7] == 1);

    return (0);
}

static int test_cmd_ping(struct harness_t *harness_p)
{
    uint8_t request[8];
    uint8_t reply[28];
    char buf[128];

    /* Prepare the socket stub with the reply packet. The first 20
       bytes in the reply are the IP header. */
    reply[20] = 0;
    reply[21] = 0;
    reply[22] = 0xff;
    reply[23] = 0xfc;
    reply[24] = 0;
    reply[25] = 2;
    reply[26] = 0;
    reply[27] = 1;
    socket_stub_input(reply, sizeof(reply));

    /* Perform the ping. */
    strcpy(buf, "inet/ping/ping 1.1.1.1\r\n");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    harness_expect(&qout, "Successfully pinged '1.1.1.1' in 0 ms.\r\n", NULL);
    
    /* Check the request send by the ping module. */
    socket_stub_output(request, sizeof(request));
    BTASSERT(request[0] == 8);
    BTASSERT(request[1] == 0);
    BTASSERT(request[2] == 0xf7);
    BTASSERT(request[3] == 0xfc);
    BTASSERT(request[4] == 0);
    BTASSERT(request[5] == 2);
    BTASSERT(request[6] == 0);
    BTASSERT(request[7] == 1);
        
    return (0);
}

static int test_cmd_ping_bad_reply(struct harness_t *harness_p)
{
    uint8_t reply[28];
    char buf[128];

    /* Prepare the socket stub with a bad reply packet. The first 20
       bytes in the reply are the IP header. */
    reply[20] = 0;
    reply[21] = 0;
    reply[22] = 0xff;
    reply[23] = 0xfe;
    reply[24] = 0;
    reply[25] = 2;
    reply[26] = 0;
    reply[27] = 2;
    socket_stub_input(reply, sizeof(reply));

    /* Perform the ping. */
    strcpy(buf, "inet/ping/ping 1.1.1.1\r\n");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "Failed to ping '1.1.1.1'.\r\n", NULL) > 0);
        
    return (0);
}

static int test_cmd_ping_bad_input(struct harness_t *harness_p)
{
    char buf[128];
    
    /* Too few arguemnts. */
    strcpy(buf, "inet/ping/ping\r\n");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout,
                            "Usage: ping <remote host>\r\n",
                            NULL) == 27);

    /* Bad ip address. */
    strcpy(buf, "inet/ping/ping a.b.c.d\r\n");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout,
                            "Bad ip address 'a.b.c.d'.\r\n",
                            NULL) == 27);
        
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_host_by_ip_address, "test_host_by_ip_address" },
        { test_bad_reply_crc, "test_bad_reply_crc" },
        { test_cmd_ping, "test_cmd_ping" },
        { test_cmd_ping_bad_reply, "test_cmd_ping_bad_reply" },
        { test_cmd_ping_bad_input, "test_cmd_ping_bad_input" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
