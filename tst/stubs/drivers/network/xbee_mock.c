/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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
#include "xbee_mock.h"

int mock_write_xbee_module_init(int res)
{
    harness_mock_write("xbee_module_init()",
                       NULL,
                       0);

    harness_mock_write("xbee_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_module_init)()
{
    int res;

    harness_mock_assert("xbee_module_init()",
                        NULL);

    harness_mock_read("xbee_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_init(void *chin_p,
                         void *chout_p,
                         int res)
{
    harness_mock_write("xbee_init(chin_p)",
                       chin_p,
                       sizeof(chin_p));

    harness_mock_write("xbee_init(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("xbee_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_init)(struct xbee_driver_t *self_p,
                                           void *chin_p,
                                           void *chout_p)
{
    int res;

    harness_mock_assert("xbee_init(chin_p)",
                        chin_p);

    harness_mock_assert("xbee_init(chout_p)",
                        chout_p);

    harness_mock_read("xbee_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_read(struct xbee_frame_t *frame_p,
                         int res)
{
    harness_mock_write("xbee_read(): return (frame_p)",
                       frame_p,
                       sizeof(*frame_p));

    harness_mock_write("xbee_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_read)(struct xbee_driver_t *self_p,
                                           struct xbee_frame_t *frame_p)
{
    int res;

    harness_mock_read("xbee_read(): return (frame_p)",
                      frame_p,
                      -1);

    harness_mock_read("xbee_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_write(const struct xbee_frame_t *frame_p,
                          int res)
{
    harness_mock_write("xbee_write(frame_p)",
                       frame_p,
                       sizeof(*frame_p));

    harness_mock_write("xbee_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_write)(struct xbee_driver_t *self_p,
                                            const struct xbee_frame_t *frame_p)
{
    int res;

    harness_mock_assert("xbee_write(frame_p)",
                        frame_p);

    harness_mock_read("xbee_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_print_frame(void *chan_p,
                                struct xbee_frame_t *frame_p,
                                int res)
{
    harness_mock_write("xbee_print_frame(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("xbee_print_frame(frame_p)",
                       frame_p,
                       sizeof(*frame_p));

    harness_mock_write("xbee_print_frame(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_print_frame)(void *chan_p,
                                                  struct xbee_frame_t *frame_p)
{
    int res;

    harness_mock_assert("xbee_print_frame(chan_p)",
                        chan_p);

    harness_mock_assert("xbee_print_frame(frame_p)",
                        frame_p);

    harness_mock_read("xbee_print_frame(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_frame_type_as_string(uint8_t frame_type,
                                         const char *res)
{
    harness_mock_write("xbee_frame_type_as_string(frame_type)",
                       &frame_type,
                       sizeof(frame_type));

    harness_mock_write("xbee_frame_type_as_string(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(xbee_frame_type_as_string)(uint8_t frame_type)
{
    const char *res;

    harness_mock_assert("xbee_frame_type_as_string(frame_type)",
                        &frame_type);

    harness_mock_read("xbee_frame_type_as_string(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_tx_status_as_string(uint8_t tx_status,
                                        const char *res)
{
    harness_mock_write("xbee_tx_status_as_string(tx_status)",
                       &tx_status,
                       sizeof(tx_status));

    harness_mock_write("xbee_tx_status_as_string(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(xbee_tx_status_as_string)(uint8_t tx_status)
{
    const char *res;

    harness_mock_assert("xbee_tx_status_as_string(tx_status)",
                        &tx_status);

    harness_mock_read("xbee_tx_status_as_string(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_modem_status_as_string(uint8_t modem_status,
                                           const char *res)
{
    harness_mock_write("xbee_modem_status_as_string(modem_status)",
                       &modem_status,
                       sizeof(modem_status));

    harness_mock_write("xbee_modem_status_as_string(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(xbee_modem_status_as_string)(uint8_t modem_status)
{
    const char *res;

    harness_mock_assert("xbee_modem_status_as_string(modem_status)",
                        &modem_status);

    harness_mock_read("xbee_modem_status_as_string(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_at_command_response_status_as_string(uint8_t response_status,
                                                         const char *res)
{
    harness_mock_write("xbee_at_command_response_status_as_string(response_status)",
                       &response_status,
                       sizeof(response_status));

    harness_mock_write("xbee_at_command_response_status_as_string(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(xbee_at_command_response_status_as_string)(uint8_t response_status)
{
    const char *res;

    harness_mock_assert("xbee_at_command_response_status_as_string(response_status)",
                        &response_status);

    harness_mock_read("xbee_at_command_response_status_as_string(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
