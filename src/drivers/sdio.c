/**
 * @file sdio.c
 * @version 4.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

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
