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

#if CONFIG_USB_HOST == 1

/* Only one device supported for now. */
#define DEVICE_ADDRESS    1

struct module_t {
    int initialized;
    /* A list of all drivers. */
    struct usb_host_driver_t *drivers_p;
    struct usb_host_device_driver_t *device_drivers_p;
#if CONFIG_FS_CMD_USB_HOST_LIST == 1
    struct fs_command_t cmd_list;
#endif
};

static int device_enumerate(struct usb_host_device_t *device_p);

#include "usb_host_port.i"

static struct module_t module;

#if CONFIG_FS_CMD_USB_HOST_LIST == 1

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void * arg_p,
                       void *call_arg_p)
{
    int i;
    int verbose = 0;
    struct usb_host_driver_t *self_p;
    struct usb_host_device_t *device_p;

    if (argc == 2) {
        if (strcmp("-v", argv[1]) == 0) {
            verbose = 1;
        }
    }

    std_fprintf(out_p, OSTR("BUS  ADDRESS   VID   PID  DESCRIPTION\r\n"));

    self_p = module.drivers_p;

    while (self_p != NULL) {
        device_p = &self_p->devices_p[0];

        for (i = 0; i < self_p->length; i++, device_p++) {
            if (device_p->state != USB_HOST_DEVICE_STATE_ATTACHED) {
                continue;
            }

            std_fprintf(out_p,
                        OSTR("%3d %8d  %04x  %04x  %s\r\n"),
                        device_p->self_p->id,
                        device_p->address,
                        device_p->vid,
                        device_p->pid,
                        device_p->description_p);

            if (verbose == 1) {
                usb_format_descriptors(out_p,
                                       device_p->descriptors.buf,
                                       device_p->descriptors.size);
            }
        }

        self_p = self_p->next_p;
    }

    return (0);
}

#endif

/**
 * Set the device address.
 */
static int device_set_address(struct usb_host_device_t *device_p,
                              uint16_t address)
{
    struct usb_setup_t setup;

    std_printf(FSTR("Setting device address.\r\n"));

    /* Initiate the setup. */
    setup.request_type = (REQUEST_TYPE_DATA_DIRECTION_HOST_TO_DEVICE);
    setup.request = REQUEST_SET_ADDRESS;
    setup.u.set_address.device_address = address;
    setup.u.set_address.zero = 0;
    setup.length = 0;

    /* Write the setup to the hardware and read the response into
     * dst_p. */
    if (usb_host_device_control_transfer(device_p, &setup, NULL, 0) != 0) {
        std_printf(FSTR("Unable to set device address.\r\n"));

        return (-1);
    }

    device_p->address = address;

    return (usb_pipe_set_address(device_p->self_p,
                                 device_p->pipes[0],
                                 address));
}

/**
 * Set the device configuration.
 */
int usb_host_device_set_configuration(struct usb_host_device_t *device_p,
                                      uint8_t configuration)
{
    struct usb_setup_t setup;
    struct usb_pipe_t *pipe_p;
    int i, j;
    uint8_t *buf_p;
    size_t size;
    struct usb_descriptor_interface_t *int_p = NULL;
    struct usb_descriptor_endpoint_t *ep_p = NULL;
    int type;

    std_printf(FSTR("Setting device configuration %d.\r\n"), configuration);

    /* Initiate the setup. */
    setup.request_type = REQUEST_TYPE_DATA_DIRECTION_HOST_TO_DEVICE;
    setup.request = REQUEST_SET_CONFIGURATION;
    setup.u.set_configuration.configuration_value = configuration;
    setup.u.set_configuration.zero = 0;
    setup.length = 0;

    /* Write the setup to the hardware and read the response into
     * dst_p. */
    if (usb_host_device_control_transfer(device_p, &setup, NULL, 0) != 0) {
        std_printf(FSTR("Unable to set configration.\r\n"));

        return (-1);
    }

    device_p->current.configuration = configuration;
    device_p->current.descriptor.conf_p = NULL;

    /* Iterate over all interfaces in the configuration and allocate a
     * pipe for each endpoint. */
    buf_p = device_p->descriptors.buf;
    size = device_p->descriptors.size;

    for (i = 0;
         (int_p = usb_desc_get_interface(buf_p, size, 1, i)) != NULL;
         i++) {
        for (j = 1;
             (ep_p = usb_desc_get_endpoint(buf_p, size, 1, i, j)) != NULL;
             j++) {
            switch (ENDPOINT_ATTRIBUTES_TRANSFER_TYPE(ep_p->attributes)) {
            case ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_CONTROL:
                type = USB_PIPE_TYPE_CONTROL;
                break;
            case ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_ISOCHRONOUS:
                type = USB_PIPE_TYPE_ISOCHRONOUS;
                break;
            case ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK:
                type = USB_PIPE_TYPE_BULK;
                break;
            case ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_INTERRUPT:
                type = USB_PIPE_TYPE_INTERRUPT;
                break;
            default:
                return (-1);
            }

            pipe_p = usb_pipe_alloc(device_p->self_p,
                                    type,
                                    (ep_p->endpoint_address & 0x7f),
                                    device_p->address,
                                    ep_p->interval);

            if (pipe_p == NULL) {
                std_printf(FSTR("Pipe allocation failed.\r\n"));

                return (-1);
            }
        }
    }

    return (0);
}

/**
 * Get the device descriptors from the device.
 */
static ssize_t device_get_descriptor_device(struct usb_host_device_t *device_p,
                                            struct usb_descriptor_device_t *desc_p)
{
    struct usb_setup_t setup;

    /* Initiate the setup datastructure. */
    setup.request_type = REQUEST_TYPE_DATA_DIRECTION_DEVICE_TO_HOST;
    setup.request = REQUEST_GET_DESCRIPTOR;
    setup.u.get_descriptor.descriptor_index = 0;
    setup.u.get_descriptor.descriptor_type = DESCRIPTOR_TYPE_DEVICE;
    setup.u.get_descriptor.language_id = 0;
    setup.length = 8; /* Only read 8 bytes in the first read. */

    if (usb_host_device_control_transfer(device_p, &setup, desc_p, setup.length)
        != setup.length) {
        std_printf(FSTR("failed to read first 8 bytes of the device descriptor.\r\n"));

        return (-1);
    }

    /* Set the maximum packet size the device supports. */
    device_p->max_packet_size = desc_p->max_packet_size_0;

    std_printf(FSTR("max packet size = %d\r\n"), device_p->max_packet_size);

    /* Read the whole descriptor. */
    setup.length = sizeof(*desc_p);

    if (usb_host_device_control_transfer(device_p, &setup, desc_p, setup.length)
        != setup.length) {
        std_printf(FSTR("failed to read the device descriptor.\r\n"));

        return (-1);
    }

    device_p->current.descriptor.dev_p = desc_p;
    device_p->vid = desc_p->id_vendor;
    device_p->pid = desc_p->id_product;

    return (0);
}

/**
 * Get a configuration descriptors from the device.
 */
static ssize_t device_get_descriptor_configuration(struct usb_host_device_t *device_p,
                                                   uint8_t index,
                                                   uint8_t *dst_p,
                                                   size_t size)
{
    struct usb_setup_t setup;
    struct usb_descriptor_configuration_t *desc_p;

    desc_p = (struct usb_descriptor_configuration_t *)dst_p;

    std_printf(FSTR("Getting device configuration.\r\n"));

    /* Initiate the setup. */
    setup.request_type = REQUEST_TYPE_DATA_DIRECTION_DEVICE_TO_HOST;
    setup.request = REQUEST_GET_DESCRIPTOR;
    setup.u.get_descriptor.descriptor_index = index;
    setup.u.get_descriptor.descriptor_type = DESCRIPTOR_TYPE_CONFIGURATION;
    setup.u.get_descriptor.language_id = 0;
    setup.length = size;

    /* Write the setup to the hardware and read the response into */
    /* dst_p. */
    if (usb_host_device_control_transfer(device_p,
                                         &setup,
                                         desc_p,
                                         setup.length) != setup.length) {
        std_printf(FSTR("Unable to read the configuration descriptor from the device.\r\n"));

        return (-1);
    }

    return (setup.length);
}

/**
 * Get all descriptors from the device.
 */
static ssize_t device_get_descriptors(struct usb_host_device_t *device_p)
{
    struct usb_descriptor_configuration_t *desc_p;
    uint8_t *dst_p;

    dst_p = device_p->descriptors.buf;
    dst_p += device_p->descriptors.size;

    /* Get the configuration descriptor at index 0. */
    if (device_get_descriptor_configuration(device_p,
                                            0,
                                            dst_p,
                                            sizeof(struct usb_descriptor_configuration_t))
        != sizeof(struct usb_descriptor_configuration_t)) {
        return (-1);
    }

    desc_p = (struct usb_descriptor_configuration_t *)dst_p;

    if (desc_p->total_length > sizeof(struct usb_descriptor_configuration_t)) {
        if (device_get_descriptor_configuration(device_p,
                                                0,
                                                dst_p,
                                                desc_p->total_length)
            != desc_p->total_length) {
            return (-1);
        }
    }

    device_p->descriptors.size += desc_p->total_length;

    return (0);
}

static struct usb_host_device_driver_t *
find_device_driver(struct usb_host_device_t *device_p)
{
    struct usb_host_device_driver_t *driver_p;

    driver_p = module.device_drivers_p;

    while (driver_p != NULL) {
        if (driver_p->supports(device_p)) {
            return (driver_p);
        }

        driver_p = driver_p->next_p;
    }

    return (NULL);
}

/**
 * Enumerate a detected device. After the enumeration the device has
 * been configured and is ready to use by the application.
 */
static int device_enumerate(struct usb_host_device_t *device_p)
{
    struct usb_descriptor_device_t *device_desc_p;
    struct usb_host_device_driver_t *driver_p;
    struct usb_pipe_t *pipe_p = NULL;
    union usb_message_t message;

    std_printf(FSTR("Starting the USB enumeration process for a device.\r\n"));

    /* Set the initial maximum packet size.*/
    device_p->max_packet_size = 8;

    /* Reset the device. */
    if (device_reset(device_p) != 0) {
        return (-1);
    }

    /* Allocate a control pipe. */
    pipe_p = usb_pipe_alloc(device_p->self_p,
                            USB_PIPE_TYPE_CONTROL,
                            0,
                            0,
                            0);

    if (pipe_p == NULL) {
        goto err;
    }

    device_p->pipes[0] = pipe_p;

    /* Get the device descriptor. */
    device_desc_p = (struct usb_descriptor_device_t *)(device_p->descriptors.buf);

    if (device_get_descriptor_device(device_p, device_desc_p) != 0) {
        goto err;
    }

    device_p->descriptors.size = sizeof(struct usb_descriptor_device_t);

    /* Assign the device an address. */
    if (device_set_address(device_p, DEVICE_ADDRESS) != 0) {
        std_printf(FSTR("failed to set device address\r\n"));
        goto err;
    }

    /* Get various descriptors from the device. */
    if (device_get_descriptors(device_p) != 0) {
        std_printf(FSTR("failed to get device descriptors\r\n"));
        goto err;
    }

    driver_p = find_device_driver(device_p);

    if (driver_p != NULL) {
        /* Driver specific enumeration. */
        if (driver_p->enumerate(device_p) != 0) {
            goto err;
        }
    } else {
        std_printf(FSTR("Unsupported USB device.\r\n"));
    }

    device_p->state = USB_HOST_DEVICE_STATE_ATTACHED;
    device_p->description_p = "";

    std_printf(FSTR("Enumeration done.\r\n"));

    usb_format_descriptors(sys_get_stdout(),
                           device_p->descriptors.buf,
                           device_p->descriptors.size);

    /* Notify the controller. */
    message.add.header.type = USB_MESSAGE_TYPE_ADD;
    message.add.device = device_p->id;

    queue_write(&device_p->self_p->control,
                &message,
                sizeof(message));

    return (0);

 err:
    if (pipe_p != NULL) {
        usb_pipe_free(device_p->self_p, pipe_p);
    }

    return (-1);
}

int usb_host_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_USB_HOST_LIST == 1

    fs_command_init(&module.cmd_list,
                    CSTR("/drivers/usb_host/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

    return (0);
}

int usb_host_init(struct usb_host_driver_t *self_p,
                  struct usb_device_t *dev_p,
                  struct usb_host_device_t *devices_p,
                  size_t length)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);
    ASSERTN(devices_p != NULL, EINVAL);
    ASSERTN(length > 0, EINVAL);

    int i;

    self_p->dev_p =  dev_p;
    self_p->length = length;
    self_p->devices_p = devices_p;

    queue_init(&self_p->control, NULL, 0);

    for (i = 0; i < length; i++) {
        self_p->devices_p[i].id = i;
        self_p->devices_p[i].state = USB_HOST_DEVICE_STATE_NONE;
        self_p->devices_p[i].self_p = self_p;
    }

    return (usb_host_port_init(self_p, dev_p));
}

int usb_host_start(struct usb_host_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    if (usb_host_port_start(self_p) != 0) {
        return (-1);
    }

    /* Add the driver to the list. */
    self_p->next_p = module.drivers_p;
    module.drivers_p = self_p;

    return (0);
}

int usb_host_stop(struct usb_host_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (usb_host_port_stop(self_p));
}

int usb_host_driver_add(struct usb_host_driver_t *self_p,
                        struct usb_host_device_driver_t *driver_p,
                        void *arg_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(driver_p != NULL, EINVAL);

    driver_p->next_p = module.device_drivers_p;
    module.device_drivers_p = driver_p;

    return (0);
}

struct usb_host_device_t *
usb_host_device_open(struct usb_host_driver_t *self_p,
                     int device)
{
    ASSERTNRN(self_p != NULL, EINVAL);

    struct usb_host_device_t *device_p;

    if (device > self_p->length - 1) {
        return (NULL);
    }

    device_p = &self_p->devices_p[device];

    if (device_p->state != USB_HOST_DEVICE_STATE_ATTACHED) {
        return (NULL);
    }

    return (device_p);
}

int usb_host_device_close(struct usb_host_driver_t *self_p,
                          int device)
{
    ASSERTN(self_p != NULL, EINVAL);

    if (device > self_p->length - 1) {
        return (-1);
    }

    return (0);
}

ssize_t usb_host_device_read(struct usb_host_device_t *device_p,
                             int endpoint,
                             void *buf_p,
                             size_t size)
{
    ASSERTN(device_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (usb_host_port_device_read(device_p, endpoint, buf_p, size));
}

ssize_t usb_host_device_write(struct usb_host_device_t *device_p,
                              int endpoint,
                              const void *buf_p,
                              size_t size)
{
    ASSERTN(device_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (usb_host_port_device_write(device_p, endpoint, buf_p, size));
}

ssize_t usb_host_device_control_transfer(struct usb_host_device_t *device_p,
                                         struct usb_setup_t *setup_p,
                                         void *buf_p,
                                         size_t size)
{
    ASSERTN(device_p != NULL, EINVAL);
    ASSERTN(setup_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    std_printf(FSTR("Starting a control transfer.\r\n"));

    if (usb_host_port_device_write_setup(device_p, setup_p) != sizeof(*setup_p)) {
        std_printf(FSTR("Failed to write SETUP.\r\n"));

        return (-1);
    }

    /* Data and status stages. */
    if (setup_p->request_type & REQUEST_TYPE_DATA_DIRECTION_DEVICE_TO_HOST) {
        if (size > 0) {
            /* Read data from the device. */
            if (usb_host_port_device_read(device_p, 0, buf_p, size) != size) {
                return (-1);
            }
        }

        if (usb_host_port_device_write(device_p, 0, NULL, 0) != 0) {
            return (-1);
        }
    } else {
        if (size > 0) {
            /* Write data to the device. */
            if (usb_host_port_device_write(device_p, 0, buf_p, size) != size) {
                return (-1);
            }
        }

        if (usb_host_port_device_read(device_p, 0, NULL, 0) != 0) {
            return (-1);
        }
    }

    return (size);
}

#endif
