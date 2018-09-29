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

#ifndef __FLASH_MOCK_H__
#define __FLASH_MOCK_H__

#include "simba.h"

int mock_write_flash_module_init(int res);

int mock_write_flash_init(struct flash_device_t *dev_p,
                          int res);

int mock_write_flash_read(void *dst_p,
                          uintptr_t src,
                          size_t size,
                          ssize_t res);

int mock_write_flash_read_seq(void *dst_p,
                              uintptr_t src,
                              size_t size,
                              ssize_t res,
                              size_t length);

int mock_write_flash_read_seq2(void *dst_p,
                               uintptr_t src,
                               size_t size,
                               ssize_t res,
                               size_t length);

int mock_write_flash_write(uintptr_t dst,
                           const void *src_p,
                           size_t size,
                           ssize_t res);

int mock_write_flash_write_seq(uintptr_t dst,
                               const void *src_p,
                               size_t size,
                               ssize_t res,
                               size_t length);

int mock_write_flash_write_seq2(uintptr_t dst,
                               const void *src_p,
                               size_t size,
                               ssize_t res,
                               size_t length);

int mock_write_flash_erase(uintptr_t addr,
                           size_t size,
                           int res);

#endif
