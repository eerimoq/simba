/**
 * @section License
 *
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

#ifndef __JTAG_SOFT_MOCK_H__
#define __JTAG_SOFT_MOCK_H__

#include "simba.h"

int mock_write_jtag_soft_module_init(int res);

int mock_write_jtag_soft_init(struct pin_device_t *tck_p,
                              struct pin_device_t *tms_p,
                              struct pin_device_t *tdi_p,
                              struct pin_device_t *tdo_p,
                              int res);

int mock_write_jtag_soft_start(int res);

int mock_write_jtag_soft_stop(int res);

int mock_write_jtag_soft_reset(int res);

int mock_write_jtag_soft_instruction_write(const void *buf_p,
                                           size_t number_of_bits,
                                           int res);

int mock_write_jtag_soft_data_transfer(void *rxbuf_p,
                                       const void *txbuf_p,
                                       size_t number_of_bits,
                                       int res);

int mock_write_jtag_soft_data_read(void *buf_p,
                                   size_t number_of_bits,
                                   int res);

int mock_write_jtag_soft_data_write(const void *buf_p,
                                    size_t number_of_bits,
                                    int res);

int mock_write_jtag_soft_make_transition(int transition,
                                         int res);

#endif
