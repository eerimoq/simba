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

#ifndef __DRIVERS_USB_DEVICE_CLASS_CDC_H__
#define __DRIVERS_USB_DEVICE_CLASS_CDC_H__

#include "simba.h"

struct usb_device_class_cdc_driver_t {
    struct usb_device_driver_base_t base;
    struct usb_device_driver_t *drv_p;
    int control_interface;
    int endpoint_in;
    int endpoint_out;
    int line_state;
    struct usb_cdc_line_info_t line_info;
    struct chan_t chout;
    struct queue_t chin;
};

/**
 * Initialize the CDC module.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] rxbuf_p Reception buffer.
 * @param[in] size Reception buffer size.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_init(struct usb_device_class_cdc_driver_t *self_p,
                              int control_interface,
                              int endpoint_in,
                              int endpoint_out,
                              void *rxbuf_p,
                              size_t size);

/**
 * Read data from the CDC driver.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of bytes read or negative error code.
 */
#define usb_device_class_cdc_read(self_p, buf_p, size)  \
    queue_read(&(self_p)->chin, buf_p, size)

/**
 * Write data to the CDC driver.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of bytes written or negative error code.
 */
#define usb_device_class_cdc_write(self_p, buf_p, size)         \
    (self_p)->chout.write(&(self_p)->chout, buf_p, size)

/**
 * Called by the USB device driver periodically to let the CDC driver
 * read received data from the hardware.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int usb_device_class_cdc_input_isr(struct usb_device_class_cdc_driver_t *self_p);

/**
 * Check if the CDC is connected to the remote endpoint.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return true(1) if connected, false(0) if disconnected, otherwise
 *         negative error code.
 */
int usb_device_class_cdc_is_connected(struct usb_device_class_cdc_driver_t *self_p);

#endif
