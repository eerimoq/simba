/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#if CONFIG_SDIO == 1

int sdio_module_init(void)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

int sdio_init(struct sdio_driver_t *self_p,
              struct sdio_device_t *dev_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

int sdio_start(struct sdio_driver_t *self_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

int sdio_stop(struct sdio_driver_t *self_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}


int sdio_send_relative_addr(struct sdio_driver_t *self_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}


int sdio_io_send_op_cond(struct sdio_driver_t *self_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}


int sdio_select_deselect_card(struct sdio_driver_t *self_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}


int sdio_io_read_direct(struct sdio_driver_t *self_p,
                        void *dst_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}


int sdio_io_write_direct(struct sdio_driver_t *self_p,
                         const void *src_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}


ssize_t sdio_io_read_extended(struct sdio_driver_t *self_p,
                              int function_number,
                              int block_mode,
                              int op_code,
                              void *dst_p,
                              uint32_t src_address,
                              size_t size)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}


ssize_t sdio_io_write_extended(struct sdio_driver_t *self_p,
                               int function_number,
                               int block_mode,
                               int op_code,
                               uint32_t dst_address,
                               const void *src_p,
                               size_t size)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

#endif
