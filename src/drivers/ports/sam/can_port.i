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

#define MAILBOX_TX     0
#define MAILBOX_RX_EID 1
#define MAILBOX_RX_SID 2

static struct fs_counter_t rx_channel_overflow;

static void read_frame_from_hw(struct can_driver_t *self_p,
                               volatile struct sam_can_mailbox_t *mailbox_p)
{
    struct can_frame_t frame;
    uint32_t mid, msr;

    /* Read the frame from the hardware. */
    mid = mailbox_p->MID;
    frame.id = (((mid & CAN_MID_MIDVA_MASK) >> CAN_MID_MIDVA_POS)
                | ((mid & CAN_MID_MIDVB_MASK) << 11));
    frame.extended_frame = ((mid & CAN_MID_MIDE) != 0);
    msr = mailbox_p->MSR;
    frame.size = ((msr & CAN_MSR_MDLC_MASK) >> CAN_MSR_MDLC_POS);
    frame.rtr = ((msr & CAN_MSR_MRTR) != 0);
#if CONFIG_CAN_FRAME_TIMESTAMP == 1
    frame.timestamp = 0;
#endif
    frame.data.u32[0] = mailbox_p->MDL;
    frame.data.u32[1] = mailbox_p->MDH;

    /* Allow reception of the next message. */
    mailbox_p->MCR = CAN_MCR_MTCR;

    /* Write the received frame to the application input channel. */
    if (queue_unused_size_isr(&self_p->chin) >= sizeof(frame)) {
        queue_write_isr(&self_p->chin,
                        &frame,
                        sizeof(frame));

        /* Resume any polling thread. */
        if (chan_is_polled_isr(&self_p->base)) {
            thrd_resume_isr(self_p->base.reader_p, 0);
            self_p->base.reader_p = NULL;
        }
    } else {
        fs_counter_increment(&rx_channel_overflow, 1);
    }
}

static void write_frame_to_hw(volatile struct sam_can_mailbox_t *mailbox_p,
                              const struct can_frame_t *frame_p)
{
    /* Write the frame to the hardware. */
    if (frame_p->extended_frame == 0) {
        mailbox_p->MID = CAN_MID_MIDVA(frame_p->id);
    } else {
        mailbox_p->MID = (CAN_MID_MIDE
                          | CAN_MID_MIDVA(frame_p->id & 0x7ff)
                          | CAN_MID_MIDVB(frame_p->id >> 11));
    }

    mailbox_p->MDL = frame_p->data.u32[0];
    mailbox_p->MDH = frame_p->data.u32[1];

    /* Set DLC and trigger the transmission. */
    mailbox_p->MCR = (CAN_MSR_MDLC(frame_p->size)
                      | CAN_MCR_MTCR);
}

static void isr(struct can_device_t *dev_p)
{
    struct can_driver_t *self_p;
    uint32_t status;

    if (dev_p->drv_p == NULL) {
        return;
    }

    self_p = dev_p->drv_p;

    /* Read the status. */
    status = dev_p->regs_p->SR;
    status &= dev_p->regs_p->IMR;

    /* Handle TX complete interrupt. */
    if ((status & (1 << MAILBOX_TX)) != 0) {
        if (self_p->txsize > 0) {
            write_frame_to_hw(&dev_p->regs_p->MAILBOX[MAILBOX_TX],
                              self_p->txframe_p);
            self_p->txsize -= sizeof(*self_p->txframe_p);
            self_p->txframe_p++;
        } else {
            dev_p->regs_p->IDR = CAN_IDR_MB0;
            thrd_resume_isr(self_p->thrd_p, 0);
        }
    }

    if ((status & (1 << MAILBOX_RX_EID)) != 0) {
        read_frame_from_hw(self_p,
                           &dev_p->regs_p->MAILBOX[MAILBOX_RX_EID]);
    }

    if ((status & (1 << MAILBOX_RX_SID)) != 0) {
        read_frame_from_hw(self_p,
                           &dev_p->regs_p->MAILBOX[MAILBOX_RX_SID]);
    }
}

ISR(can0)
{
    isr(&can_device[0]);
}

ISR(can1)
{
    isr(&can_device[1]);
}

static ssize_t write_cb(void *arg_p,
                        const void *buf_p,
                        size_t size)
{
    struct can_driver_t *self_p;
    struct can_device_t *dev_p;
    const struct can_frame_t *frame_p = (struct can_frame_t *)buf_p;

    self_p = arg_p;
    dev_p = self_p->dev_p;

    sem_take(&self_p->sem, NULL);

    self_p->txframe_p = (frame_p + 1);
    self_p->txsize = (size - sizeof(*frame_p));
    self_p->thrd_p = thrd_self();

    write_frame_to_hw(&dev_p->regs_p->MAILBOX[MAILBOX_TX],
                      frame_p);

    sys_lock();
    dev_p->regs_p->IER = CAN_IER_MB0;
    thrd_suspend_isr(NULL);
    sys_unlock();

    sem_give(&self_p->sem, 1);

    return (size);
}

int can_port_module_init()
{
    fs_counter_init(&rx_channel_overflow,
                    FSTR("/drivers/can/rx_channel_overflow"),
                    0);
    fs_counter_register(&rx_channel_overflow);

    return (0);
}

int can_port_init(struct can_driver_t *self_p,
                  struct can_device_t *dev_p,
                  uint32_t speed)
{
    self_p->speed = speed;

    return (0);
}

int can_port_start(struct can_driver_t *self_p)
{
    uint32_t mask;
    volatile struct sam_pio_t *pio_p;
    struct can_device_t *dev_p = self_p->dev_p;

    /* Configure tx pin. */
    mask = dev_p->tx.mask;
    pio_p = dev_p->tx.pio_p;
    pio_p->PDR = mask;
    pio_p->ABSR &= ~mask;

    /* Configure rx pin. */
    mask = dev_p->rx.mask;
    pio_p = dev_p->rx.pio_p;
    pio_p->PDR = mask;
    pio_p->ABSR &= ~mask;

    pmc_peripheral_clock_enable(dev_p->id);

    /* One TX mailbox. */
    dev_p->regs_p->MAILBOX[MAILBOX_TX].MMR = CAN_MMR_MOT_MB_TX;
    dev_p->regs_p->MAILBOX[MAILBOX_TX].MAM = CAN_MAM_MIDVA(0x7ff);
    dev_p->regs_p->MAILBOX[MAILBOX_TX].MID &= ~CAN_MAM_MIDE;

    /* 1 RX mailboxes for extended ID. */
    dev_p->regs_p->MAILBOX[MAILBOX_RX_EID].MMR = CAN_MMR_MOT_MB_RX;
    dev_p->regs_p->MAILBOX[MAILBOX_RX_EID].MAM = CAN_MAM_MIDE;
    dev_p->regs_p->MAILBOX[MAILBOX_RX_EID].MID = CAN_MID_MIDE;

    /* 1 RX mailboxes for standard ID. */
    dev_p->regs_p->MAILBOX[MAILBOX_RX_SID].MMR = CAN_MMR_MOT_MB_RX;
    dev_p->regs_p->MAILBOX[MAILBOX_RX_SID].MAM = 0;
    dev_p->regs_p->MAILBOX[MAILBOX_RX_SID].MID = 0;

    /* Baud rate. */
    dev_p->regs_p->BR = self_p->speed;

    dev_p->regs_p->IDR = 0xffffffff;
    /* Enable interrupt for RX mailboxes.*/
    dev_p->regs_p->IER = (CAN_IER_MB1 | CAN_IER_MB2);
    dev_p->regs_p->MR = (CAN_MR_CANEN);

    while ((dev_p->regs_p->SR & CAN_SR_WAKEUP) == 0);

    nvic_enable_interrupt(dev_p->id);

    dev_p->drv_p = self_p;

    return (0);
}

int can_port_stop(struct can_driver_t *self_p)
{
    struct can_device_t *dev_p = self_p->dev_p;

    nvic_disable_interrupt(dev_p->id);

    dev_p->regs_p->MR = 0;
    dev_p->regs_p->IDR = 0xffffffff;

    pmc_peripheral_clock_disable(dev_p->id);

    return (0);
}
