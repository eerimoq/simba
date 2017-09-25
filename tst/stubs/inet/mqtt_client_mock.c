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
#include "mqtt_client_mock.h"

int mock_write_mqtt_client_init(const char *name_p,
                                struct log_object_t *log_object_p,
                                void *chout_p,
                                void *chin_p,
                                mqtt_on_publish_t on_publish,
                                mqtt_on_error_t on_error,
                                int res)
{
    harness_mock_write("mqtt_client_init(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("mqtt_client_init(log_object_p)",
                       log_object_p,
                       sizeof(*log_object_p));

    harness_mock_write("mqtt_client_init(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("mqtt_client_init(chin_p)",
                       chin_p,
                       sizeof(chin_p));

    harness_mock_write("mqtt_client_init(on_publish)",
                       &on_publish,
                       sizeof(on_publish));

    harness_mock_write("mqtt_client_init(on_error)",
                       &on_error,
                       sizeof(on_error));

    harness_mock_write("mqtt_client_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mqtt_client_init)(struct mqtt_client_t *self_p,
                                                  const char *name_p,
                                                  struct log_object_t *log_object_p,
                                                  void *chout_p,
                                                  void *chin_p,
                                                  mqtt_on_publish_t on_publish,
                                                  mqtt_on_error_t on_error)
{
    int res;

    harness_mock_assert("mqtt_client_init(name_p)",
                        name_p);

    harness_mock_assert("mqtt_client_init(log_object_p)",
                        log_object_p);

    harness_mock_assert("mqtt_client_init(chout_p)",
                        chout_p);

    harness_mock_assert("mqtt_client_init(chin_p)",
                        chin_p);

    harness_mock_assert("mqtt_client_init(on_publish)",
                        &on_publish);

    harness_mock_assert("mqtt_client_init(on_error)",
                        &on_error);

    harness_mock_read("mqtt_client_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mqtt_client_main(void *arg_p,
                                void *res)
{
    harness_mock_write("mqtt_client_main(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("mqtt_client_main(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(mqtt_client_main)(void *arg_p)
{
    void *res;

    harness_mock_assert("mqtt_client_main(arg_p)",
                        arg_p);

    harness_mock_read("mqtt_client_main(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mqtt_client_connect(int res)
{
    harness_mock_write("mqtt_client_connect(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mqtt_client_connect)(struct mqtt_client_t *self_p)
{
    int res;

    harness_mock_read("mqtt_client_connect(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mqtt_client_disconnect(int res)
{
    harness_mock_write("mqtt_client_disconnect(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mqtt_client_disconnect)(struct mqtt_client_t *self_p)
{
    int res;

    harness_mock_read("mqtt_client_disconnect(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mqtt_client_ping(int res)
{
    harness_mock_write("mqtt_client_ping(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mqtt_client_ping)(struct mqtt_client_t *self_p)
{
    int res;

    harness_mock_read("mqtt_client_ping(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mqtt_client_publish(struct mqtt_application_message_t *message_p,
                                   int res)
{
    harness_mock_write("mqtt_client_publish(): return (message_p)",
                       message_p,
                       sizeof(*message_p));

    harness_mock_write("mqtt_client_publish(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mqtt_client_publish)(struct mqtt_client_t *self_p,
                                                     struct mqtt_application_message_t *message_p)
{
    int res;

    harness_mock_read("mqtt_client_publish(): return (message_p)",
                      message_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_read("mqtt_client_publish(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mqtt_client_subscribe(struct mqtt_application_message_t *message_p,
                                     int res)
{
    harness_mock_write("mqtt_client_subscribe(message_p)",
                       message_p,
                       sizeof(*message_p));

    harness_mock_write("mqtt_client_subscribe(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mqtt_client_subscribe)(struct mqtt_client_t *self_p,
                                                       struct mqtt_application_message_t *message_p)
{
    int res;

    harness_mock_assert("mqtt_client_subscribe(message_p)",
                        message_p);

    harness_mock_read("mqtt_client_subscribe(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_mqtt_client_unsubscribe(struct mqtt_application_message_t *message_p,
                                       int res)
{
    harness_mock_write("mqtt_client_unsubscribe(message_p)",
                       message_p,
                       sizeof(*message_p));

    harness_mock_write("mqtt_client_unsubscribe(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(mqtt_client_unsubscribe)(struct mqtt_client_t *self_p,
                                                         struct mqtt_application_message_t *message_p)
{
    int res;

    harness_mock_assert("mqtt_client_unsubscribe(message_p)",
                        message_p);

    harness_mock_read("mqtt_client_unsubscribe(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
