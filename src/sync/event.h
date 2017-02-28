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

#ifndef __SYNC_EVENT_H__
#define __SYNC_EVENT_H__

#include "simba.h"

/**
 * Event channel.
 */
struct event_t {
    struct chan_t base;
    uint32_t mask;                 /* Events that occured. */
    uint32_t reader_mask;          /* Events the reader are waiting for. */
};

/**
 * Initialize given event channel.
 *
 * @param[in] self_p Event channel to initialize.
 *
 * @return zero(0) or negative error code
 */
int event_init(struct event_t *self_p);

/**
 * Wait for an event to occur in given event mask. This function
 * blocks until at least one of the events in the event mask has been
 * set. When the function returns, given event mask has been
 * overwritten with the events that actually occured.
 *
 * @param[in] self_p Event channel object.
 * @param[in, out] buf_p The mask of events to wait for. When the
 *                       function returns the mask contains the events
 *                       that have occured.
 * @param[in] size Size to read (always sizeof(mask)).
 *
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_read(struct event_t *self_p,
                   void *buf_p,
                   size_t size);

/**
 * Write given event(s) to given event channel.
 *
 * @param[in] self_p Event channel object.
 * @param[in] buf_p The mask of events to write.
 * @param[in] size Must always be sizeof(mask).
 *
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_write(struct event_t *self_p,
                    const void *buf_p,
                    size_t size);

/**
 * Write given events to the event channel from isr or with the system
 * lock taken (see `sys_lock()`).
 *
 * @param[in] self_p Event channel object.
 * @param[in] buf_p The mask of events to write.
 * @param[in] size Must always be sizeof(mask).
 *
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_write_isr(struct event_t *self_p,
                        const void *buf_p,
                        size_t size);

/**
 * Checks if there are events active on the event channel.
 *
 * @param[in] self_p Event channel object.
 *
 * @return one(1) is at least one event is active, otherwise zero(0).
 */
ssize_t event_size(struct event_t *self_p);

#endif
