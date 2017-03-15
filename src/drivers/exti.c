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

#include "simba.h"

#if CONFIG_EXTI == 1

struct module_t {
    int initialized;
};

#include "exti_port.i"

static struct module_t module;

int exti_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    return (exti_port_module_init());
}

int exti_init(struct exti_driver_t *self_p,
              struct exti_device_t *dev_p,
              int trigger,
              void (*on_interrupt)(void *arg_p),
              void *arg_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);
    ASSERTN(on_interrupt != NULL, EINVAL);

    self_p->dev_p = dev_p;
    self_p->trigger = trigger;
    self_p->on_interrupt = on_interrupt;
    self_p->arg_p = arg_p;

    return (0);
}

int exti_start(struct exti_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (exti_port_start(self_p));
}

int exti_stop(struct exti_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (exti_port_stop(self_p));
}

int exti_clear(struct exti_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (exti_port_clear(self_p));
}

#endif
