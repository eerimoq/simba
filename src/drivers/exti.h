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

#ifndef __DRIVERS_EXTI_H__
#define __DRIVERS_EXTI_H__

#include "simba.h"

struct exti_driver_t;

#include "exti_port.h"

/**
 * Trigger an interrupt on both rising and falling edges.
 */
#define EXTI_TRIGGER_BOTH_EDGES   EXTI_PORT_TRIGGER_BOTH_EDGES   

/**
 * Trigger an interrupt on falling edges.
 */
#define EXTI_TRIGGER_FALLING_EDGE EXTI_PORT_TRIGGER_FALLING_EDGE 

/**
 * Trigger an interrupt on both rising edges.
 */
#define EXTI_TRIGGER_RISING_EDGE  EXTI_PORT_TRIGGER_RISING_EDGE  

extern struct exti_device_t exti_device[EXTI_DEVICE_MAX];

/**
 * Initialize the external interrupt (EXTI) module. This function must
 * be called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int exti_module_init(void);

/**
 * Initialize given driver object.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] trigger One of ``EXTI_TRIGGER_BOTH_EDGES``,
 *                    ``EXTI_TRIGGER_FALLING_EDGE`` or
 *                    ``EXTI_TRIGGER_RISING_EDGE``.
 * @param[in] on_interrupt Function callback called when an interrupt
 *                         occurs.
 * @param[in] arg_p Fucntion callback argument.
 *
 * @return zero(0) or negative error code.
 */
int exti_init(struct exti_driver_t *self_p,
              struct exti_device_t *dev_p,
              int trigger,
              void (*on_interrupt)(void *arg_p),
              void *arg_p);

/**
 * Starts the EXTI device using given driver object.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int exti_start(struct exti_driver_t *self_p);

/**
 * Stops the EXTI device referenced by given driver object.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int exti_stop(struct exti_driver_t *self_p);

/**
 * Clear the interrupt flag.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int exti_clear(struct exti_driver_t *self_p);

#endif
