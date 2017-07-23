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

#ifndef __DRIVERS_USB_HOST_H__
#define __DRIVERS_USB_HOST_H__

#include "simba.h"

/* States of a device. */
#define USB_HOST_DEVICE_STATE_NONE          0
#define USB_HOST_DEVICE_STATE_ATTACHED      1

/* Pipe types. */
#define USB_PIPE_TYPE_CONTROL       0
#define USB_PIPE_TYPE_INTERRUPT     1
#define USB_PIPE_TYPE_ISOCHRONOUS   2
#define USB_PIPE_TYPE_BULK          3

/**
 * An USB device as seen by the host.
 */
struct usb_host_device_t {
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
    struct usb_host_driver_t *self_p;
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
struct usb_host_device_driver_t {
    int (*supports)(struct usb_host_device_t *);
    int (*enumerate)(struct usb_host_device_t *);
    struct usb_host_device_driver_t *next_p;
};

#include "usb_host_port.h"

/**
 * Initialize the USB host module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_module_init(void);

/**
 * Initialize the USB host driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] dev_p USB device to use.
 * @param[in] devices_p An array of devices. One entry in this array
 *                      is allocated for each USB device that is
 *                      connected to the host.
 * @param[in] length Length of the devices array.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_init(struct usb_host_driver_t *self_p,
                  struct usb_device_t *dev_p,
                  struct usb_host_device_t *devices_p,
                  size_t length);

/**
 * Start the USB host device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_start(struct usb_host_driver_t *self_p);

/**
 * Stop the USB host device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_stop(struct usb_host_driver_t *self_p);

/**
 * Add given class/vendor driver to the USB host driver.
 *
 * When a USB device is plugged in, its class and vendor information
 * is read by the host. Those values are used to find the device
 * driver for this particular device. If there is no driver, the
 * device cannot be configured and will not work.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] driver_p USB device driver to add.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_driver_add(struct usb_host_driver_t *self_p,
                        struct usb_host_device_driver_t *driver_p,
                        void *arg_p);

/**
 * Remove given class/vendor driver from the USB host driver.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] driver_p USB device driver to remove.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_driver_remove(struct usb_host_driver_t *self_p,
                           struct usb_host_device_driver_t *driver_p);

/**
 * Open given device in given driver. Open a device before reading and
 * writing data to it with usb_host_device_read() or
 * usb_host_device_write().
 *
 * @param[in] self_p Initialized driver.
 * @param[in] device Device to open.
 *
 * @return Opened device or NULL on failure.
 */
struct usb_host_device_t *
usb_host_device_open(struct usb_host_driver_t *self_p,
                     int device);

/**
 * Close given device in given driver.
 *
 * @param[in] self_p Initialized driver.
 * @param[in] device Device to close.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_device_close(struct usb_host_driver_t *self_p,
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
ssize_t usb_host_device_read(struct usb_host_device_t *device_p,
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
ssize_t usb_host_device_write(struct usb_host_device_t *device_p,
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
ssize_t usb_host_device_control_transfer(struct usb_host_device_t *device_p,
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
int usb_host_device_set_configuration(struct usb_host_device_t *device_p,
                                      uint8_t configuration);

#endif
