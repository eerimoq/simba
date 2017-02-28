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
