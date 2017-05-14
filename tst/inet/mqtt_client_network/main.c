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

#ifndef REMOTE_HOST_IP
#   define REMOTE_HOST_IP   192.168.0.4
#endif

#ifndef REMOTE_HOST_PORT
#   define REMOTE_HOST_PORT       10000
#endif

struct message_t {
    void *buf_p;
    size_t size;
};

static struct socket_t server_sock;
static struct mqtt_client_t client;
static struct thrd_t *self_p;

static THRD_STACK(stack, 4096);

static size_t on_publish(struct mqtt_client_t *client_p,
                         const char *topic_p,
                         void *chin_p,
                         size_t size)
{
    uint8_t buf[32];
    
    chan_read(chin_p, buf, size);
    buf[size] = '\0';
    std_printf(FSTR("on_publish: %s\r\n"), &buf[0]);

    thrd_resume(self_p, 0);

    return (0);
}

static int test_init(struct harness_t *harness_p)
{
    struct thrd_t *thrd_p;
    int port = REMOTE_HOST_PORT;
    char remote_host_ip[] = STRINGIFY(REMOTE_HOST_IP);
    struct inet_addr_t remote_host_address;

    self_p = thrd_self();

    std_printf(FSTR("Connecting to '%s:%d'.\r\n"), remote_host_ip, port);

    BTASSERT(inet_aton(remote_host_ip, &remote_host_address.ip) == 0);
    remote_host_address.port = port;

    BTASSERT(socket_open_tcp(&server_sock) == 0);
    BTASSERT(socket_connect(&server_sock, &remote_host_address) == 0);

    BTASSERT(mqtt_client_init(&client,
                              "mqtt_client",
                              NULL,
                              &server_sock,
                              &server_sock,
                              on_publish,
                              NULL) == 0);

    thrd_p = thrd_spawn(mqtt_client_main,
                        &client,
                        0,
                        stack,
                        sizeof(stack));

    thrd_set_log_mask(thrd_p, LOG_UPTO(DEBUG));

    return (0);
}

static int test_connect(struct harness_t *harness_p)
{
    BTASSERT(mqtt_client_connect(&client) == 0);

    return (0);
}

static int test_ping(struct harness_t *harness_p)
{
    BTASSERT(mqtt_client_ping(&client) == 0);

    return (0);
}

static int test_publish(struct harness_t *harness_p)
{
    struct mqtt_application_message_t foobar;

    foobar.topic.buf_p = "foo/bar";
    foobar.topic.size = 7;
    foobar.payload.buf_p = "fie";
    foobar.payload.size = 3;
    foobar.qos = 1;

    BTASSERT(mqtt_client_publish(&client, &foobar) == 0);

    return (0);
}

static int test_subscribe(struct harness_t *harness_p)
{
    struct mqtt_application_message_t foobar;

    /* Subscribe. */
    foobar.topic.buf_p = "foo/bar";
    foobar.topic.size = 7;
    foobar.qos = mqtt_qos_1_t;
    BTASSERT(mqtt_client_subscribe(&client, &foobar) == 0);

    thrd_sleep_ms(500);
    
    /* Unsubscribe. */
    foobar.topic.buf_p = "foo/bar";
    foobar.topic.size = 7;
    BTASSERT(mqtt_client_unsubscribe(&client, &foobar) == 0);

    return (0);
}

static int test_disconnect(struct harness_t *harness_p)
{
    BTASSERT(mqtt_client_disconnect(&client) == 0);
    BTASSERT(socket_close(&server_sock) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_connect, "test_connect" },
        { test_ping, "test_ping" },
        { test_publish, "test_publish" },
        { test_subscribe, "test_subscribe" },
        { test_disconnect, "test_disconnect" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
