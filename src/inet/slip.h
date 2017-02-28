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

struct slip_t {
    struct {
        int is_escaped;
        uint8_t *buf_p;
        size_t size;
        size_t pos;
    } rx;
    struct chan_t chout;
    void *chout_p;
};

/**
 * Initialize given slip object.
 *
 * @param[in] self_p Object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int slip_init(struct slip_t *self_p,
              void *buf_p,
              size_t size,
              void *chout_p);

/**
 * Input a data byte into the slip parser.
 *
 * @param[in] self_p Slip object.
 * @param[in] data Data byte to input.
 *
 * @return Once a complete SLIP frame has been received the size of
 *         the frame is returned. Meanwhile, zero(0) or negative error
 *         code is returned.
 */
ssize_t slip_input(struct slip_t *self_p,
                   uint8_t data);

/**
 * Get the output channel for given slip object.
 *
 * @param[in] self_p Slip object.
 *
 * @return Output channel.
 */
void *slip_get_output_channel(struct slip_t *self_p);
