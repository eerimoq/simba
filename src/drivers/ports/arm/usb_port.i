/**
 * @file drivers/arm/usb_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

ISR(uotghs)
{
    struct usb_device_t *dev_p = &usb_device[0];
    struct usb_driver_t *drv_p = dev_p->drv_p;
    uint32_t mask;

    if (drv_p == NULL) {
        return;
    }

    /* Notify the USB thread about the interrupt. */
    mask = EVENT_IRQ;
    event_write_irq(drv_p->event_p, &mask, sizeof(mask));
}

static int handle_event_irq()
{
    if (isr & CONN) {
        usb_enumerate();
    }
}

static void *usb_main()
{
    while (1) {
        event_read(&mask, sizeof(mask));
        
        if (mask & EVENT_IRQ) {
            handle_event_irq();
        }
    }
}

static int usb_port_init(struct usb_driver_t *drv_p,
                         struct usb_device_t *dev_p)
{
    return (0);
}

static int usb_port_start(struct usb_driver_t *drv_p)
{
    struct usb_device_t *dev_p = drv_p->dev_p;

    pmc_peripheral_clock_enable(dev_p->id);
    nvic_enable_interrupt(dev_p->id);

    dev_p->drv_p = drv_p;

    return (0);
}

static int usb_port_stop(struct usb_driver_t *drv_p)
{
    struct usb_device_t *dev_p = drv_p->dev_p;

    dev_p->drv_p = NULL;

    return (0);
}
