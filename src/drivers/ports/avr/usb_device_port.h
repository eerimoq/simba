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

#ifndef __DRIVERS_USB_DEVICE_PORT_H__
#define __DRIVERS_USB_DEVICE_PORT_H__

#include "simba.h"

struct usb_device_driver_base_t;

typedef int (*usb_device_start_of_frame_cb_t)(struct usb_device_driver_base_t *self_p);
typedef int (*usb_device_setup_cb_t)(struct usb_device_driver_base_t *self_p,
                                     struct usb_setup_t *setup_p);
typedef int (*usb_device_print_cb_t)(struct usb_device_driver_base_t *self_p,
                                     void *chout_p);

struct usb_device_driver_base_t {
    struct usb_device_driver_base_t *next_p;
    usb_device_start_of_frame_cb_t start_of_frame_isr;
    usb_device_setup_cb_t setup_isr;
#if CONFIG_FS_CMD_USB_DEVICE_LIST == 1
    usb_device_print_cb_t print;
#endif
};

struct usb_device_driver_t {
    struct usb_device_t *dev_p;
    int configuration;
    struct usb_device_driver_base_t **drivers_pp;
    int drivers_max;
    FAR const union usb_descriptor_t **descriptors_pp;
};

#endif
