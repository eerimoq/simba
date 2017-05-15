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

#undef BIT
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_intr.h"
#include "soc/dport_reg.h"

static struct fs_counter_t rx_channel_overflow;
static struct fs_counter_t errors;

/**
 * Read a frame from the hardware.
 */
static void read_frame_from_hw(struct can_driver_t *self_p,
                               volatile struct esp32_can_t *regs_p)
{
    struct can_frame_t frame;
    uint8_t frame_info;
    volatile uint32_t *data_p;
    int i;

    frame_info = regs_p->U.TX_RX.FRAME_INFO;

    if (frame_info & ESP32_CAN_FRAME_INFO_FF) {
        frame.extended_frame = 1;
        frame.id = (((uint32_t)regs_p->U.TX_RX.ID_DATA[0] << 21)
                    | ((uint32_t)regs_p->U.TX_RX.ID_DATA[1] << 13)
                    | ((uint32_t)regs_p->U.TX_RX.ID_DATA[2] << 5)
                    | (regs_p->U.TX_RX.ID_DATA[3] >> 3));
        data_p = &regs_p->U.TX_RX.ID_DATA[4];
    } else {
        frame.extended_frame = 0;
        frame.id = (((uint32_t)regs_p->U.TX_RX.ID_DATA[0] << 3)
                    | (regs_p->U.TX_RX.ID_DATA[1] >> 5));
        data_p = &regs_p->U.TX_RX.ID_DATA[2];
    }

    frame.size = ESP32_CAN_FRAME_INFO_DLC_GET(frame_info);

    /* Copy the frame data to the hardware. */
    for (i = 0; i < frame.size; i++) {
        frame.data.u8[i] = *data_p++;
    }

    /* Let the hardware know the frame has been read. */
    regs_p->COMMAND = ESP32_CAN_COMMAND_RELEASE_RECV_BUF;

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
 * Write given frame to the hardware.
 */
static void write_frame_to_hw(volatile struct esp32_can_t *regs_p,
                              const struct can_frame_t *frame_p)
{
    int i;
    volatile uint32_t *data_p;

    /* Write the frame to the hardware. */
    if (frame_p->extended_frame == 1) {
        regs_p->U.TX_RX.FRAME_INFO = (ESP32_CAN_FRAME_INFO_FF | frame_p->size);
        regs_p->U.TX_RX.ID_DATA[0] = ((frame_p->id & 0x1fe00000) >> 21);
        regs_p->U.TX_RX.ID_DATA[1] = ((frame_p->id & 0x001fe000) >> 13);
        regs_p->U.TX_RX.ID_DATA[2] = ((frame_p->id & 0x00001fe0) >> 5);
        regs_p->U.TX_RX.ID_DATA[3] = ((frame_p->id & 0x0000001f) << 3);
        data_p = &regs_p->U.TX_RX.ID_DATA[4];
    } else {
        regs_p->U.TX_RX.FRAME_INFO = (frame_p->size);
        regs_p->U.TX_RX.ID_DATA[0] = ((frame_p->id & 0x000007f8) >> 3);
        regs_p->U.TX_RX.ID_DATA[1] = ((frame_p->id & 0x00000007) << 5);
        data_p = &regs_p->U.TX_RX.ID_DATA[2];
    }

    /* Copy the frame data to the hardware. */
    for (i = 0; i < frame_p->size; i++) {
        *data_p++ = frame_p->data.u8[i];
    }

    /* Transmit the frame. */
    regs_p->COMMAND = ESP32_CAN_COMMAND_TX;
}

static void isr(void *arg_p)
{
    struct can_driver_t *self_p;
    volatile struct esp32_can_t *regs_p;
    uint8_t interrupt;

    self_p = can_device[0].drv_p;

    if (self_p == NULL) {
        return;
    }

    regs_p = self_p->dev_p->regs_p;

    /* Read the interrupt status. */
    interrupt = regs_p->INT;

    /* Handle TX complete interrupt. */
    if ((interrupt & ESP32_CAN_INT_TX) != 0) {
        if (self_p->txsize > 0) {
            write_frame_to_hw(regs_p, self_p->txframe_p);
            self_p->txsize -= sizeof(*self_p->txframe_p);
            self_p->txframe_p++;
        } else {
            thrd_resume_isr(self_p->thrd_p, 0);
        }
    }

    /* Handle RX frame available interrupt. */
    if ((interrupt & ESP32_CAN_INT_RX) != 0) {
        read_frame_from_hw(self_p, regs_p);
    }

    /* Handle error interrupts. */
    if ((interrupt & (ESP32_CAN_INT_ERR
                      | ESP32_CAN_INT_DATA_OVERRUN
                      | ESP32_CAN_INT_WAKEUP
                      | ESP32_CAN_INT_ERR_PASSIVE
                      | ESP32_CAN_INT_ARB_LOST
                      | ESP32_CAN_INT_BUS_ERR)) != 0) {
        fs_counter_increment(&errors, 1);
    }
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

    sys_lock();
    write_frame_to_hw(dev_p->regs_p, frame_p);
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

    fs_counter_init(&errors,
                    FSTR("/drivers/can/errors"),
                    0);
    fs_counter_register(&errors);

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
    volatile struct esp32_can_t *regs_p;
    struct pin_device_t *tx_pin_p;
    struct pin_device_t *rx_pin_p;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    tx_pin_p = dev_p->tx_pin_p;
    rx_pin_p = dev_p->rx_pin_p;

    /* Enable the CAN hardware. */
    SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_CAN_CLK_EN);
    CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_CAN_RST);

    /* Configure TX pin in the GPIO matrix and IO MUX. */
    ESP32_GPIO->FUNC_OUT_SEL_CFG[tx_pin_p->id] = (ESP32_PERIPHERAL_SIGNAL_CAN_TX);
    ESP32_GPIO->PIN[tx_pin_p->id] = 0;
    ESP32_IO_MUX->PIN[tx_pin_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_GPIO);

    /* Configure RX pin in the GPIO matrix and IO MUX. */
    ESP32_GPIO->FUNC_IN_SEL_CFG[ESP32_PERIPHERAL_SIGNAL_CAN_RX] = rx_pin_p->id;
    ESP32_GPIO->PIN[rx_pin_p->id] = 0;
    ESP32_IO_MUX->PIN[rx_pin_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_GPIO
                                          | ESP32_IO_MUX_PIN_FUNC_IE);

    /* Clock configuration. */
    regs_p->CDIV = (ESP32_CAN_CDIV_PELICAN);

    /* Bus speed configuration. */
    regs_p->BTIM0 = (self_p->speed & 0xff);
    regs_p->BTIM1 = ((self_p->speed >> 8) & 0xff);

    /* Enable all interrupts. */
    regs_p->INTE = 0xff;

    /* Accept all frames. */
    regs_p->U.ACC.CODE[0] = 0;
    regs_p->U.ACC.CODE[1] = 0;
    regs_p->U.ACC.CODE[2] = 0;
    regs_p->U.ACC.CODE[3] = 0;
    regs_p->U.ACC.MASK[0] = 0xff;
    regs_p->U.ACC.MASK[1] = 0xff;
    regs_p->U.ACC.MASK[2] = 0xff;
    regs_p->U.ACC.MASK[3] = 0xff;

    regs_p->OCTRL = (ESP32_CAN_OCTRL_MODE_NORMAL);

    /* Clear error counters and error code capture */
    regs_p->TXERR = 0;
    regs_p->RXERR = 0;
    (void)regs_p->ECC;

    /* Clear interrupt flags by reading the interrupt status
       register.  */
    (void)regs_p->INT;

    /* Install the interrupt handler. */
    esp_xt_set_interrupt_handler(ESP32_CPU_INTR_CAN_NUM,
                                 isr,
                                 NULL);
    esp_xt_ints_on(BIT(ESP32_CPU_INTR_CAN_NUM));

    /* Map the CAN peripheral interrupt to the CAN CPU interrupt. */
    intr_matrix_set(xPortGetCoreID(),
                    ESP32_INTR_SOURCE_CAN,
                    ESP32_CPU_INTR_CAN_NUM);

    /* Set chip to normal mode. */
    regs_p->MODE = 0;

    dev_p->drv_p = self_p;

    return (0);
}

int can_port_stop(struct can_driver_t *self_p)
{
    self_p->dev_p->regs_p->MODE = 1;

    return (0);
}
