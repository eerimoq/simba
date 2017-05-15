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

#if CONFIG_FLASH == 1

struct module_t {
    int initialized;
};

#include "flash_port.i"

static struct module_t module;

int flash_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    return (flash_port_module_init());
}

int flash_init(struct flash_driver_t *self_p,
               struct flash_device_t *dev_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    self_p->dev_p = dev_p;

    return (0);
}

ssize_t flash_read(struct flash_driver_t *self_p,
                   void *dst_p,
                   size_t src,
                   size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    ssize_t res;

#if CONFIG_FLASH_DEVICE_SEMAPHORE == 1
    sem_take(&self_p->dev_p->sem, NULL);
#endif

    res = flash_port_read(self_p, dst_p, src, size);

#if CONFIG_FLASH_DEVICE_SEMAPHORE == 1
    sem_give(&self_p->dev_p->sem, 1);
#endif

    return (res);
}

ssize_t flash_write(struct flash_driver_t *self_p,
                    size_t dst,
                    const void *src_p,
                    size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(src_p != NULL , EINVAL);
    ASSERTN(size > 0, EINVAL);

    ssize_t res;

#if CONFIG_FLASH_DEVICE_SEMAPHORE == 1
    sem_take(&self_p->dev_p->sem, NULL);
#endif

    res = flash_port_write(self_p, dst, src_p, size);

#if CONFIG_FLASH_DEVICE_SEMAPHORE == 1
    sem_give(&self_p->dev_p->sem, 1);
#endif

    return (res);
}

int flash_erase(struct flash_driver_t *self_p,
                size_t addr,
                size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    ssize_t res;

#if CONFIG_FLASH_DEVICE_SEMAPHORE == 1
    sem_take(&self_p->dev_p->sem, NULL);
#endif

    res = flash_port_erase(self_p, addr, size);

#if CONFIG_FLASH_DEVICE_SEMAPHORE == 1
    sem_give(&self_p->dev_p->sem, 1);
#endif

    return (res);
}

#endif
