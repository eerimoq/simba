/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
