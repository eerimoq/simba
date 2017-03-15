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

/* Copyright (c) 2010, Peter Barrett
**
** Permission to use, copy, modify, and/or distribute this software
** for any purpose with or without fee is hereby granted, provided
** that the above copyright notice and this permission notice appear
** in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
** AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
** CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
** OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
** NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
** CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <avr/interrupt.h>
#include <avr/power.h>

#define ENDPOINT_TYPE_CONTROL                   0x00
#define ENDPOINT_TYPE_BULK_IN			0x81
#define ENDPOINT_TYPE_BULK_OUT                  0x80
#define ENDPOINT_TYPE_INTERRUPT_IN		0xc1
#define ENDPOINT_TYPE_INTERRUPT_OUT		0xc0
#define ENDPOINT_TYPE_ISOCHRONOUS_IN		0x41
#define ENDPOINT_TYPE_ISOCHRONOUS_OUT		0x40

#define ENDPOINT_SINGLE_64             0x32
#define ENDPOINT_DOUBLE_64             0x36

FAR const int8_t endpoints[] = {
    0,
    ENDPOINT_TYPE_INTERRUPT_IN,      /* CDC_ENDPOINT_ACM */
    ENDPOINT_TYPE_BULK_OUT,          /* CDC_ENDPOINT_OUT */
    ENDPOINT_TYPE_BULK_IN,           /* CDC_ENDPOINT_IN */
    ENDPOINT_TYPE_INTERRUPT_IN,      /* CDC_ENDPOINT_ACM */
    ENDPOINT_TYPE_BULK_OUT,          /* CDC_ENDPOINT_OUT */
    ENDPOINT_TYPE_BULK_IN,           /* CDC_ENDPOINT_IN */
};

/**
 * Select given endpoint in the CPU.
 */
static void endpoint_select(uint8_t index)
{
    UENUM = index;
}

/**
 * Select given endpoint in the CPU and intialize it.
 */
static void endpoint_init(uint8_t index,
                          uint8_t type,
                          uint8_t size)
{
    endpoint_select(index);
    UECONX = 1;
    UECFG0X = type;
    UECFG1X = size;
}

/**
 * Initialize all endpoints.
 */
static void endpoints_init(void)
{
    int i;

    for (i = 1; i < membersof(endpoints); i++) {
        endpoint_select(i);
        UECONX = 1;
        UECFG0X = endpoints[i];
        UECFG1X = ENDPOINT_DOUBLE_64;
    }

    UERST = 0x7e;
    UERST = 0;
}

/**
 * Read data from selected endpoint.
 */
static void endpoint_read(void* buf_p, uint8_t size)
{
    uint8_t *b_p = buf_p;

    while (size--) {
        *b_p++ = UEDATX;
    }
}

static void wait_in(void)
{
    while (!(UEINTX & _BV(TXINI)));
}

static void clear_in(void)
{
    UEINTX = ~_BV(TXINI);
}

static uint8_t wait_for_in_or_out(void)
{
    while ((UEINTX & (_BV(TXINI) | _BV(RXOUTI))) == 0);

    return ((UEINTX & _BV(RXOUTI)) == 0);
}

static void stall(void)
{
    UECONX = (_BV(STALLRQ) | _BV(EPEN));
}

static uint8_t read_write_allowed(void)
{
    return (UEINTX & _BV(RWAL));
}

static uint8_t send_space(void)
{
    if (read_write_allowed() == 0) {
        return (0);
    }

    return (64 - UEBCLX);
}

static int control_write(FAR const void* buf_p,
                         size_t size,
                         size_t offset)
{
    size_t i;
    FAR const uint8_t* b_p;

    b_p = buf_p;

    for (i = 0; i < size; i++) {
        if (wait_for_in_or_out() == 0) {
            return (-1);
        }

        UEDATX = *b_p++;

        if (((offset + i + 1) & 0x3f) == 0) {
            clear_in();
        }
    }

    return (0);
}

/**
 * Write an array of descriptors to the host.
 */
static int control_vwrite(FAR const union usb_descriptor_t **descs_pp,
                          size_t size)
{
    size_t offset;
    size_t desc_size;
    FAR const union usb_descriptor_t* desc_p;

    offset = 0;

    while (offset < size) {
        desc_p = *descs_pp++;

        /* Reading too much. */
        if (desc_p == NULL) {
            return (-1);
        }

        desc_size = desc_p->header.length;
        control_write(desc_p, desc_size, offset);

        offset += desc_size;
    }

    return (0);
}

static int handle_get_descriptor(struct usb_device_driver_t *self_p,
                                 uint8_t index,
                                 uint8_t type,
                                 uint16_t length)
{
    int res = 0;

    endpoint_select(0);

    switch (type) {

    case DESCRIPTOR_TYPE_CONFIGURATION:
        res = control_vwrite(&self_p->descriptors_pp[1], length);
        break;

    case DESCRIPTOR_TYPE_DEVICE:
        res = control_write(self_p->descriptors_pp[0], length, 0);
        break;

    case DESCRIPTOR_TYPE_STRING:
        res = -1;
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

static int handle_type_standard(struct usb_device_driver_t *self_p,
                                struct usb_setup_t *setup_p)
{
    int res = 0;

    switch (setup_p->request) {

    case REQUEST_GET_STATUS:
        UEDATX = 0;
        UEDATX = 0;
        break;

    case REQUEST_SET_ADDRESS:
        wait_in();
        UDADDR = (setup_p->u.set_address.device_address | _BV(ADDEN));
        break;

    case REQUEST_GET_DESCRIPTOR:
        res = handle_get_descriptor(self_p,
                                    setup_p->u.get_descriptor.descriptor_index,
                                    setup_p->u.get_descriptor.descriptor_type,
                                    setup_p->length);
        break;

    case REQUEST_SET_CONFIGURATION:
        if ((setup_p->request_type & REQUEST_TYPE_RECIPIENT_MASK)
            == REQUEST_TYPE_RECIPIENT_DEVICE) {
            endpoints_init();
            self_p->configuration =
                setup_p->u.set_configuration.configuration_value;
        } else {
            res = -1;
        }
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

static int handle_type_class(struct usb_device_driver_t *self_p,
                             struct usb_setup_t *setup_p)
{
    int i;
    struct usb_device_driver_base_t *driver_p;

    for (i = 0; i < self_p->drivers_max; i++) {
        driver_p = self_p->drivers_pp[i];

        if (driver_p->setup_isr(driver_p, setup_p) == 0) {
            return (0);
        }
    }

    return (-1);
}

ISR(USB_COM_vect)
{
    int res;
    struct usb_setup_t setup;
    struct usb_device_t *dev_p;
    struct usb_device_driver_t *self_p;

    /* Get the driver. */
    dev_p = &usb_device[0];
    self_p = dev_p->drv_p;

    if (self_p == NULL) {
        return;
    }

    endpoint_select(0);

    if ((UEINTX & _BV(RXSTPI)) == 0) {
        return;
    }

    endpoint_read(&setup, sizeof(setup));
    UEINTX = ~(_BV(RXSTPI) | _BV(RXOUTI) | _BV(TXINI));

    if (setup.request_type & REQUEST_TYPE_DATA_DIRECTION_DEVICE_TO_HOST) {
        wait_in();
    } else {
        clear_in();
    }

    switch (setup.request_type & REQUEST_TYPE_TYPE_MASK) {

    case REQUEST_TYPE_TYPE_STANDARD:
        res = handle_type_standard(self_p, &setup);
        break;

    case REQUEST_TYPE_TYPE_CLASS:
        res = handle_type_class(self_p, &setup);
        break;

    default:
        res = -1;
        break;
    }

    if (res == 0) {
        clear_in();
    } else {
        stall();
    }
}

ISR(USB_GEN_vect)
{
    int i;
    uint8_t status;
    struct usb_device_t *dev_p;
    struct usb_device_driver_t *self_p;
    struct usb_device_driver_base_t *driver_p;

    /* Read and clear the interrupt status register. */
    status = UDINT;
    UDINT = 0;

    /* Get the driver. */
    dev_p = &usb_device[0];
    self_p = dev_p->drv_p;

    if (self_p == NULL) {
        return;
    }

    /* End of Reset. */
    if (status & _BV(EORSTI)) {
        self_p->configuration = -1;
        endpoint_init(0, ENDPOINT_TYPE_CONTROL, ENDPOINT_SINGLE_64);
        UEIENX = _BV(RXSTPE);
    }

    /* Start of Frame. This frame is detected periodically. The period
       is 1 ms. */
    if (status & _BV(SOFI)) {
        for (i = 0; i < self_p->drivers_max; i++) {
            driver_p = self_p->drivers_pp[i];
            driver_p->start_of_frame_isr(driver_p);
        }
    }
}

static int usb_device_port_module_init(void)
{
    return (0);
}

static int usb_device_port_init(struct usb_device_driver_t *self_p,
                                struct usb_device_t *dev_p)
{
    return (0);
}

static int usb_device_port_start(struct usb_device_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;

    UHWCON |= _BV(UVREGE); /* power internal reg */
    USBCON = _BV(USBE) | _BV(FRZCLK);	/* clock frozen, usb enabled */

    PLLCSR |= (_BV(PINDIV) | _BV(PLLE));

    while ((PLLCSR & _BV(PLOCK)) == 0);

    /* Some tests on specific versions of macosx (10.7.3), reported
       some strange behaviors when the board is reset using the serial
       port touch at 1200 bps. This delay fixes this behavior. */
    time_busy_wait_us(1000);

    /* Start USB clock, enable VBUS Pad */
    USBCON &= ~_BV(FRZCLK);
    USBCON |= _BV(OTGPADE);

    /* Enable attach resistor, set full speed mode. */
    UDCON &= ~(_BV(RSTCPU) | _BV(LSM) | _BV(RMWKUP) | _BV(DETACH));

    /* Clear already pending WAKEUP / SUSPEND requests. */
    UDINT &= ~(_BV(WAKEUPI) | _BV(SUSPI));

    /* Enable interrupts for EOR (End of Reset), SOF (start of frame)
       and SUSPEND. */
    UDIEN = (_BV(EORSTE) | _BV(SOFE) | _BV(SUSPE));

    return (0);
}

static int usb_device_port_stop(struct usb_device_driver_t *self_p)
{
    UDIEN = _BV(DETACH);
    USBCON ^= _BV(USBE);

    return (0);
}

static ssize_t usb_device_port_read_isr(struct usb_device_driver_t *self_p,
                                        int endpoint,
                                        void *buf_p,
                                        size_t size)
{
    size_t i;
    size_t fifo_size;
    uint8_t *b_p;

    endpoint_select(endpoint);

    fifo_size = UEBCLX;
    size = MIN(size, fifo_size);

    if (size == 0) {
        return (0);
    }

    b_p = buf_p;

    for (i = 0; i < size; i++) {
        *b_p++ = UEDATX;
    }

    /* Release empty buffer. */
    if (fifo_size == size) {
        UEINTX = 0x6b;
    }

    return (size);
}

static ssize_t usb_device_port_write_isr(struct usb_device_driver_t *self_p,
                                         int endpoint,
                                         const void *buf_p,
                                         size_t size)
{
    int r = size;
    const uint8_t* b_p;
    uint8_t timeout;

    r = size;
    b_p = buf_p;
    timeout = 250;

    endpoint_select(endpoint);

    while (size > 0) {
        uint8_t n = send_space();

        if (n == 0) {
            if (--timeout == 0) {
                return (-1);
            }

            time_busy_wait_us(1000);
            continue;
        }

        if (n > size) {
            n = size;
        }

        /* Frame may have been released by the SOF interrupt
           handler. */
        if (read_write_allowed() == 0) {
            continue;
        }

        size -= n;

        while (n > 0) {
            UEDATX = *b_p++;
            n--;
        }

        /* Release the buffer. */
        UEINTX = 0x3a;
    }

    return (r);
}

static ssize_t usb_device_port_write(struct usb_device_driver_t *self_p,
                                     int endpoint,
                                     const void *buf_p,
                                     size_t size)
{
    sys_lock();
    size = usb_device_port_write_isr(self_p, endpoint, buf_p, size);
    sys_unlock();

    return (size);
}
