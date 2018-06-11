/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

#ifndef __DRIVERS_PCINT_H__
#define __DRIVERS_PCINT_H__

#include "simba.h"
#include "pcint_port.h"

/**
 * Trigger an interrupt on both rising and falling edges.
 */
#define PCINT_TRIGGER_BOTH_EDGES   PCINT_PORT_TRIGGER_BOTH_EDGES

/**
 * Trigger an interrupt on falling edges.
 */
#define PCINT_TRIGGER_FALLING_EDGE PCINT_PORT_TRIGGER_FALLING_EDGE

/**
 * Trigger an interrupt on rising edges.
 */
#define PCINT_TRIGGER_RISING_EDGE  PCINT_PORT_TRIGGER_RISING_EDGE

extern struct pcint_device_t pcint_device[PCINT_DEVICE_MAX];

/**
 * Initialize the external change interrupt module. This function must
 * be called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int pcint_module_init(void);

/**
 * Initialize given change interrupt driver object.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] trigger One of ``PCINT_TRIGGER_BOTH_EDGES``,
 *                    ``PCINT_TRIGGER_FALLING_EDGE`` or
 *                    ``PCINT_TRIGGER_RISING_EDGE``.
 * @param[in] on_interrupt Function callback called when an interrupt
 *                         occurs.
 * @param[in] arg_p Function callback argument.
 *
 * @return zero(0) or negative error code.
 */
int pcint_init(struct pcint_driver_t *self_p,
               struct pcint_device_t *dev_p,
               int trigger,
               void (*on_interrupt)(void *arg_p),
               void *arg_p);

/**
 * Starts the pin change interrupt device using given driver object.
 *
 * Enables interrupts for given pin change interrupt driver.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int pcint_start(struct pcint_driver_t *self_p);

/**
 * Stops the pin change interrupt device referenced by given driver
 * object.
 *
 * Disables interrupts for given pin change interrupt driver.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int pcint_stop(struct pcint_driver_t *self_p);

#endif
