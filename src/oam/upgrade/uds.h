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

#ifndef __OAM_UPGRADE_UDS_H__
#define __OAM_UPGRADE_UDS_H__

#include "simba.h"

/* States. */
#define UDS_STATE_IDLE        1
#define UDS_STATE_SWDL        2

struct upgrade_uds_t {
    int state;
    void *chin_p;
    void *chout_p;
    struct {
        uint8_t next_block_sequence_counter;
    } swdl;
};

/**
 * Initialize the UDS object.
 *
 * @param[in,out] self_p UDS object.
 * @param[in] chin_p Input channel.
 * @param[in] chout_p Output channel.
 *
 * @returns zero(0) or negative error code.
 */
int upgrade_uds_init(struct upgrade_uds_t *self_p,
                     void *chin_p,
                     void *chout_p);

/**
 * Handle a service.
 *
 * @param[in] self_p UDS object.
 *
 * @returns zero(0) or negative error code.
 */
int upgrade_uds_handle_service(struct upgrade_uds_t *self_p);

/**
 * Run the main loop of the UDS object.
 *
 * @param[in] self_p UDS object.
 *
 * @returns Never returns.
 */
void upgrade_uds_main(struct upgrade_uds_t *self_p);

#endif
