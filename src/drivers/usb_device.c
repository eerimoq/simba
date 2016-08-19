/**
 * @file usb_device.c
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
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

#include "simba.h"

struct module_t {
    int initialized;
    struct usb_device_driver_t *driver_p;
#if CONFIG_FS_CMD_USB_DEVICE_LIST == 1
    struct fs_command_t cmd_list;
#endif
};

#include "usb_device_port.i"

static struct module_t module;

#if CONFIG_FS_CMD_USB_DEVICE_LIST == 1

static int cmd_list_cb(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    int i;

    if (module.driver_p == NULL) {
        return (0);
    }

    for (i = 0; i < module.driver_p->drivers_max; i++) {
        module.driver_p->drivers_pp[i]->print(module.driver_p->drivers_pp[i],
                                              out_p);
    }

    return (0);
}

#endif

int usb_device_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_USB_DEVICE_LIST == 1

    fs_command_init(&cmd_list,
                    FSTR("/drivers/usb_device/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&cmd_list);

#endif

    return (usb_device_port_module_init());
}

int usb_device_init(struct usb_device_driver_t *self_p,
                    struct usb_device_t *dev_p,
                    struct usb_device_driver_base_t **drivers_pp,
                    int drivers_max,
                    FAR const union usb_descriptor_t **descriptors_pp)
{
    ASSERTN(self_p != NULL, INVAL);
    ASSERTN(dev_p != NULL, INVAL);
    ASSERTN(drivers_pp != NULL, INVAL);
    ASSERTN(drivers_max > 0, INVAL);
    ASSERTN(descriptors_pp != NULL, INVAL);

    self_p->dev_p = dev_p;
    self_p->configuration = -1;
    self_p->drivers_pp = drivers_pp;
    self_p->drivers_max = drivers_max;
    self_p->descriptors_pp = descriptors_pp;

    return (usb_device_port_init(self_p, dev_p));
}

int usb_device_start(struct usb_device_driver_t *self_p)
{
    ASSERTN(self_p != NULL, INVAL);

    module.driver_p = self_p;

    return (usb_device_port_start(self_p));
}

int usb_device_stop(struct usb_device_driver_t *self_p)
{
    ASSERTN(self_p != NULL, INVAL);

    return (usb_device_port_stop(self_p));
}

ssize_t usb_device_write(struct usb_device_driver_t *self_p,
                         int endpoint,
                         const void *buf_p,
                         size_t size)
{
    ASSERTN(self_p != NULL, INVAL);
    ASSERTN(endpoint > 0, INVAL);
    ASSERTN(buf_p != NULL, INVAL);
    ASSERTN(size > 0, INVAL);

    return (usb_device_port_write(self_p, endpoint, buf_p, size));
}

ssize_t usb_device_read_isr(struct usb_device_driver_t *self_p,
                            int endpoint,
                            void *buf_p,
                            size_t size)
{
    ASSERTN(self_p != NULL, INVAL);
    ASSERTN(endpoint > 0, INVAL);
    ASSERTN(buf_p != NULL, INVAL);
    ASSERTN(size > 0, INVAL);

    return (usb_device_port_read_isr(self_p, endpoint, buf_p, size));
}

ssize_t usb_device_write_isr(struct usb_device_driver_t *self_p,
                             int endpoint,
                             const void *buf_p,
                             size_t size)
{
    ASSERTN(self_p != NULL, INVAL);
    ASSERTN(endpoint > 0, INVAL);
    ASSERTN(buf_p != NULL, INVAL);
    ASSERTN(size > 0, INVAL);

    return (usb_device_port_write_isr(self_p, endpoint, buf_p, size));
}
