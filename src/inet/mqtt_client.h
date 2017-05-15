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

#ifndef __INET_MQTT_CLIENT_H__
#define __INET_MQTT_CLIENT_H__

#include "simba.h"

/** Client states. */
enum mqtt_client_state_t {
    mqtt_client_state_disconnected_t,
    mqtt_client_state_connected_t,
    mqtt_client_state_connecting_t
};

/** Quality of Service. */
enum mqtt_qos_t {
    mqtt_qos_0_t = 0,
    mqtt_qos_1_t = 1,
    mqtt_qos_2_t = 2
};

struct mqtt_client_t;

/**
 * Prototype of the on-publish callback function.
 *
 * @param[in] client_p The client.
 * @param[in] topic_p The received topic.
 * @param[in] chin_p The channel to read the value from.
 * @param[in] size Number of bytes of the value to read from chin_p.
 *
 * @return Number of bytes read from the input channel.
 */
typedef size_t (*mqtt_on_publish_t)(struct mqtt_client_t *client_p,
                                    const char *topic_p,
                                    void *chin_p,
                                    size_t size);

/**
 * Prototype of the on-error callback function.
 *
 * @param[in] client_p The client.
 * @param[in] error The number of error that occured.
 *
 * @return zero(0) or nagative error code.
 */
typedef int (*mqtt_on_error_t)(struct mqtt_client_t *client_p,
                               int error);

/**
 * MQTT client.
 */
struct mqtt_client_t {
    const char *name_p;
    struct log_object_t *log_object_p;
    int state;
    struct {
        int type;
        void *data_p;
    } message;
    struct {
        void *out_p;
        void *in_p;
    } transport;
    struct {
        struct queue_t out;
        struct queue_t in;
    } control;
    mqtt_on_publish_t on_publish;
    mqtt_on_error_t on_error;
};

/**
 * MQTT application message.
 */
struct mqtt_application_message_t {
    struct {
        const char *buf_p;
        size_t size;
    } topic;
    struct {
        const void *buf_p;
        size_t size;
    } payload;
    enum mqtt_qos_t qos;
};

/**
 * Initialize given MQTT client.
 *
 * @param[in] self_p MQTT client.
 * @param[in] name_p Name of the thread.
 * @param[in] log_object_p Log object.
 * @param[in] chout_p Output channel for client to server packets.
 * @param[in] chin_p Input channel for server to client packets.
 * @param[in] on_publish On-publish callback function. Called when the
 *                       server publishes a message.
 * @param[in] on_error On-error callback function. Called when an error
 *                     occurs. If NULL, a default handler is used.
 *
 * @return zero(0) or negative error code.
 */
int mqtt_client_init(struct mqtt_client_t *self_p,
                     const char *name_p,
                     struct log_object_t *log_object_p,
                     void *chout_p,
                     void *chin_p,
                     mqtt_on_publish_t on_publish,
                     mqtt_on_error_t on_error);

/**
 * MQTT client thread.
 *
 * @param[in] arg_p MQTT client.
 *
 * @return Never returns.
 */
void *mqtt_client_main(void *arg_p);

/**
 * Establish a connection to the server.
 *
 * @param[in] self_p MQTT client.
 *
 * @return zero(0) or negative error code.
 */
int mqtt_client_connect(struct mqtt_client_t *self_p);

/**
 * Disconnect from the server.
 *
 * @param[in] self_p MQTT client.
 *
 * @return zero(0) or negative error code.
 */
int mqtt_client_disconnect(struct mqtt_client_t *self_p);

/**
 * Send a ping request to the server (broker) and wait for the ping
 * response.
 *
 * @param[in] self_p MQTT client.
 *
 * @return zero(0) or negative error code.
 */
int mqtt_client_ping(struct mqtt_client_t *self_p);

/**
 * Publish given topic.
 *
 * @param[in] self_p MQTT client.
 * @param[in] topic_p Topic.
 * @param[in] payload_p Payload to publish. May be NULL.
 * @param[in] payload_size Number of bytes in the payload.
 *
 * @return zero(0) or negative error code.
 */
int mqtt_client_publish(struct mqtt_client_t *self_p,
                        struct mqtt_application_message_t *message_p);

/**
 * Subscribe to given message.
 *
 * @param[in] self_p MQTT client.
 * @param[in] message_p The message to subscribe to. The payload part
 *                      of the message is not used. The topic may use
 *                      wildcards, given that the server supports it.
 *
 * @return zero(0) or negative error code.
 */
int mqtt_client_subscribe(struct mqtt_client_t *self_p,
                          struct mqtt_application_message_t *message_p);

/**
 * Unsubscribe from given message.
 *
 * @param[in] self_p MQTT client.
 * @param[in] message_p The message to unsubscribe from. Only the
 *                      topic in the message is used.
 *
 * @return zero(0) or negative error code.
 */
int mqtt_client_unsubscribe(struct mqtt_client_t *self_p,
                            struct mqtt_application_message_t *message_p);

#endif
