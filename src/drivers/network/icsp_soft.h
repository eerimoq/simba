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

#ifndef __DRIVERS_ICSP_SOFT_H__
#define __DRIVERS_ICSP_SOFT_H__

#include "simba.h"

struct icsp_soft_driver_t {
    struct pin_device_t *pgec_p;
    struct pin_device_t *pged_p;
    struct pin_device_t *mclrn_p;
};

/**
 * Initialize ICSP soft module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] pgec_p Clock.
 * @param[in] pged_p Data.
 * @param[in] mclrn_p Master control (reset).
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_init(struct icsp_soft_driver_t *self_p,
                   struct pin_device_t *pgec_p,
                   struct pin_device_t *pged_p,
                   struct pin_device_t *mclrn_p);

/**
 * Start given ICSP soft driver. Configures all pins and enters the
 * idle state in the ICSP TAP controller state machine.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_start(struct icsp_soft_driver_t *self_p);

/**
 * Stop given ICSP soft driver. Enters the reset state in the ICSP TAP
 * controller state machine and then configures all pins as inputs.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_stop(struct icsp_soft_driver_t *self_p);

/**
 * Reset the ICSP device and then enter the idle state.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_reset(struct icsp_soft_driver_t *self_p);

/**
 * Write given instruction to given ICSP device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Instruction to write.
 * @param[in] size Number of bits to write.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_instruction_write(struct icsp_soft_driver_t *self_p,
                                const void *buf_p,
                                size_t number_of_bits);

/**
 * Simultaniuos data read/write operation from/to given ICSP device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] rxbuf_p Buffer to read into.
 * @param[in] txbuf_p Buffer to write.
 * @param[in] size Number of bits to transfer.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_data_transfer(struct icsp_soft_driver_t *self_p,
                            void *rxbuf_p,
                            const void *txbuf_p,
                            size_t number_of_bits);

/**
 * Read data from given ICSP device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bits to read.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_data_read(struct icsp_soft_driver_t *self_p,
                        void *buf_p,
                        size_t number_of_bits);

/**
 * Write given data to the ICSP device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bits to write.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_data_write(struct icsp_soft_driver_t *self_p,
                         const void *buf_p,
                         size_t number_of_bits);

/**
 * Simultaniuos fast data read/write operation from/to given ICSP
 * device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] rxbuf_p Buffer to read into.
 * @param[in] txbuf_p Buffer to write.
 * @param[in] size Number of bits to transfer.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_fast_data_transfer(struct icsp_soft_driver_t *self_p,
                                 void *rxbuf_p,
                                 const void *txbuf_p,
                                 size_t number_of_bits);

/**
 * Fast read data from given ICSP device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] data_p Read data.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_fast_data_read(struct icsp_soft_driver_t *self_p,
                             uint32_t *data_p);

/**
 * Fast write given data to the ICSP device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] data Dat ato write.
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_fast_data_write(struct icsp_soft_driver_t *self_p,
                              uint32_t data);

/**
 * Make given transition in the ICSP TAP controller state machine.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] transition Transition to take in the state machine
 *                       (one(1) or zero(0)).
 *
 * @return zero(0) or negative error code.
 */
int icsp_soft_make_transition(struct icsp_soft_driver_t *self_p,
                              int transition);

#endif
