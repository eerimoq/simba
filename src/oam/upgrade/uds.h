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
