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
