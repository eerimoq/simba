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

#ifndef __SPI_MOCK_H__
#define __SPI_MOCK_H__

#include "simba.h"

int mock_write_spi_module_init(int res);

int mock_write_spi_init(struct spi_device_t *dev_p,
                        struct pin_device_t *ss_pin_p,
                        int mode,
                        int speed,
                        int polarity,
                        int phase,
                        int res);

int mock_write_spi_start(int res);

int mock_write_spi_stop(int res);

int mock_write_spi_take_bus(int res);

int mock_write_spi_give_bus(int res);

int mock_write_spi_select(int res);

int mock_write_spi_deselect(int res);

int mock_write_spi_transfer(void *rxbuf_p,
                            const void *txbuf_p,
                            size_t size,
                            ssize_t res);

int mock_write_spi_read(void *buf_p,
                        size_t size,
                        ssize_t res);

int mock_write_spi_write(const void *buf_p,
                         size_t size,
                         ssize_t res);

int mock_write_spi_get(uint8_t *data_p,
                       ssize_t res);

int mock_write_spi_put(uint8_t data,
                       ssize_t res);

#endif
