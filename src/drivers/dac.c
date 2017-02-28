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

#if CONFIG_DAC == 1

struct module_t {
    int initialized;
};

#include "dac_port.i"

static struct module_t module;

int dac_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    return (dac_port_module_init());
}

int dac_init(struct dac_driver_t *self_p,
             struct dac_device_t *dev_p,
             struct pin_device_t *pin0_dev_p,
             struct pin_device_t *pin1_dev_p,
             int sampling_rate)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);
    ASSERTN(sampling_rate > 0, EINVAL);
    ASSERTN(pin0_dev_p != NULL, EINVAL);
    ASSERTN(pin0_dev_p != pin1_dev_p, EINVAL);

    return (dac_port_init(self_p,
                          dev_p,
                          pin0_dev_p,
                          pin1_dev_p,
                          sampling_rate));
}

int dac_async_convert(struct dac_driver_t *self_p,
                      void *samples_p,
                      size_t length)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(samples_p != NULL, EINVAL);
    ASSERTN(length > 0, EINVAL);

    return (dac_port_async_convert(self_p, samples_p, length));
}

int dac_async_wait(struct dac_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (dac_port_async_wait(self_p));
}

int dac_convert(struct dac_driver_t *self_p,
                void *samples_p,
                size_t length)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(samples_p != NULL, EINVAL);
    ASSERTN(length > 0, EINVAL);

    return (dac_port_convert(self_p, samples_p, length));
}

#endif
