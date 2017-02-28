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

#ifndef __DRIVERS_USB_HOST_CLASS_HID_H__
#define __DRIVERS_USB_HOST_CLASS_HID_H__

#include "simba.h"

/* Subclasses. */
#define USB_CLASS_HID_SUBCLASS_NONE            0
#define USB_CLASS_HID_SUBCLASS_BOOT_INTERFACE  1

/* Protocols. */
#define USB_CLASS_HID_PROTOCOL_NONE            0
#define USB_CLASS_HID_PROTOCOL_KEYBOARD        1
#define USB_CLASS_HID_PROTOCOL_MOUSE           2

struct usb_host_class_hid_device_t {
    uint8_t buf[1];
};

struct usb_host_class_hid_driver_t {
    struct usb_host_driver_t *usb_p;
    struct usb_host_class_hid_device_t *devices_p;
    size_t length;
    struct {
        size_t size;
    } report;
    struct usb_host_device_driver_t device_driver;
};

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] usb_p USB driver to use.
 * @param[in] devices_p Array of devices. One entry in this array is
                        allocated for each HID device that is connected to
                        the host.
 * @param[in] length Length of the devices array.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_class_hid_init(struct usb_host_class_hid_driver_t *self_p,
                            struct usb_host_driver_t *usb_p,
                            struct usb_host_class_hid_device_t *devices_p,
                            size_t length);

/**
 * Starts the HID driver.
 *
 * @param[in] self_p Initialized driver object to start.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_class_hid_start(struct usb_host_class_hid_driver_t *self_p);

/**
 * Stops the HID driver.
 *
 * @param[in] self_p Initialized to stop.
 *
 * @return zero(0) or negative error code.
 */
int usb_host_class_hid_stop(struct usb_host_class_hid_driver_t *self_p);

#endif
