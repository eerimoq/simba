/**
 * @file drivers/usb_device.h
 * @version 2.0.0
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

#ifndef __DRIVERS_USB_DEVICE_H__
#define __DRIVERS_USB_DEVICE_H__

#include "simba.h"

/* States of a device. */
#define USB_DEVICE_DEVICE_STATE_NONE          0
#define USB_DEVICE_DEVICE_STATE_ATTACHED      1

/* Pipe types. */
#define USB_PIPE_TYPE_CONTROL       0
#define USB_PIPE_TYPE_INTERRUPT     1
#define USB_PIPE_TYPE_ISOCHRONOUS   2
#define USB_PIPE_TYPE_BULK          3

/**
 * An USB device as seen by the device.
 */
struct usb_device_device_t {
    int id;
    int state;
    int address;                       /* Device address on the USB bus. */
    int vid;
    int pid;
    char *description_p;
    size_t max_packet_size;
    struct {
        uint8_t configuration;         /* Current configuration. */
        struct {
            struct usb_descriptor_device_t *dev_p;
            struct usb_descriptor_configuration_t *conf_p;
        } descriptor;
    } current;
    struct usb_device_driver_t *self_p;
    struct usb_pipe_t *pipes[32];
    /* A list of descriptors. */
    struct {
        size_t size;
        uint8_t buf[128];
    } descriptors;
};

/**
 * Used to find a device driver.
 */
struct usb_device_device_driver_t {
    int (*supports)(struct usb_device_device_t *);
    int (*enumerate)(struct usb_device_device_t *);
    struct usb_device_device_driver_t *next_p;
};

#include "usb_device_port.h"

/**
 * Initialize the USB device module.
 */
int usb_device_module_init(void);

/**
 * Initialize the USB device driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p USB device to use.
 * @param[in] devices_p An array of devices. One entry in this array
 *                      is allocated for each USB device that is
 *                      connected to the device.
 * @param[in] length Length of the devices array.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_init(struct usb_device_driver_t *self_p,
                    struct usb_device_t *dev_p);

/**
 * Start the USB device device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_start(struct usb_device_driver_t *self_p);

/**
 * Stop the USB device device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_stop(struct usb_device_driver_t *self_p);

/**
 * Add given class/vendor driver to the USB device driver.
 *
 * When a USB device is plugged in, its class and vendor information
 * is read by the device. Those values are used to find the device
 * driver for this particular device. If there is no driver, the
 * device cannot be configured and will not work.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] driver_p USB device driver to add.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_driver_add(struct usb_device_driver_t *self_p,
                          struct usb_device_device_driver_t *driver_p,
                          void *arg_p);

/**
 * Remove given class/vendor driver from the USB device driver.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] driver_p USB device driver to remove.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_driver_remove(struct usb_device_driver_t *self_p,
                             struct usb_device_device_driver_t *driver_p);

/**
 * Open given device in given driver. Open a device before reading and
 * writing data to it with usb_device_device_read() or
 * usb_device_device_write().
 *
 * @param[in] self_p Initialized driver.
 * @param[in] device Device to open.
 *
 * @return Opened device or NULL on failure.
 */
struct usb_device_device_t *
usb_device_device_open(struct usb_device_driver_t *self_p,
                       int device);

/**
 * Close given device in given driver.
 *
 * @param[in] self_p Initialized driver.
 * @param[in] device Device to close.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_device_close(struct usb_device_driver_t *self_p,
                            int device);

/**
 * Read data from given endpoint for given device.
 *
 * @param[in] device_p Device to read from.
 * @param[in] endpoint Endpoint to read data from.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t usb_device_device_read(struct usb_device_device_t *device_p,
                               int endpoint,
                               void *buf_p,
                               size_t size);

/**
 * Write data to given endpoint for given device.
 *
 * @param[in] device_p Device to write to.
 * @param[in] endpoint Endpoint to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t usb_device_device_write(struct usb_device_device_t *device_p,
                                int endpoint,
                                const void *buf_p,
                                size_t size);

/**
 * Perform a control transfer on endpoint zero(0).
 *
 * A control transfer can have up to three stages. First the setup
 * stage, then an optional data stage, and at last a status stage.
 *
 * @param[in] device_p Device to write to.
 * @param[in] setup_p Setup packet to write.
 * @param[in] buf_p Buffer to read/write. May be NULL if no data shall
 *                  be transferred.
 * @param[in] size Number of bytes to read/write.
 *
 * @return Number of bytes read/written or negative error code.
 */
ssize_t usb_device_device_control_transfer(struct usb_device_device_t *device_p,
                                           struct usb_setup_t *setup_p,
                                           void *buf_p,
                                           size_t size);

/**
 * Set configuration for given device.
 *
 * @param[in] device_p Device to use.
 * @param[in] configuration Configuration to set.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_device_set_configuration(struct usb_device_device_t *device_p,
                                        uint8_t configuration);

#endif
