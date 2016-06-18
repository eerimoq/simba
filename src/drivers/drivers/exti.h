/**
 * @file drivers/exti.h
 * @version 1.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_EXTI_H__
#define __DRIVERS_EXTI_H__

#include "simba.h"

struct exti_driver_t;

#include "exti_port.h"

#define EXTI_TRIGGER_BOTH_EDGES   EXTI_PORT_TRIGGER_BOTH_EDGES   
#define EXTI_TRIGGER_FALLING_EDGE EXTI_PORT_TRIGGER_FALLING_EDGE 
#define EXTI_TRIGGER_RISING_EDGE  EXTI_PORT_TRIGGER_RISING_EDGE  

extern struct exti_device_t exti_device[EXTI_DEVICE_MAX];

/**
 * Initialize the external interrupt (EXTI) module.
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
 * @param[in] on_interrupt Fucntion callback argument.
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
