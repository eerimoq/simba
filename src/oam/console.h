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

#ifndef __OAM_CONSOLE_H__
#define __OAM_CONSOLE_H__

#include "simba.h"

/**
 * Initialize the console module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int console_module_init(void);

/**
 * Initialize the console.
 *
 * @return zero(0) or negative error code.
 */
int console_init(void);

/**
 * Start the console.
 *
 * @return zero(0) or negative error code.
 */
int console_start(void);

/**
 * Stop the console.
 *
 * @return zero(0) or negative error code.
 */
int console_stop(void);

/**
 * Set the pointer to the input channel.
 *
 * @return zero(0) or negative error code.
 */
int console_set_input_channel(void *chan_p);

/**
 * Get the pointer to the input channel.
 *
 * @return Input channel or NULL.
 */
void *console_get_input_channel(void);

/**
 * Set the pointer to the output channel.
 *
 * @return zero(0) or negative error code.
 */
void *console_set_output_channel(void *chan_p);

/**
 * Get the pointer to the output channel.
 *
 * @return Output channel or NULL.
 */
void *console_get_output_channel(void);

#endif
