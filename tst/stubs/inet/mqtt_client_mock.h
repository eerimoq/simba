/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#ifndef __MQTT_CLIENT_MOCK_H__
#define __MQTT_CLIENT_MOCK_H__

#include "simba.h"

int mock_write_mqtt_client_init(const char *name_p,
                                struct log_object_t *log_object_p,
                                void *chout_p,
                                void *chin_p,
                                mqtt_on_publish_t on_publish,
                                mqtt_on_error_t on_error,
                                int res);

int mock_write_mqtt_client_main(void *arg_p,
                                void *res);

int mock_write_mqtt_client_connect(int res);

int mock_write_mqtt_client_disconnect(int res);

int mock_write_mqtt_client_ping(int res);

int mock_write_mqtt_client_publish(struct mqtt_application_message_t *message_p,
                                   int res);

int mock_write_mqtt_client_subscribe(struct mqtt_application_message_t *message_p,
                                     int res);

int mock_write_mqtt_client_unsubscribe(struct mqtt_application_message_t *message_p,
                                       int res);

#endif
