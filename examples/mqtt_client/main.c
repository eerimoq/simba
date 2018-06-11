/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
 * Copyright (c) 2017-2018, Google Inc.
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

static struct socket_t server_sock;
static struct mqtt_client_t client;

static THRD_STACK(stack, 4096);
static int message_count = 0;

//! Callback when we get an message from the MQTT broker.
static size_t on_publish(struct mqtt_client_t *client_p,
                         const char *topic_p,
                         void *chin_p,
                         size_t size)
{
    uint8_t buf[16];
    int unread, toread;

    std_printf(OSTR("on_publish: %s = "), topic_p);

    /*
     * We read the value in chunks as our buffer may be to small, as
     * MQTT allows up to 256MB messages. This complicates the code a
     * bit, but we must read all the bytes from the channel to avoid
     * getting out of sync with the mqtt_client thread.
     */
    for (unread = size; unread > 0; unread -= toread) {
        toread = MIN(unread, sizeof(buf) - 1);
        chan_read(chin_p, buf, toread);
        buf[toread] = '\0';
        std_printf(OSTR("%s"), &buf[0]);
    }

    std_printf(OSTR("\r\n"));

    message_count++;

    return (0);
}

static int mqtt_init(void)
{
    struct thrd_t *thrd_p;
    int port = REMOTE_HOST_PORT;
    char remote_host_ip[] = STRINGIFY(REMOTE_HOST_IP);
    struct inet_addr_t remote_host_address;
    int res;

    /* Setup TCP connection to MQTT broker. */
    std_printf(OSTR("Connecting to MQTT Broker '%s:%d'.\r\n"),
               remote_host_ip,
               port);
    remote_host_address.port = port;
    res = inet_aton(remote_host_ip, &remote_host_address.ip);

    if (res != 0) {
        std_printf(OSTR("Failed to convert '%s' to IP address: %d'\r\n"),
                   remote_host_ip,
                   res);
        return (res);
    }

    res = socket_open_tcp(&server_sock);

    if (res != 0) {
        std_printf(OSTR("Failed to open TCP socket: %d'\r\n"),
                   res);
        return (res);
    }

    res = socket_connect(&server_sock, &remote_host_address);

    if (res != 0) {
        std_printf(OSTR("Failed to connect TCP socket to broker: %d'\r\n"),
                   res);
        return (res);
    }

    res = mqtt_client_init(&client,
                           "mqtt_client",
                           NULL,
                           &server_sock,
                           &server_sock,
                           on_publish,
                           NULL);

    if (res != 0) {
        std_printf(OSTR("Failed to initialized MQTT client: %d'\r\n"),
                   res);
        return (res);
    }

    /* Start the thread which will do the actual work. */
    thrd_p = thrd_spawn(mqtt_client_main,
                        &client,
                        0,
                        stack,
                        sizeof(stack));

    if (thrd_p == NULL) {
        std_printf(OSTR("Failed to spawn thread for MQTT client'\r\n"));
        return (-1);
    }

    /* Set the mqtt_client thread to log errors. */
    thrd_set_log_mask(thrd_p, LOG_UPTO(DEBUG));

    /* Start MQTT connection. */
    res = mqtt_client_connect(&client, NULL);

    if (res != 0) {
        std_printf(OSTR("Failed to connect to Broker: %d'\r\n"),
                   res);
        return (res);
    }

    /*
     * We ping the broke to see if we have a connection.  This is not
     * required, just a sanity check.
     */
    res = mqtt_client_ping(&client);

    if (res != 0) {
        std_printf(OSTR("Failed to ping Broker: %d'\r\n"),
                   res);
        return (res);
    }

    return (0);
}

static void publish_connected(void)
{
    struct mqtt_application_message_t pub;
    int res;

    pub.topic.buf_p = "simba/test/connected";
    pub.topic.size = strlen(pub.topic.buf_p);
    pub.payload.buf_p = "true";
    pub.payload.size = strlen(pub.payload.buf_p);
    pub.qos = mqtt_qos_1_t;

    res = mqtt_client_publish(&client, &pub);

    if (res != 0) {
        std_printf(OSTR("Failed to publish %s = %s: %d\r\n"),
                   pub.topic.buf_p,
                   pub.payload.buf_p,
                   res);
    }
}

static void subscribe(void)
{
    struct mqtt_application_message_t sub;
    int res;

    sub.topic.buf_p = "simba/test/sub0";
    sub.topic.size = strlen(sub.topic.buf_p);
    sub.qos = mqtt_qos_1_t;

    std_printf(OSTR("Subscribing to %s\r\n"), sub.topic.buf_p);

    res = mqtt_client_subscribe(&client, &sub);

    if (res != 0) {
        std_printf(OSTR("Failed to subscribe %s: %d\r\n"),
                   sub.topic.buf_p,
                   res);
    }
}

static void unsubscribe(void)
{
    struct mqtt_application_message_t unsub;
    int res;

    unsub.topic.buf_p = "simba/test/sub0";
    unsub.topic.size = strlen(unsub.topic.buf_p);

    std_printf(OSTR("Unsubscribing from %s\r\n"), unsub.topic.buf_p);

    res = mqtt_client_unsubscribe(&client, &unsub);

    if (res != 0) {
        std_printf(OSTR("Failed to unsubscribe %s: %d\r\n"),
                   unsub.topic.buf_p,
                   res);
    }
}

int main()
{
    int res;

    /* Initialization. */
    sys_start();

    /* Give serial console time to attach. */
    thrd_sleep(1);

    /* Connect and start operations. */
    mqtt_init();

    /* Publish a message. */
    publish_connected();

    /* Subscribe to our single topic. */
    subscribe();

    while (message_count < 100) {
        std_printf(OSTR("Got %d messages\r\n"), message_count);

        /*
         * We must ensure we sent traffic periodically (at least every
         * 10 seconds) to the MQTT broker, otherwise it will close the
         * connection.
         */
        res = mqtt_client_ping(&client);

        if (res != 0) {
            std_printf(OSTR("Failed to ping Broker: %d\r\n"),
                   res);
        }

        thrd_sleep(5);
    }

    /* Unsbuscribe from our topic. */
    unsubscribe();

    return (0);
}
