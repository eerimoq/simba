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

static struct fs_counter_t rx_channel_overflow;

/**
 * Read a frame from given message buffer.
 */
static void read_frame_from_hw(struct can_driver_t *self_p,
                               volatile struct spc5_flexcan_msgbuf_t *msgbuf_p)
{
    struct can_frame_t frame;

    /* Read the frame from the hardware. */
    frame.size = ((msgbuf_p->CTRL_STATUS
                   & SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH_MASK)
                  >> SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH_POS);
    frame.rtr = 0;
#if CONFIG_CAN_FRAME_TIMESTAMP == 1
    frame.timestamp = 0;
#endif
    frame.data.u32[0] = msgbuf_p->DATA[0];
    frame.data.u32[1] = msgbuf_p->DATA[1];

    if (msgbuf_p->CTRL_STATUS & SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE) {
        frame.extended_frame = 1;
        frame.id = ((msgbuf_p->PRIO_ID
                     & SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID_MASK)
                    >> SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID_POS);

        /* Allow reception of the next message. */
        msgbuf_p->CTRL_STATUS = (SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(4)
                                 | SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE);
    } else {
        frame.extended_frame = 0;
        frame.id = ((msgbuf_p->PRIO_ID
                     & SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID_MASK)
                    >> SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID_POS);

        /* Allow reception of the next message. */
        msgbuf_p->CTRL_STATUS = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(4);
    }

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

/**
 * Write given frame to the hardware for transmission.
 */
static void write_frame_to_hw(volatile struct spc5_flexcan_t *regs_p,
                              const struct can_frame_t *frame_p)
{
    uint32_t ctrl_status;
    volatile struct spc5_flexcan_msgbuf_t *msgbuf_p;

    /* Write the frame to the hardware. */
    if (frame_p->extended_frame == 0) {
        ctrl_status = 0;
        msgbuf_p = &regs_p->MSGBUF[2];
        msgbuf_p->PRIO_ID = SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID(frame_p->id);
    } else {
        ctrl_status = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE;
        msgbuf_p = &regs_p->MSGBUF[3];
        msgbuf_p->PRIO_ID = SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID(frame_p->id);
    }

    msgbuf_p->DATA[0] = frame_p->data.u32[0];
    msgbuf_p->DATA[1] = frame_p->data.u32[1];

    /* Set DLC and trigger the transmission. */
    msgbuf_p->CTRL_STATUS =
        (ctrl_status
         | SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(0xc)
         | SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH(frame_p->size));
}

ISR(flexcan_0_esr_err)
{
    struct can_driver_t *self_p;
    volatile struct spc5_flexcan_t *regs_p;

    self_p = can_device[0].drv_p;
    regs_p = can_device[0].regs_p;

    if (self_p != NULL) {
        if (self_p->thrd_p != NULL) {
            /* Abort the ongoing transmission. */
            if (self_p->txframe_p[-1].extended_frame == 0) {
                regs_p->MSGBUF[2].CTRL_STATUS =
                    SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(0x9);
            } else {
                regs_p->MSGBUF[3].CTRL_STATUS =
                    SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(0x9);
            }

            /* Resume the thread. */
            thrd_resume_isr(self_p->thrd_p, -1);
            self_p->thrd_p = NULL;
        }
    }

    /* Clear the interrupt flag. */
    regs_p->ESR = SPC5_FLEXCAN_ESR_ERR_INT;
}

ISR(flexcan_0_buf_00_03)
{
    struct can_driver_t *self_p;
    volatile struct spc5_flexcan_t *regs_p;
    uint32_t iflag1;

    self_p = can_device[0].drv_p;
    regs_p = can_device[0].regs_p;

    iflag1 = regs_p->IFLAG1;

    if (self_p != NULL) {
        /* Handle standard ID RX complete interrupt. */
        if (iflag1 & 0x1) {
            read_frame_from_hw(self_p, &regs_p->MSGBUF[0]);
        }

        /* Handle extended ID RX complete interrupt. */
        if (iflag1 & 0x2) {
            read_frame_from_hw(self_p, &regs_p->MSGBUF[1]);
        }

        /* Handle TX complete interrupt. */
        if (iflag1 & 0xc) {
            if (self_p->txsize > 0) {
                write_frame_to_hw(regs_p, self_p->txframe_p);
                self_p->txsize -= sizeof(*self_p->txframe_p);
                self_p->txframe_p++;
            } else {
                thrd_resume_isr(self_p->thrd_p, 0);
                self_p->thrd_p = NULL;
            }
        }
    }

    /* Clear all handled flags. */
    regs_p->IFLAG1 = iflag1;
}

static ssize_t write_cb(void *arg_p,
                        const void *buf_p,
                        size_t size)
{
    struct can_driver_t *self_p;
    struct can_device_t *dev_p;
    volatile struct spc5_flexcan_t *regs_p;
    const struct can_frame_t *frame_p;
    int res;

    self_p = arg_p;
    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    frame_p = (struct can_frame_t *)buf_p;

    sem_take(&self_p->sem, NULL);

    self_p->txframe_p = (frame_p + 1);
    self_p->txsize = (size - sizeof(*frame_p));
    self_p->thrd_p = thrd_self();

    sys_lock();
    write_frame_to_hw(regs_p, frame_p);
    res = thrd_suspend_isr(NULL);
    sys_unlock();

    sem_give(&self_p->sem, 1);

    return (res == 0 ? size : -1);
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
    struct can_device_t *dev_p;
    volatile struct spc5_flexcan_t *regs_p;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;

    /* Pin configration. */
    SPC5_SIUL->PCR[16] = (1 << 10) | (1 << 9);
    SPC5_SIUL->PCR[17] = 0x0103;

    SPC5_INTC->PSR[64/4] = 0xffffffff;
    SPC5_INTC->PSR[68/4] = 0xffffffff;

    regs_p->MCR = (SPC5_FLEXCAN_MCR_HALT
                   | SPC5_FLEXCAN_MCR_SUPV
                   | SPC5_FLEXCAN_MCR_SRX_DIS
                   | SPC5_FLEXCAN_MCR_AEN
                   | SPC5_FLEXCAN_MCR_MAXMB(3));
    regs_p->CTRL = (SPC5_FLEXCAN_CTRL_ERR_MSK
                    | self_p->speed);

    /* Standard frame ID RX message buffer initialization. */
    regs_p->MSGBUF[0].PRIO_ID = 0;
    regs_p->MSGBUF[0].CTRL_STATUS = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(4);

    /* Extended frame ID RX message buffer initialization. */
    regs_p->MSGBUF[1].PRIO_ID = 0;
    regs_p->MSGBUF[1].CTRL_STATUS = (SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(4)
                                     | SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE);

    /* Standard frame ID TX message buffer initialization. */
    regs_p->MSGBUF[2].CTRL_STATUS = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(8);

    /* Extended frame ID TX message buffer initialization. */
    regs_p->MSGBUF[3].CTRL_STATUS = (SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(8)
                                     | SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE);

    regs_p->IMASK1 = 0xf;
    regs_p->RXGMASK = 0;

    /* Let it loose! */
    regs_p->MCR &= ~SPC5_FLEXCAN_MCR_HALT;

    dev_p->drv_p = self_p;

    return (0);
}

int can_port_stop(struct can_driver_t *self_p)
{
    return (0);
}
