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

#include "simba.h"

static struct fs_counter_t rx_channel_overflow;

static void read_frame_from_hw(struct can_driver_t *self_p,
                               volatile struct spc5_flexcan_t *regs_p)
{
    struct can_frame_t frame;

    /* Read the frame from the hardware. */
    if (regs_p->MSGBUF[0].CTRL_STATUS & SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE) {
        frame.extended_frame = 1;
        frame.id = ((regs_p->MSGBUF[0].PRIO_ID
                     & SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID_MASK)
                    >> SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID_POS);
    } else {
        frame.extended_frame = 0;
        frame.id = ((regs_p->MSGBUF[0].PRIO_ID
                     & SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID_MASK)
                    >> SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID_POS);
    }

    frame.size = ((regs_p->MSGBUF[0].CTRL_STATUS
                   & SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH_MASK)
                  >> SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH_POS);
    frame.rtr = 0;
    frame.timestamp = 0;
    frame.data.u32[0] = regs_p->MSGBUF[0].DATA[0];
    frame.data.u32[1] = regs_p->MSGBUF[0].DATA[1];

    /* Allow reception of the next message. */
    regs_p->MSGBUF[0].CTRL_STATUS = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(4);

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

static void write_frame_to_hw(volatile struct spc5_flexcan_t *regs_p,
                              const struct can_frame_t *frame_p)
{
    uint32_t ctrl_status;

    /* Write the frame to the hardware. */
    if (frame_p->extended_frame == 0) {
        ctrl_status = 0;
        regs_p->MSGBUF[1].PRIO_ID = SPC5_FLEXCAN_MSGBUF_PRIO_ID_STD_ID(frame_p->id);
    } else {
        ctrl_status = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_IDE;
        regs_p->MSGBUF[1].PRIO_ID = SPC5_FLEXCAN_MSGBUF_PRIO_ID_EXT_ID(frame_p->id);
    }

    regs_p->MSGBUF[1].DATA[0] = frame_p->data.u32[0];
    regs_p->MSGBUF[1].DATA[1] = frame_p->data.u32[1];

    /* Set DLC and trigger the transmission. */
    regs_p->MSGBUF[1].CTRL_STATUS = (ctrl_status
                                     | SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(0xc)
                                     | SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_LENGTH(frame_p->size));
}

ISR(flexcan_0_esr_err)
{
    while (1);
}

ISR(flexcan_0_buf_00_03)
{
    struct can_driver_t *self_p;
    struct can_device_t *dev_p;
    volatile struct spc5_flexcan_t *regs_p;

    dev_p = &can_device[0];

    if (dev_p->drv_p == NULL) {
        return;
    }

    self_p = dev_p->drv_p;
    regs_p = dev_p->regs_p;

    /* Handle TX complete interrupt. */
    if (regs_p->IFLAG1 & 0x2) {
        /* Clear interrupt flag. */
        regs_p->IFLAG1 = 0x2;

        if (self_p->txsize > 0) {
            write_frame_to_hw(regs_p, self_p->txframe_p);
            self_p->txsize -= sizeof(*self_p->txframe_p);
            self_p->txframe_p++;
        } else {
            thrd_resume_isr(self_p->thrd_p, 0);
        }
    }

    if (regs_p->IFLAG1 & 0x1) {
        /* Clear interrupt flag. */
        regs_p->IFLAG1 = 0x1;

        read_frame_from_hw(self_p, regs_p);
    }
}

static ssize_t write_cb(void *arg_p,
                        const void *buf_p,
                        size_t size)
{
    struct can_driver_t *self_p;
    struct can_device_t *dev_p;
    volatile struct spc5_flexcan_t *regs_p;
    const struct can_frame_t *frame_p;

    self_p = arg_p;
    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    frame_p = (struct can_frame_t *)buf_p;

    self_p->txframe_p = (frame_p + 1);
    self_p->txsize = (size - sizeof(*frame_p));
    self_p->thrd_p = thrd_self();

    sys_lock();
    write_frame_to_hw(regs_p, frame_p);
    thrd_suspend_isr(NULL);
    sys_unlock();

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
    struct can_device_t *dev_p;
    volatile struct spc5_flexcan_t *regs_p;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;

    /* Pin configration. */
    SPC5_SIUL->PCR[16] = (1 << 10) | (1 << 9);
    SPC5_SIUL->PCR[17] = 0x0103;

    SPC5_INTC->PSR[68/4] = 0xffffffff;

    regs_p->MCR = (SPC5_FLEXCAN_MCR_HALT
                   | SPC5_FLEXCAN_MCR_SUPV
                   | SPC5_FLEXCAN_MCR_WRN_EN
                   | SPC5_FLEXCAN_MCR_SRX_DIS
                   | SPC5_FLEXCAN_MCR_AEN
                   | SPC5_FLEXCAN_MCR_MAXMB(1));
    regs_p->CTRL = (self_p->speed);

    /* RX message buffer initialization. */
    regs_p->MSGBUF[0].PRIO_ID = 0;
    regs_p->MSGBUF[0].CTRL_STATUS = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(4);

    /* TX message buffer initialization. */
    regs_p->MSGBUF[1].CTRL_STATUS = SPC5_FLEXCAN_MSGBUF_CTRL_STATUS_CODE(8);

    regs_p->IMASK1 = 0x3;
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
