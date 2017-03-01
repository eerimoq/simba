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

#ifndef __DRIVERS_SDIO_H__
#define __DRIVERS_SDIO_H__

#include "simba.h"

#define SDIO_IO_RW_EXTENDED_BLOCK_MODE_BYTE  0
#define SDIO_IO_RW_EXTENDED_BLOCK_MODE_BLOCK 1

#define SDIO_IO_RW_EXTENDED_OP_CODE_FIXED_ADDRESS        0
#define SDIO_IO_RW_EXTENDED_OP_CODE_INCREMENTING_ADDRESS 1

struct sdio_io_rw_extended_t {
    uint8_t rw_flag : 1;
    uint8_t function_number : 3;
    uint8_t block_mode : 1;
    uint8_t op_code : 1;
    uint8_t register_address_16_15 : 2;
    uint8_t register_address_14_7 : 8;
    uint8_t register_address_6_0 : 7;
    uint8_t byte_block_count_8 : 1;
    uint8_t byte_block_count_7_0 : 8;
};

#include "sdio_port.h"

extern struct sdio_device_t sdio_device[SDIO_DEVICE_MAX];

/**
 * Initialize the SDIO module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int sdio_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 *
 * @return zero(0) or negative error code.
 */
int sdio_init(struct sdio_driver_t *self_p,
              struct sdio_device_t *dev_p);

/**
 * Starts the SDIO device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int sdio_start(struct sdio_driver_t *self_p);

/**
 * Stops the SDIO device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int sdio_stop(struct sdio_driver_t *self_p);

/**
 * Send the `send relative address` command (CMD3) to the device and
 * optionally wait for the response.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int sdio_send_relative_addr(struct sdio_driver_t *self_p);

/**
 * Send the `io send operation condition` command (CMD5) to the device
 * and optionally wait for the response.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int sdio_io_send_op_cond(struct sdio_driver_t *self_p);

/**
 * Send the `select/deselect card` command (CMD7) to the device and
 * optionally wait for the response.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int sdio_select_deselect_card(struct sdio_driver_t *self_p);

/**
 * Execute the `input output read write direct` command (CMD52) as a
 * read operation with given parameters.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] dst_p Destination buffer.
 *
 * @return Number of bytes read or negative error code.
 */
int sdio_io_read_direct(struct sdio_driver_t *self_p,
                        void *dst_p);

/**
 * Execute the `input output read write direct` command (CMD52) as a
 * write operation with given parameters.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] src_p Source buffer.
 *
 * @return Number of bytes written or negative error code.
 */
int sdio_io_write_direct(struct sdio_driver_t *self_p,
                         const void *src_p);

/**
 * Execute the `input output read write extended` command (CMD53) as a
 * read operation with given parameters.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] function_number Function number.
 * @param[in] block_mode Block or byte mode.
 * @param[in] op_code Operation code.
 * @param[out] dst_p Destination buffer.
 * @param[in] src_address Source address.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t sdio_io_read_extended(struct sdio_driver_t *self_p,
                              int function_number,
                              int block_mode,
                              int op_code,
                              void *dst_p,
                              uint32_t src_address,
                              size_t size);

/**
 * Execute the `input output read write extended` command (CMD53) as a
 * write operation with given parameters.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] function_number Function number.
 * @param[in] block_mode Block or byte mode.
 * @param[in] op_code Operation code.
 * @param[in] dst_address Destination address.
 * @param[in] src_p Source buffer.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t sdio_io_write_extended(struct sdio_driver_t *self_p,
                               int function_number,
                               int block_mode,
                               int op_code,
                               uint32_t dst_address,
                               const void *src_p,
                               size_t size);

#endif
