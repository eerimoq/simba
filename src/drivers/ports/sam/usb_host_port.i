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

#include "usb_host_port.h"

/* Events set by the interrupt service routine to notify the main
 * thread about new events. */
#define EVENT_CONNECT 0x1

void state(int pipe)
{
    /* std_printf(FSTR("  HOST.CTRL = 0x%lx\r\n"), SAM_UOTGHS->HOST.CTRL); */
    /* std_printf(FSTR("  HOST.ISR = 0x%lx\r\n"), SAM_UOTGHS->HOST.ISR); */
    /* std_printf(FSTR("  HOST.IMR = 0x%lx\r\n"), SAM_UOTGHS->HOST.IMR); */

    /* std_printf(FSTR("  HOST.ADDR[0] = 0x%lx\r\n"), SAM_UOTGHS->HOST.ADDR[0]); */

    /* std_printf(FSTR("      HOST.PIPCFG[%d] = 0x%lx\r\n"), pipe, SAM_UOTGHS->HOST.PIPCFG[pipe]); */
    /* std_printf(FSTR("      HOST.PIPISR[%d] = 0x%08lx\r\n"), pipe, SAM_UOTGHS->HOST.PIPISR[pipe]); */
    /* std_printf(FSTR("      HOST.PIPIMR[%d] = 0x%08lx\r\n\r\n"), pipe, SAM_UOTGHS->HOST.PIPIMR[pipe]); */

    /* std_printf(FSTR("      HOST.PIPCFG[1] = 0x%08lx\r\n"), SAM_UOTGHS->HOST.PIPCFG[1]); */
    /* std_printf(FSTR("      HOST.PIPISR[1] = 0x%08lx\r\n"), SAM_UOTGHS->HOST.PIPISR[1]); */
    /* std_printf(FSTR("      HOST.PIPIMR[1] = 0x%08lx\r\n\r\n"), SAM_UOTGHS->HOST.PIPIMR[1]); */

    /* std_printf(FSTR("      HOST.PIPCFG[2] = 0x%08lx\r\n"), SAM_UOTGHS->HOST.PIPCFG[2]); */
    /* std_printf(FSTR("      HOST.PIPISR[2] = 0x%08lx\r\n"), SAM_UOTGHS->HOST.PIPISR[2]); */
    /* std_printf(FSTR("      HOST.PIPIMR[2] = 0x%08lx\r\n\r\n"), SAM_UOTGHS->HOST.PIPIMR[2]); */
}

ISR(uotghs)
{
    struct usb_device_t *dev_p = &usb_device[0];
    struct usb_host_driver_t *drv_p = dev_p->drv_p;
    uint32_t mask;
    uint32_t isr;
    int i;

    if (drv_p == NULL) {
        return;
    }

    isr = dev_p->regs_p->HOST.ISR;

    /* Connection interrupt. */
    if (isr & SAM_UOTGHS_HOST_ISR_DCONNI) {
        /* Clear the connection interrupt since it's handled now. */
        dev_p->regs_p->HOST.ICR = SAM_UOTGHS_HOST_ICR_DCONNIC;

        /* Notify the USB thread about the interrupt. */
        mask = EVENT_CONNECT;
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
}

/**
 * Handle the device connect attempt hardware interrupt.
 */
static int handle_event_connect(struct usb_host_driver_t *self_p)
{
    return (device_enumerate(self_p->devices_p));
}

static int device_reset(struct usb_host_device_t *device_p)
{
    std_printf(FSTR("Resetting the device.\r\n"));

    device_p->self_p->dev_p->regs_p->HOST.CTRL = SAM_UOTGHS_HOST_CTRL_RESET;

    thrd_sleep_us(100000);

    return (0);
}

static void *usb_main(struct usb_host_driver_t *self_p)
{
    uint32_t mask;

    thrd_set_name("usb");

    while (1) {
        mask = EVENT_CONNECT;
        event_read(&self_p->event, &mask, sizeof(mask));

        if (mask & EVENT_CONNECT) {
            handle_event_connect(self_p);
        }
    }

    return (NULL);
}

static int usb_host_port_init(struct usb_host_driver_t *self_p,
                              struct usb_device_t *dev_p)
{
    int i;

    event_init(&self_p->event);

    for (i = 0; i < membersof(dev_p->pipes); i++) {
        dev_p->pipes[i].id = i;
        dev_p->pipes[i].state = USB_PIPE_STATE_FREE;
    }

    return (0);
}

static int usb_host_port_start(struct usb_host_driver_t *self_p)
{
    struct usb_device_t *dev_p = self_p->dev_p;

    std_printf(FSTR("Starting the USB clock.\r\n"));

    /* Start the USB clock. */
    SAM_PMC->CKGR_UCKR = (PMC_CKGR_UCKR_UPLLCOUNT(3)
                          | PMC_CKGR_UCKR_UPLLEN);

    while ((SAM_PMC->SR & PMC_SR_LOCKU) == 0);

    SAM_PMC->USB = (PMC_USB_USBS | PMC_USB_USBDIV(0));
    SAM_PMC->SCER = PMC_UOTGCLK;

    pmc_peripheral_clock_enable(dev_p->id);

    /* Enable the device in the nvic interrupt subsyetem. */
    nvic_enable_interrupt(dev_p->id);

    std_printf(FSTR("Enabling the USB hardware.\r\n"));

    /* Enable the USB hardware. Force host mode. */
    dev_p->regs_p->CTRL = (SAM_UOTGHS_CTRL_OTGPADE
                           | SAM_UOTGHS_CTRL_USBE
                           | SAM_UOTGHS_CTRL_VBUSPO);

    std_printf(FSTR("Waiting for the clock to be usable.\r\n"));

    /* Wait for the USB clock to stablize. */
    while ((dev_p->regs_p->SR & SAM_UOTGHS_SR_CLKUSABLE) == 0);

    /* Enable the USB hardware. Force host mode. */
    dev_p->regs_p->CTRL = (SAM_UOTGHS_CTRL_OTGPADE
                           | SAM_UOTGHS_CTRL_USBE
                           | SAM_UOTGHS_CTRL_VBUSPO
                           | SAM_UOTGHS_CTRL_VBUSHWC);
    dev_p->regs_p->SFR = BIT(9);

    /* Enable connection attempt interrupt. */
    dev_p->regs_p->HOST.IER = SAM_UOTGHS_HOST_IER_DCONNIES;

    dev_p->drv_p = self_p;

    thrd_spawn((void *(*)(void *))usb_main,
               self_p,
               -19,
               self_p->stack,
               sizeof(self_p->stack));

    std_printf(FSTR("USB start finished.\r\n"));

    return (0);
}

static int usb_host_port_stop(struct usb_host_driver_t *self_p)
{
    struct usb_device_t *dev_p = self_p->dev_p;

    /* Disable the USB hardware. */
    dev_p->regs_p->CTRL &= ~SAM_UOTGHS_CTRL_USBE;

    /* Stop the USB clock. */

    dev_p->drv_p = NULL;

    return (0);
}

static struct usb_pipe_t *
usb_pipe_alloc(struct usb_host_driver_t *self_p,
               int type,
               int endpoint,
               int address,
               int interval)
{
    struct usb_device_t *dev_p = self_p->dev_p;
    volatile struct sam_uotghs_t *regs_p = dev_p->regs_p;
    struct usb_pipe_t *pipe_p;
    int i;
    uint32_t reg = 0;

    if (type == USB_PIPE_TYPE_CONTROL) {
        pipe_p = &dev_p->pipes[0];
        reg = (SAM_UOTGHS_HOST_PIPCFG_PTYPE_CTRL
               | SAM_UOTGHS_HOST_PIPCFG_PSIZE_64_BYTES);
    } else {
        pipe_p = NULL;

        for (i = 0; i < membersof(dev_p->pipes); i++) {
            if (dev_p->pipes[i].state == USB_PIPE_STATE_FREE) {
                pipe_p = &dev_p->pipes[i];

                break;
            }
        }

        /* No free pipes. */
        if (pipe_p == NULL) {
            return (NULL);
        }

        reg = SAM_UOTGHS_HOST_PIPCFG_PSIZE_1024_BYTES;

        switch (type) {

        case USB_PIPE_TYPE_INTERRUPT:
            reg |= (SAM_UOTGHS_HOST_PIPCFG_PTYPE_INTRPT
                   | SAM_UOTGHS_HOST_PIPCFG_INTFRQ_BINTERVAL(interval));
            break;

        case USB_PIPE_TYPE_ISOCHRONOUS:
            reg |= SAM_UOTGHS_HOST_PIPCFG_PTYPE_ISO;
            break;

        case USB_PIPE_TYPE_BULK:
            reg |= SAM_UOTGHS_HOST_PIPCFG_PTYPE_BLK;
            break;

        default:
            return (NULL);
        }
    }

    /* Set the pipe address. */
    regs_p->HOST.ADDR[pipe_p->id / 4] =
        bits_insert_32(regs_p->HOST.ADDR[pipe_p->id / 4],
                       8 * (pipe_p->id & 0x3),
                       8,
                       address);

    /* Configure the pipe. */
    regs_p->HOST.PIP |= (SAM_UOTGHS_HOST_PIP_PEN0 << pipe_p->id);
    regs_p->HOST.PIPCFG[pipe_p->id] =
        (SAM_UOTGHS_HOST_PIPCFG_PEPNUM(endpoint)
         | SAM_UOTGHS_HOST_PIPCFG_PBK_1_BANK
         | SAM_UOTGHS_HOST_PIPCFG_ALLOC
         | reg);

    if ((regs_p->HOST.PIPISR[pipe_p->id] & SAM_UOTGHS_HOST_PIPISR_CFGOK) == 0) {
        std_printf(FSTR("Pipe not configured correctly.\r\n"));

        return (NULL);
    }

    pipe_p->state = USB_PIPE_STATE_ALLOCATED;

    std_printf(FSTR("Allocated pipe at index %d.\r\n"), pipe_p->id);

    return (pipe_p);
}

static int usb_pipe_free(struct usb_host_driver_t *self_p,
                         struct usb_pipe_t *pipe_p)
{
    struct usb_device_t *dev_p = self_p->dev_p;
    volatile struct sam_uotghs_t *regs_p = dev_p->regs_p;

    /* Disable the pipe. */
    regs_p->HOST.PIP &= ~(SAM_UOTGHS_HOST_PIP_PEN0 << pipe_p->id);
    pipe_p->state = USB_PIPE_STATE_FREE;

    return (0);
}

static int usb_pipe_set_address(struct usb_host_driver_t *self_p,
                                struct usb_pipe_t *pipe_p,
                                int address)
{
    struct usb_device_t *dev_p = self_p->dev_p;
    volatile struct sam_uotghs_t *regs_p = dev_p->regs_p;

    /* Set the pipe address. */
    regs_p->HOST.ADDR[pipe_p->id / 4] =
        bits_insert_32(regs_p->HOST.ADDR[pipe_p->id / 4],
                       8 * (pipe_p->id & 0x3),
                       8,
                       address);

    return (0);
}

static ssize_t pipe_transfer(struct usb_device_t *dev_p,
                             uint8_t pipe)
{
    volatile struct sam_uotghs_t *regs_p = dev_p->regs_p;

    /* std_printf(FSTR("pipe %d transfer\r\n"), pipe); */
    /* state(pipe); */

    /* Start the transfer. */
    dev_p->pipes[pipe].thrd_p = thrd_self();

    sys_lock();

    /* Enable the transfer complete interrupt. */
    regs_p->HOST.IER = (SAM_UOTGHS_HOST_IER_PEP_0 << pipe);

    /* Start the transfer.*/
    regs_p->HOST.PIPIDR[pipe] = (SAM_UOTGHS_HOST_PIPIDR_FIFOCONC
                                 | SAM_UOTGHS_HOST_PIPIDR_PFREEZEC);

    /* Wait for completion. */
    thrd_suspend_isr(NULL);

    dev_p->pipes[pipe].thrd_p = NULL;

    sys_unlock();

    /* state(pipe); */

    return (0);
}

/**
 * Write a SETUP packet to the device.
 */
static ssize_t 
usb_host_port_device_write_setup(struct usb_host_device_t *device_p,
                                 struct usb_setup_t *setup_p)
{
    struct usb_device_t *dev_p = device_p->self_p->dev_p;
    volatile struct sam_uotghs_t *regs_p = dev_p->regs_p;
    uint8_t *fifo_p, *buf8_p = (uint8_t *)setup_p;
    int pipe = 0, i;

    regs_p->HOST.PIPCFG[pipe] = SAM_UOTGHS_HOST_PIPCFG_PTOKEN_SET(
        regs_p->HOST.PIPCFG[pipe], SETUP);

    /* Write data to the pipe fifo. */
    fifo_p = (&((uint8_t *)SAM_UOTGHS_DPRAM_ADDRESS)[SAM_UOTGHS_DPRAM_PIPE_SIZE * pipe]);

    for (i = 0; i < sizeof(*setup_p); i++) {
        *fifo_p++ = *buf8_p++;
    }

    regs_p->HOST.PIPIER[pipe] = SAM_UOTGHS_HOST_PIPIER_TXSTPES_UNDERFES;
    regs_p->HOST.PIPICR[pipe] = SAM_UOTGHS_HOST_PIPICR_SHORTPACKETIC;

    pipe_transfer(dev_p, pipe);

    regs_p->HOST.PIPIER[pipe] = SAM_UOTGHS_HOST_PIPIER_PFREEZES;
    regs_p->HOST.PIPIDR[pipe] = SAM_UOTGHS_HOST_PIPIDR_TXSTPEC_UNDERFEC;
    regs_p->HOST.PIPICR[pipe] = SAM_UOTGHS_HOST_PIPICR_TXSTPIC_UNDERFIC;

    return (sizeof(*setup_p));
}

static ssize_t usb_host_port_device_write(struct usb_host_device_t *device_p,
                                          uint8_t endpoint,
                                          const void *buf_p,
                                          size_t size)
{
    struct usb_device_t *dev_p = device_p->self_p->dev_p;
    volatile struct sam_uotghs_t *regs_p = dev_p->regs_p;
    uint8_t *fifo_p;
    const uint8_t *buf8_p = buf_p;
    int pipe = endpoint;
    size_t left, n, i;

    /* This shold already be comfigured to the pipe. Should be removed
     * from this function. */
    regs_p->HOST.PIPCFG[pipe] = SAM_UOTGHS_HOST_PIPCFG_PTOKEN_SET(
        regs_p->HOST.PIPCFG[pipe], OUT);

    left = size;

    do {
        if (left > device_p->max_packet_size) {
            n = device_p->max_packet_size;
        } else {
            n = left;
        }

        /* Write data to the pipe fifo. */
        fifo_p = (&((uint8_t *)SAM_UOTGHS_DPRAM_ADDRESS)[SAM_UOTGHS_DPRAM_PIPE_SIZE * pipe]);

        /* Copy data from the local buffer to the transmission
         * fifo. */
        for (i = 0; i < n; i++) {
            *fifo_p++ = *buf8_p++;
        }

        /* Enable the transmission complete interrupt. */
        regs_p->HOST.PIPIER[pipe] = SAM_UOTGHS_HOST_PIPIER_TXOUTES;
        regs_p->HOST.PIPICR[pipe] = SAM_UOTGHS_HOST_PIPICR_SHORTPACKETIC;

        pipe_transfer(dev_p, pipe);

        regs_p->HOST.PIPIER[pipe] = SAM_UOTGHS_HOST_PIPIER_PFREEZES;
        regs_p->HOST.PIPIDR[pipe] = SAM_UOTGHS_HOST_PIPIDR_TXOUTEC;
        regs_p->HOST.PIPICR[pipe] = SAM_UOTGHS_HOST_PIPICR_TXOUTIC;

        left -= n;
    } while (left > 0);

    return (size);
}

static ssize_t usb_host_port_device_read(struct usb_host_device_t *device_p,
                                         uint8_t endpoint,
                                         void *buf_p,
                                         size_t size)
{
    struct usb_device_t *dev_p = device_p->self_p->dev_p;
    volatile struct sam_uotghs_t *regs_p = dev_p->regs_p;
    uint8_t *fifo_p, *buf8_p = buf_p;
    int pipe = endpoint;
    size_t i, left, n;
    size_t count;
    int short_packet = 0;

    /* This shold already be comfigured to the pipe. Should be removed
     * from this function. */
    regs_p->HOST.PIPCFG[pipe] = SAM_UOTGHS_HOST_PIPCFG_PTOKEN_SET(
        regs_p->HOST.PIPCFG[pipe], IN);

    left = size;

    do {
        if (left > device_p->max_packet_size) {
            n = device_p->max_packet_size;
        } else {
            n = left;
        }

        /* Enable the reception complete interrupt. */
        regs_p->HOST.PIPIER[pipe] = SAM_UOTGHS_HOST_PIPIER_RXINES;

        pipe_transfer(dev_p, pipe);

        regs_p->HOST.PIPIDR[pipe] = SAM_UOTGHS_HOST_PIPIDR_RXINEC;
        regs_p->HOST.PIPICR[pipe] = SAM_UOTGHS_HOST_PIPICR_RXINIC;

        /* Read data from the pipe fifo. */
        fifo_p = (&((uint8_t *)SAM_UOTGHS_DPRAM_ADDRESS)[SAM_UOTGHS_DPRAM_PIPE_SIZE * pipe]);

        count = ((SAM_UOTGHS->HOST.PIPISR[pipe] & SAM_UOTGHS_HOST_PIPISR_PBYCT_MASK)
                 >> SAM_UOTGHS_HOST_PIPISR_PBYCT_POS);
        /* Needed to make a mouse work. Unclear why. */
        /* std_printf(FSTR("n = %lu, rx count = %lu\r\n"), n, count); */

        if (count < n) {
            n = count;
            short_packet = 1;
        }

        /* Copy the data received from the device to the local
         * buffer. */
        for (i = 0; i < n; i++) {
            *buf8_p++ = *fifo_p++;
        }

        /* Data read. */
        regs_p->HOST.PIPIDR[pipe] = SAM_UOTGHS_HOST_PIPIDR_FIFOCONC;

        left -= n;
    } while ((left > 0) && (short_packet == 0));

    return (size - left);
}
