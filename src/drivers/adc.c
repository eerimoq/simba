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

#if CONFIG_ADC == 1

struct module_t {
    int initialized;
};

#include "adc_port.i"

static struct module_t module;

int adc_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    return (adc_port_module_init());
}

int adc_init(struct adc_driver_t *self_p,
             struct adc_device_t *dev_p,
             struct pin_device_t *pin_dev_p,
             int reference,
             long sampling_rate)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(adc_is_valid_device(dev_p), EINVAL);
    ASSERTN(pin_is_valid_device(pin_dev_p), EINVAL);
    ASSERTN(reference == ADC_REFERENCE_VCC, EINVAL);
    ASSERTN(sampling_rate > 0, EINVAL);

    return (adc_port_init(self_p,
                          dev_p,
                          pin_dev_p,
                          reference,
                          sampling_rate));
}

int adc_async_convert(struct adc_driver_t *self_p,
                      uint16_t *samples_p,
                      size_t length)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(samples_p != NULL, EINVAL);
    ASSERTN(length > 0, EINVAL);

    return (adc_port_async_convert(self_p, samples_p, length));
}

int adc_async_wait(struct adc_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (adc_port_async_wait(self_p));
}

int adc_convert(struct adc_driver_t *self_p,
                uint16_t *samples_p,
                size_t length)
{
    adc_port_async_convert(self_p, samples_p, length);
    adc_port_async_wait(self_p);

    return (0);
}

RAM_CODE int adc_convert_isr(struct adc_driver_t *self_p,
                             uint16_t *sample_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(sample_p != NULL, EINVAL);

    return (adc_port_convert_isr(self_p, sample_p));
}

int adc_is_valid_device(struct adc_device_t *dev_p)
{
    return ((dev_p >= &adc_device[0])
            && (dev_p < &adc_device[ADC_DEVICE_MAX]));
}

#endif
