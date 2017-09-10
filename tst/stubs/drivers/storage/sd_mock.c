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
#include "sd_mock.h"

int mock_write_sd_init(struct spi_driver_t *spi_p,
                       int res)
{
    harness_mock_write("sd_init(): return (spi_p)",
                       spi_p,
                       sizeof(*spi_p));

    harness_mock_write("sd_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sd_init)(struct sd_driver_t *self_p,
                                         struct spi_driver_t *spi_p)
{
    int res;

    harness_mock_read("sd_init(): return (spi_p)",
                      spi_p,
                      -1);

    harness_mock_read("sd_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sd_start(int res)
{
    harness_mock_write("sd_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sd_start)(struct sd_driver_t *self_p)
{
    int res;

    harness_mock_read("sd_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sd_stop(int res)
{
    harness_mock_write("sd_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sd_stop)(struct sd_driver_t *self_p)
{
    int res;

    harness_mock_read("sd_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sd_read_cid(struct sd_cid_t* cid_p,
                           ssize_t res)
{
    harness_mock_write("sd_read_cid(): return (cid_p)",
                       cid_p,
                       sizeof(*cid_p));

    harness_mock_write("sd_read_cid(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(sd_read_cid)(struct sd_driver_t *self_p,
                                                 struct sd_cid_t* cid_p)
{
    ssize_t res;

    harness_mock_read("sd_read_cid(): return (cid_p)",
                      cid_p,
                      -1);

    harness_mock_read("sd_read_cid(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sd_read_csd(union sd_csd_t* csd_p,
                           ssize_t res)
{
    harness_mock_write("sd_read_csd(): return (csd_p)",
                       csd_p,
                       sizeof(*csd_p));

    harness_mock_write("sd_read_csd(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(sd_read_csd)(struct sd_driver_t *self_p,
                                                 union sd_csd_t* csd_p)
{
    ssize_t res;

    harness_mock_read("sd_read_csd(): return (csd_p)",
                      csd_p,
                      -1);

    harness_mock_read("sd_read_csd(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sd_erase_blocks(uint32_t start_block,
                               uint32_t end_block,
                               int res)
{
    harness_mock_write("sd_erase_blocks(start_block)",
                       &start_block,
                       sizeof(start_block));

    harness_mock_write("sd_erase_blocks(end_block)",
                       &end_block,
                       sizeof(end_block));

    harness_mock_write("sd_erase_blocks(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sd_erase_blocks)(struct sd_driver_t *self_p,
                                                 uint32_t start_block,
                                                 uint32_t end_block)
{
    int res;

    harness_mock_assert("sd_erase_blocks(start_block)",
                        &start_block);

    harness_mock_assert("sd_erase_blocks(end_block)",
                        &end_block);

    harness_mock_read("sd_erase_blocks(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sd_read_block(void *dst_p,
                             uint32_t src_block,
                             ssize_t res)
{
    harness_mock_write("sd_read_block(): return (dst_p)",
                       dst_p,
                       sizeof(dst_p));

    harness_mock_write("sd_read_block(src_block)",
                       &src_block,
                       sizeof(src_block));

    harness_mock_write("sd_read_block(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(sd_read_block)(struct sd_driver_t *self_p,
                                                   void *dst_p,
                                                   uint32_t src_block)
{
    ssize_t res;

    harness_mock_read("sd_read_block(): return (dst_p)",
                      dst_p,
                      -1);

    harness_mock_assert("sd_read_block(src_block)",
                        &src_block);

    harness_mock_read("sd_read_block(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sd_write_block(uint32_t dst_block,
                              const void *src_p,
                              ssize_t res)
{
    harness_mock_write("sd_write_block(dst_block)",
                       &dst_block,
                       sizeof(dst_block));

    harness_mock_write("sd_write_block(src_p)",
                       src_p,
                       sizeof(src_p));

    harness_mock_write("sd_write_block(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(sd_write_block)(struct sd_driver_t *self_p,
                                                    uint32_t dst_block,
                                                    const void *src_p)
{
    ssize_t res;

    harness_mock_assert("sd_write_block(dst_block)",
                        &dst_block);

    harness_mock_assert("sd_write_block(src_p)",
                        src_p);

    harness_mock_read("sd_write_block(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
