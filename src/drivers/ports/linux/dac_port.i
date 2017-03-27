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

static int dac_port_module_init(void)
{
    return (0);
}

static int dac_port_init(struct dac_driver_t *self_p,
                         struct dac_device_t *dev_p,
                         struct pin_device_t *pin0_dev_p,
                         struct pin_device_t *pin1_dev_p,
                         long sampling_rate)
{
    return (0);
}

static int dac_port_async_convert(struct dac_driver_t *self_p,
                                  uint32_t *samples_p,
                                  size_t length)
{
    return (0);
}

static int dac_port_async_wait(struct dac_driver_t *self_p)
{
    return (0);
}

static int dac_port_convert(struct dac_driver_t *self_p,
                            uint32_t *samples_p,
                            size_t length)
{
    return (0);
}
