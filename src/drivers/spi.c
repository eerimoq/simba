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

#if CONFIG_SPI == 1

struct module_t {
    int initialized;
};

#include "spi_port.i"

static struct module_t module;

int spi_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    return (spi_port_module_init());
}

int spi_init(struct spi_driver_t *self_p,
             struct spi_device_t *dev_p,
             struct pin_device_t *ss_pin_p,
             int mode,
             int speed,
             int polarity,
             int phase)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);
    ASSERTN(ss_pin_p != NULL, EINVAL);

    self_p->dev_p = dev_p;
    self_p->mode = mode;
    self_p->speed = speed;
    self_p->polarity = polarity;
    self_p->phase = phase;

    return (spi_port_init(self_p,
                          dev_p,
                          ss_pin_p,
                          mode,
                          speed,
                          polarity,
                          phase));
}

int spi_start(struct spi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    spi_take_bus(self_p);
    sem_give(&self_p->dev_p->sem, 1);

    return (0);
}

int spi_stop(struct spi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    /* Stop SPI hardware with driver configuration. */
    if (self_p->dev_p->drv_p == self_p) {
        self_p->dev_p->drv_p = NULL;
        spi_port_stop(self_p);
    }

    return (0);
}

int spi_take_bus(struct spi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    sem_take(&self_p->dev_p->sem, NULL);

    /* Configure and start SPI hardware with driver configuration. */
    if (self_p->dev_p->drv_p != self_p) {
        self_p->dev_p->drv_p = self_p;
        spi_port_start(self_p);
    }

    return (0);
}

int spi_give_bus(struct spi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    sem_give(&self_p->dev_p->sem, 1);

    return (0);
}

int spi_select(struct spi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (pin_write(&self_p->ss, 0));
}

int spi_deselect(struct spi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (pin_write(&self_p->ss, 1));
}

ssize_t spi_transfer(struct spi_driver_t *self_p,
                     void *rxbuf_p,
                     const void *txbuf_p,
                     size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN((rxbuf_p != NULL) || (txbuf_p != NULL), EINVAL);
    ASSERTN(size > 0, EINVAL);

    return(spi_port_transfer(self_p, rxbuf_p, txbuf_p, size));
}

ssize_t spi_read(struct spi_driver_t *self_p,
                 void *rxbuf_p,
                 size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(rxbuf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (spi_transfer(self_p, rxbuf_p, NULL, size));
}

ssize_t spi_write(struct spi_driver_t *self_p,
                  const void *txbuf_p,
                  size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(txbuf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (spi_transfer(self_p, NULL, txbuf_p, size));
}

ssize_t spi_get(struct spi_driver_t *self_p,
                uint8_t *data_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(data_p != NULL, EINVAL);

    return (spi_read(self_p, data_p, 1));
}

ssize_t spi_put(struct spi_driver_t *self_p,
                uint8_t data)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (spi_write(self_p, &data, 1));
}

#endif
