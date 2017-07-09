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

#if CONFIG_USB_DEVICE == 1

#define LINE_STATE_DTR              0x01
#define STAY_IN_BOOT_LOADER_MAGIC 0x7777

/**
 * Output channel callback. Called when the user writes dat ato the
 * driver.
 */
static ssize_t write_cb(void *chan_p,
                        const void *buf_p,
                        size_t size)
{
    struct usb_device_class_cdc_driver_t *self_p;

    self_p = container_of(chan_p,
                          struct usb_device_class_cdc_driver_t,
                          chout);

    return (usb_device_write(self_p->drv_p,
                             self_p->endpoint_out,
                             buf_p,
                             size));
}

/**
 * Arduino Pro Micro bootloader.
 *
 * Auto-reset into the bootloader is triggered when the port, already
 * open at 1200 bps, is closed. This is the signal to start the
 * watchdog with a relatively long period so it can finish
 * housekeeping tasks like servicing endpoints before the application
 * ends.
 */
static int check_reset_to_bootloader(struct usb_device_class_cdc_driver_t *self_p)
{
#if defined(BOARD_ARDUINO_PRO_MICRO)

    if (self_p->line_info.dte_rate != 1200) {
        return (0);
    }

    /* Most OSs do some intermediate steps when configuring ports
       and DTR can twiggle more than once before stabilizing. To
       avoid spurious resets we set the watchdog to 120ms and
       eventually cancel if DTR goes back high. */

    /* We check DTR state to determine if host port is open (bit 0 of
       line state). */
    if ((self_p->line_state & LINE_STATE_DTR) == 0) {
        *(uint16_t *)0x0800 = STAY_IN_BOOT_LOADER_MAGIC;
        watchdog_start_ms(100, NULL);
    } else {
        *(uint16_t *)0x0800 = 0x0;
        watchdog_stop();
    }

#endif
    
    return (0);
}

/**
 * USB device driver callback.
 */
static int start_of_frame_isr(struct usb_device_driver_base_t *base_p)
{
    char c;
    struct usb_device_class_cdc_driver_t *self_p;

    self_p = (struct usb_device_class_cdc_driver_t *)base_p;

    /* Read data from the hardware and write it to the driver input
       queue. */
    while (1) {
        if (usb_device_read_isr(self_p->drv_p,
                                self_p->endpoint_in,
                                &c,
                                sizeof(c)) != sizeof(c)) {
            break;
        }

        if (queue_write_isr(&self_p->chin,
                            &c,
                            sizeof(c)) != sizeof(c)) {
            break;
        }
    }

    return (0);
}

/**
 * USB device driver setup callback.
 */
static int setup_isr(struct usb_device_driver_base_t *base_p,
                     struct usb_setup_t *setup_p)
{
    int res = 0;
    uint8_t direction;
    uint8_t type;
    struct usb_device_class_cdc_driver_t *self_p;

    self_p = (struct usb_device_class_cdc_driver_t *)base_p;

    type = (setup_p->request_type & REQUEST_TYPE_RECIPIENT_MASK);

    if (type != REQUEST_TYPE_RECIPIENT_INTERFACE) {
        return (1);
    }

    /* Is the addressed interface owned by this driver? */
    if (self_p->control_interface != setup_p->u.base.index) {
        return (1);
    }

    time_busy_wait_us(50);

    direction = (setup_p->request_type & REQUEST_TYPE_DATA_MASK);

    switch (direction) {

    case REQUEST_TYPE_DATA_DIRECTION_DEVICE_TO_HOST:
        switch (setup_p->request) {

        case USB_CDC_LINE_CODING:
            usb_device_write_isr(NULL,
                                 0,
                                 &self_p->line_info,
                                 sizeof(self_p->line_info));
            res = 0;
            break;

        default:
            res = -1;
            break;
        }

        break;

    case REQUEST_TYPE_DATA_DIRECTION_HOST_TO_DEVICE:
        switch (setup_p->request) {

        case USB_CDC_LINE_CODING:
            usb_device_read_isr(NULL,
                                0,
                                &self_p->line_info,
                                sizeof(self_p->line_info));
            check_reset_to_bootloader(self_p);
            res = 0;
            break;

        case USB_CDC_CONTROL_LINE_STATE:
            self_p->line_state = setup_p->u.base.value;
            check_reset_to_bootloader(self_p);
            res = 0;
            break;

        case USB_CDC_SEND_BREAK:
            /* Lost serial connection; mark line state as gone. */
            self_p->line_state = 0;
            res = 0;
            break;

        default:
            res = -1;
            break;
        }

        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

#if CONFIG_FS_CMD_USB_DEVICE_LIST == 1

static int print(struct usb_device_driver_base_t *base_p,
                 void *chout_p)
{
    struct usb_device_class_cdc_driver_t *self_p;

    self_p = (struct usb_device_class_cdc_driver_t *)base_p;

    std_fprintf(chout_p,
                FSTR("CDC ACM (emulated serial port over USB)\r\n"
                     "  endpoint_in = %d\r\n"
                     "  endpoint_out = %d\r\n"
                     "  line_state = %d\r\n"
                     "  line_info {\r\n"
                     "    dte_rate = %ld\r\n"
                     "    char_format = %d\r\n"
                     "    parity_type = %d\r\n"
                     "    data_bits = %d\r\n"
                     "  }\r\n"),
                self_p->endpoint_in,
                self_p->endpoint_out,
                self_p->line_state,
                (long)self_p->line_info.dte_rate,
                (int)self_p->line_info.char_format,
                (int)self_p->line_info.parity_type,
                (int)self_p->line_info.data_bits);

    return (0);
}

#endif

int usb_device_class_cdc_module_init(void)
{
    return (0);
}

int usb_device_class_cdc_init(struct usb_device_class_cdc_driver_t *self_p,
                              int control_interface,
                              int endpoint_in,
                              int endpoint_out,
                              void *rxbuf_p,
                              size_t size)
{
    self_p->base.start_of_frame_isr = start_of_frame_isr;
    self_p->base.setup_isr = setup_isr;
#if CONFIG_FS_CMD_USB_DEVICE_LIST == 1
    self_p->base.print = print;
#endif

    self_p->control_interface = control_interface;
    self_p->endpoint_in = endpoint_in;
    self_p->endpoint_out = endpoint_out;
    self_p->line_state = 0;

    self_p->line_info.dte_rate = 38400;
    self_p->line_info.char_format = 0;
    self_p->line_info.parity_type = 0;
    self_p->line_info.data_bits = 0;

    chan_init(&self_p->chout,
              chan_read_null,
              write_cb,
              chan_size_null);

    if (size > 0) {
        queue_init(&self_p->chin, rxbuf_p, size);
    }

    return (0);
}

int usb_device_class_cdc_is_connected(struct usb_device_class_cdc_driver_t *self_p)
{
    return (self_p->line_state != 0);
}

#endif
