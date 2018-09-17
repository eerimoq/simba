/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

#include "usb_port.h"

ISR(uotghs)
{
    struct usb_driver_base_t *drv_p;

    drv_p = usb_device[0].drv_p;

    drv_p->isr(drv_p);

#if 0

    isr = dev_p->regs_p->HOST.ISR;

    /* Connection interrupt. */
    if (isr & SAM_UOTGHS_HOST_ISR_DCONNI) {
        /* Clear the connection interrupt since it's handled now. */
        dev_p->regs_p->HOST.ICR = SAM_UOTGHS_HOST_ICR_DCONNIC;

        /* Notify the USB thread about the interrupt. */
        mask = USB_HOST_PORT_EVENT_CONNECT;
        event_write_isr(&drv_p->event, &mask, sizeof(mask));
    }

    /* Pipe interrupts. */
    for (i = 0; i < 10; i++) {
        if (isr & (SAM_UOTGHS_HOST_ISR_PEP_0 << i)) {
            dev_p->regs_p->HOST.IDR = (SAM_UOTGHS_HOST_IDR_PEP_0 << i);

            if (dev_p->pipes[i].thrd_p != NULL) {
                thrd_resume_isr(dev_p->pipes[i].thrd_p, 0);
            }
        }
    }

#endif
}
