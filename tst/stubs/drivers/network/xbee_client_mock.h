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

#ifndef __XBEE_CLIENT_MOCK_H__
#define __XBEE_CLIENT_MOCK_H__

#include "simba.h"

int mock_write_xbee_client_module_init(int res);

int mock_write_xbee_client_init(void *chin_p,
                                void *chout_p,
                                void *buf_p,
                                size_t size,
                                int flags,
                                int res);

int mock_write_xbee_client_main(void *arg_p,
                                void *res);

int mock_write_xbee_client_read_from(void *buf_p,
                                     size_t size,
                                     struct xbee_client_address_t *address_p,
                                     ssize_t res);

int mock_write_xbee_client_write_to(const void *buf_p,
                                    size_t size,
                                    int flags,
                                    struct xbee_client_address_t *address_p,
                                    ssize_t res);

int mock_write_xbee_client_pin_set_mode(int pin,
                                        int mode,
                                        int res);

int mock_write_xbee_client_pin_read(int pin,
                                    int res);

int mock_write_xbee_client_pin_write(int pin,
                                     int value,
                                     int res);

int mock_write_xbee_client_pin_toggle(int pin,
                                      int res);

int mock_write_xbee_client_pin_convert(int pin,
                                       int res);

int mock_write_xbee_client_at_command_read(const char *command_p,
                                           uint8_t *parameter_p,
                                           size_t size,
                                           ssize_t res);

int mock_write_xbee_client_at_command_write(const char *command_p,
                                            const uint8_t *parameter_p,
                                            size_t size,
                                            int res);

int mock_write_xbee_client_at_command_read_u8(const char *command_p,
                                              uint8_t *parameter_p,
                                              int res);

int mock_write_xbee_client_at_command_write_u8(const char *command_p,
                                               uint8_t parameter,
                                               int res);

int mock_write_xbee_client_at_command_read_u16(const char *command_p,
                                               uint16_t *parameter_p,
                                               int res);

int mock_write_xbee_client_at_command_write_u16(const char *command_p,
                                                uint16_t parameter,
                                                int res);

int mock_write_xbee_client_at_command_read_u32(const char *command_p,
                                               uint32_t *parameter_p,
                                               int res);

int mock_write_xbee_client_at_command_write_u32(const char *command_p,
                                                uint32_t parameter,
                                                int res);

int mock_write_xbee_client_print_address(void *chan_p,
                                         struct xbee_client_address_t *address_p,
                                         int res);

#endif
