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

struct soam_t {
    int is_printf;
    uint8_t transaction_id;
    struct {
        uint8_t *buf_p;
        size_t size;
        ssize_t pos;
        struct sem_t sem;
        void *chout_p;
        uint8_t packet_index;
    } tx;
    struct chan_t stdout_chan;
    struct chan_t log_chan;
    struct chan_t command_chan;
};

/**
 * Initialize the soam module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int soam_module_init(void);

/**
 * Initialize given soam object.
 *
 * @param[in] self_p Object to initialize.
 * @param[in] buf_p Transmission buffer.
 * @param[in] size Transmission buffer size.
 * @param[in] chout_p Soam packets are written to this channel.
 *
 * @return zero(0) or negative error code.
 */
int soam_init(struct soam_t *self_p,
              void *buf_p,
              size_t size,
              void *chout_p);

/**
 * Process given soam packet.
 *
 * @param[in] self_p Soam object.
 * @param[in] buf_p Buffer to input.
 * @param[in] size Size to input in bytes.
 *
 * @return zero(0) or negative error code.
 */
int soam_input(struct soam_t *self_p,
               uint8_t *buf_p,
               size_t size);

/**
 * Start outputting a soam packet of given type.
 *
 * @param[in] self_p Soam object.
 * @param[in] type Packet type.
 *
 * @return zero(0) or negative error code.
 */
ssize_t soam_write_begin(struct soam_t *self_p,
                         int type);

/**
 * Add given chunk of data to current packet.
 *
 * @param[in] self_p Soam object.
 * @param[in] buf_p Buffer to output.
 * @param[in] size Size to output in bytes.
 *
 * @return zero(0) or negative error code.
 */
ssize_t soam_write_chunk(struct soam_t *self_p,
                         const void *buf_p,
                         size_t size);

/**
 * Finalize current packet and transmit it.
 *
 * @param[in] self_p Soam object.
 *
 * @return zero(0) or negative error code.
 */
ssize_t soam_write_end(struct soam_t *self_p);

/**
 * Create and transmit a soam packet of given type and data.
 *
 * @param[in] self_p Soam object.
 * @param[in] type Packet type.
 * @param[in] buf_p Buffer to output.
 * @param[in] size Size to output in bytes.
 *
 * @return zero(0) or negative error code.
 */
ssize_t soam_write(struct soam_t *self_p,
                   int type,
                   const void *buf_p,
                   size_t size);

/**
 * Get the log input channel. This channel can be set as output
 * channel of the log module with
 * `log_set_default_handler_output_channel()`.
 *
 * @param[in] self_p Soam object.
 *
 * @return Log input channel.
 */
void *soam_get_log_input_channel(struct soam_t *self_p);

/**
 * Get the standard output input channel. This channel can be set as
 * standard output channel of the sys module with `sys_set_stdout()`.
 *
 * @param[in] self_p Soam object.
 *
 * @return Standard output input channel.
 */
void *soam_get_stdout_input_channel(struct soam_t *self_p);
