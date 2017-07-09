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

struct message_t {
    void *buf_p;
    size_t size;
};

static struct mqtt_client_t client;
static struct queue_t qout;
static struct queue_t qin;
static struct queue_t qserverout;
static struct queue_t qserverin;
static char qoutbuf[64];
static char qinbuf[64];
static char qserveroutbuf[64];
static char qserverinbuf[64];
static struct thrd_t *self_p;

THRD_STACK(stack, 1024);
THRD_STACK(server_stack, 512);

static void *server_main(void *arg_p)
{
    int i;
    struct message_t message;
    char byte;

    thrd_set_name("mqtt_server");

    while (1) {
        /* Read a message from the test case. */
        queue_read(&qserverin, &message, sizeof(message));

        if (message.buf_p == NULL) {
            /* Read the message written by the client, and write it to the
               test thread queue. */
            for (i = 0; i < message.size; i++) {
                chan_read(&qout, &byte, sizeof(byte));
                chan_write(&qserverout, &byte, sizeof(byte));
            }
        } else {
            /* Write the response to the client. */
            chan_write(&qin, message.buf_p, message.size);
        }
    }

    return (NULL);
}

static size_t on_publish(struct mqtt_client_t *client_p,
                         const char *topic_p,
                         void *chin_p,
                         size_t size)
{
    uint8_t buf[16];

    chan_read(chin_p, buf, size);
    thrd_resume(self_p, 0);

    return (0);
}

static int on_error(struct mqtt_client_t *client_p,
                    int error)
{
    std_printf(FSTR("error = %d\r\n"), error);

    return (0);
}

static int test_init(struct harness_t *harness_p)
{
    struct thrd_t *thrd_p;

    BTASSERT(queue_init(&qout, qoutbuf, sizeof(qoutbuf)) == 0);
    BTASSERT(queue_init(&qin, qinbuf, sizeof(qinbuf)) == 0);
    BTASSERT(queue_init(&qserverout, qserveroutbuf, sizeof(qserveroutbuf)) == 0);
    BTASSERT(queue_init(&qserverin, qserverinbuf, sizeof(qserverinbuf)) == 0);
    BTASSERT(mqtt_client_init(&client,
                              "mqtt_client",
                              NULL,
                              &qout,
                              &qin,
                              on_publish,
                              on_error) == 0);

    thrd_p = thrd_spawn(mqtt_client_main,
                        &client,
                        0,
                        stack,
                        sizeof(stack));

    thrd_set_log_mask(thrd_p, LOG_UPTO(DEBUG));

    BTASSERT(thrd_spawn(server_main,
                        NULL,
                        0,
                        server_stack,
                        sizeof(server_stack)) != NULL);

    return (0);
}

static int test_connect(struct harness_t *harness_p)
{
    struct message_t message;
    uint8_t buf[16];

    /* Prepare the server to receive the connection message. */
    message.buf_p = NULL;
    message.size = 14;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Prepare the server to send the connection ack message. */
    buf[0] = 0x20;
    buf[1] = 2;
    buf[2] = 0;
    buf[3] = 0;
    message.buf_p = buf;
    message.size = 4;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Connect. */
    BTASSERTI(mqtt_client_connect(&client), ==, 0);

    BTASSERTI(queue_read(&qserverout, buf, 2), ==, 2);
    BTASSERTI(buf[0], ==, 0x10);
    BTASSERTI(buf[1], ==, 12);

    BTASSERTI(queue_read(&qserverout, buf, 12), ==, 12);
    BTASSERTI(buf[0], ==, 0);
    BTASSERTI(buf[1], ==, 4);
    BTASSERTI(buf[2], ==, 'M');
    BTASSERTI(buf[3], ==, 'Q');
    BTASSERTI(buf[4], ==, 'T');
    BTASSERTI(buf[5], ==, 'T');
    BTASSERTI(buf[6], ==, 4);
    BTASSERTI(buf[7], ==, 0x02);
    BTASSERTI(buf[8], ==, 1);
    BTASSERTI(buf[9], ==, 0x2c);
    BTASSERTI(buf[10], ==, 0);
    BTASSERTI(buf[11], ==, 0);

    return (0);
}

static int test_ping(struct harness_t *harness_p)
{
    struct message_t message;
    uint8_t response[2];
    uint8_t request[2];

    /* Prepare the server to receive the ping request packet. */
    message.buf_p = NULL;
    message.size = 2;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Prepare the server to send the ping response packet. */
    response[0] = (13 << 4);
    response[1] = 0;
    message.buf_p = response;
    message.size = 2;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Send the ping. */
    BTASSERT(mqtt_client_ping(&client) == 0);

    /* Verify the ping message. */
    BTASSERT(queue_read(&qserverout, request, 2) == 2);
    BTASSERT(request[0] == (12 << 4));
    BTASSERT(request[1] == 0);

    return (0);
}

static int test_publish(struct harness_t *harness_p)
{
    struct mqtt_application_message_t foobar;
    struct message_t message;
    uint8_t buf[16];

    /* Prepare the server to receive the publish message. */
    message.buf_p = NULL;
    message.size = 16;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Prepare the server to send the publish ack message. */
    buf[0] = (4 << 4);
    buf[1] = 2;
    buf[2] = 0;
    buf[3] = 1;
    message.buf_p = buf;
    message.size = 4;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Publish a message. */
    foobar.topic.buf_p = "foo/bar";
    foobar.topic.size = 7;
    foobar.payload.buf_p = "fie";
    foobar.payload.size = 3;
    foobar.qos = 1;

    BTASSERT(mqtt_client_publish(&client, &foobar) == 0);

    BTASSERT(queue_read(&qserverout, buf, 16) == 16);
    BTASSERT(buf[0] == ((3 << 4) | (1 << 1)));
    BTASSERT(buf[1] == 14);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 7);
    BTASSERT(buf[4] == 'f');
    BTASSERT(buf[5] == 'o');
    BTASSERT(buf[6] == 'o');
    BTASSERT(buf[7] == '/');
    BTASSERT(buf[8] == 'b');
    BTASSERT(buf[9] == 'a');
    BTASSERT(buf[10] == 'r');
    BTASSERT(buf[11] == 0);
    BTASSERT(buf[12] == 1);
    BTASSERT(buf[13] == 'f');
    BTASSERT(buf[14] == 'i');
    BTASSERT(buf[15] == 'e');

    return (0);
}

static int test_subscribe(struct harness_t *harness_p)
{
    uint8_t buf[16];
    struct message_t message;
    struct mqtt_application_message_t foobar;

    /* Prepare the server to receive the subscribe message. */
    message.buf_p = NULL;
    message.size = 14;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Prepare the server to send the subscibe ack message. */
    buf[0] = (9 << 4);
    buf[1] = 3;
    buf[2] = 0;
    buf[3] = 1;
    buf[4] = 0;
    message.buf_p = buf;
    message.size = 5;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Subscribe. */
    foobar.topic.buf_p = "foo/bar";
    foobar.topic.size = 7;
    foobar.qos = mqtt_qos_1_t;
    BTASSERT(mqtt_client_subscribe(&client, &foobar) == 0);

    BTASSERT(queue_read(&qserverout, buf, 14) == 14);
    BTASSERT(buf[0] == ((8 << 4) | 2));
    BTASSERT(buf[1] == 12);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 1);
    BTASSERT(buf[4] == 0);
    BTASSERT(buf[5] == 7);
    BTASSERT(buf[6] == 'f');
    BTASSERT(buf[7] == 'o');
    BTASSERT(buf[8] == 'o');
    BTASSERT(buf[9] == '/');
    BTASSERT(buf[10] == 'b');
    BTASSERT(buf[11] == 'a');
    BTASSERT(buf[12] == 'r');
    BTASSERT(buf[13] == 0x01);

    /* Prepare the server to send a publish message. */
    buf[0] = (3 << 4);
    buf[1] = 14;
    buf[2] = 0;
    buf[3] = 7;
    buf[4] = 'f';
    buf[5] = 'o';
    buf[6] = 'o';
    buf[7] = '/';
    buf[8] = 'b';
    buf[9] = 'a';
    buf[10] = 'r';
    buf[11] = 0;
    buf[12] = 1;
    buf[13] = 'f';
    buf[14] = 'i';
    buf[15] = 'e';
    message.buf_p = buf;
    message.size = 16;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Resumed from the callback. */
    thrd_suspend(NULL);

    /* Prepare the server to receive the unsubscribe message. */
    message.buf_p = NULL;
    message.size = 13;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Prepare the server to send the unsubscibe ack message. */
    buf[0] = (11 << 4);
    buf[1] = 2;
    buf[2] = 0;
    buf[3] = 2;
    message.buf_p = buf;
    message.size = 4;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Unsubscribe. */
    foobar.topic.buf_p = "foo/bar";
    foobar.topic.size = 7;
    BTASSERT(mqtt_client_unsubscribe(&client, &foobar) == 0);

    BTASSERT(queue_read(&qserverout, buf, 13) == 13);
    BTASSERT(buf[0] == ((10 << 4) | 2));
    BTASSERT(buf[1] == 11);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 2);
    BTASSERT(buf[4] == 0);
    BTASSERT(buf[5] == 7);
    BTASSERT(buf[6] == 'f');
    BTASSERT(buf[7] == 'o');
    BTASSERT(buf[8] == 'o');
    BTASSERT(buf[9] == '/');
    BTASSERT(buf[10] == 'b');
    BTASSERT(buf[11] == 'a');
    BTASSERT(buf[12] == 'r');

    return (0);
}

static int test_disconnect(struct harness_t *harness_p)
{
    struct message_t message;
    uint8_t buf[16];

    /* Prepare the server to receive the disconnect message. */
    message.buf_p = NULL;
    message.size = 2;
    BTASSERT(queue_write(&qserverin, &message, sizeof(message)) == sizeof(message));

    /* Disconnect. */
    BTASSERT(mqtt_client_disconnect(&client) == 0);

    BTASSERT(queue_read(&qserverout, buf, 2) == 2);
    BTASSERT(buf[0] == (14 << 4));
    BTASSERT(buf[1] == 0);

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

    self_p = thrd_self();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
