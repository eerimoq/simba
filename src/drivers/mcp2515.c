/**
 * @file drivers/mcp2515.c
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

#include "simba.h"

/* Oscillator frequency. */
#define F_OSC 16000000

/* SPI instructions. */
#define SPI_INSTR_RESET          0xc0
#define SPI_INSTR_READ           0x03
#define SPI_INSTR_READ_RX_BUFFER 0x90
#define SPI_INSTR_WRITE          0x02
#define SPI_INSTR_LOAD_TX_BUFFER 0x40
#define SPI_INSTR_RTS            0x80
#define SPI_INSTR_READ_STATUS    0xa0
#define SPI_INSTR_RX_STATUS      0xb0
#define SPI_INSTR_BIT_MODIFY     0x05

/* Registers. */
#define REG_BFPCTRL          0x0c
#define REG_TXRTSCTRL        0x0d
#define REG_CANSTAT          0x0e
#define REG_CANCTRL          0x0f
#define REG_TEC              0x1c
#define REG_REC              0x1d
#define REG_CNF3             0x28
#define REG_CNF2             0x29
#define REG_CNF1             0x2a
#define REG_CANINTE          0x2b
#define REG_CANINTF          0x2c
#define REG_EFLG             0x2d
#define REG_TXB0CTRL         0x30
#define REG_TXB1CTRL         0x40
#define REG_TXB2CTRL         0x50
#define REG_RXB0CTRL         0x60
#define REG_RXB1CTRL         0x70

/* CANCTRL */
#define REG_CANCTRL_REQOP(mode) ((mode) << 5)
#define REG_CANCTRL_REQOP_NORMAL   REG_CANCTRL_REQOP(0)
#define REG_CANCTRL_REQOP_SLEEP    REG_CANCTRL_REQOP(1)
#define REG_CANCTRL_REQOP_LOOPBACK REG_CANCTRL_REQOP(2)
#define REG_CANCTRL_REQOP_LISTEN   REG_CANCTRL_REQOP(3)
#define REG_CANCTRL_REQOP_CONFIG   REG_CANCTRL_REQOP(4)

#define REG_CANCTRL_REQOP_MASK   0xe0

/* RXBNCTRL */
#define REG_RXBNCTRL_RXM_ANY     0x60

/* CANINTE */
#define REG_CANINTE_MERRE 0x80
#define REG_CANINTE_WAKIE 0x40
#define REG_CANINTE_ERRIE 0x20
#define REG_CANINTE_TX2IE 0x10
#define REG_CANINTE_TX1IE 0x08
#define REG_CANINTE_TX0IE 0x04
#define REG_CANINTE_RX1IE 0x02
#define REG_CANINTE_RX0IE 0x01

/* CANINTF */
#define SPI_READ_STATUS_RX0IF  0x01
#define SPI_READ_STATUS_RX1IF  0x02
#define SPI_READ_STATUS_TXREQ0 0x04
#define SPI_READ_STATUS_TX0IF  0x08
#define SPI_READ_STATUS_TXREQ1 0x10
#define SPI_READ_STATUS_TX1IF  0x20
#define SPI_READ_STATUS_TXREQ2 0x40
#define SPI_READ_STATUS_TX2IF  0x80

#define REG_CNF1_SJW(v) ((v) << 6)
#define REG_CNF1_BRP(v) ((v))

#define REG_CNF2_BTLMODE(v) (((v) & 0x1) << 7)
#define REG_CNF2_SAM(v)     (((v) & 0x1) << 6)
#define REG_CNF2_PHSEG1(v)  (((v) & 0x7) << 3)
#define REG_CNF2_PRSEG(v)   ((v) & 0x7)

#define REG_CNF3_SOF(v)     (((v) & 0x1) << 7)
#define REG_CNF3_WAKFIL(v)  (((v) & 0x1) << 6)
#define REG_CNF3_PHSEG2(v)  ((v) & 0x7)

/* 8 TQ as 1 sync, 1 propseg, 3 ps1 and 3 ps2. */
#define REG_CNF1_1000KBPS   (REG_CNF1_SJW(1) |  \
                             REG_CNF1_BRP(0))
#define REG_CNF2_1000KBPS   (REG_CNF2_BTLMODE(1) |      \
                             REG_CNF2_SAM(1) |          \
                             REG_CNF2_PHSEG1(2) |       \
                             REG_CNF2_PRSEG(0))
#define REG_CNF3_1000KBPS   (REG_CNF3_SOF(1) |          \
                             REG_CNF3_WAKFIL(0) |       \
                             REG_CNF3_PHSEG2(2))

/* 16 TQ as 1 sync, 1 propseg, 7 ps1 and 7 ps2. */
#define REG_CNF1_500KBPS   (REG_CNF1_SJW(1) |   \
                            REG_CNF1_BRP(0))
#define REG_CNF2_500KBPS   (REG_CNF2_BTLMODE(1) |       \
                            REG_CNF2_SAM(1) |           \
                            REG_CNF2_PHSEG1(6) |        \
                            REG_CNF2_PRSEG(0))
#define REG_CNF3_500KBPS   (REG_CNF3_SOF(1) |           \
                            REG_CNF3_WAKFIL(0) |        \
                            REG_CNF3_PHSEG2(6))

/* MCP2515 frame structure. */
struct spi_frame_t {
    uint8_t instr;
    uint16_t id : 11;
    uint16_t srr : 1;
    uint16_t ide : 1;
    uint16_t reserved0 : 1;
    uint16_t eid_17_16 : 2;
    uint16_t eid_15_0;
    uint8_t reserved1 : 1;
    uint8_t rtr : 1;
    uint8_t reserved2 : 2;
    uint8_t dlc : 4;
    uint8_t data[8];
} __attribute__((packed));

/* Number of frames discarded due to buffer overflow. */
COUNTER_DEFINE("/drivers/mcp2515/rx_frame_discarded", mcp2515_rx_frame_discarded);

/**
 * Interrupt handler.
 * @param[in] arg Driver object.
 */
static void isr(void *arg)
{
    struct mcp2515_driver_t *drv_p = arg;
    char c;

    queue_write_irq(&drv_p->irqchan, &c, sizeof(c));
}

/**
 * Read status register.
 */
static int read_status(struct mcp2515_driver_t *drv_p,
                       uint8_t *value)
{
    uint8_t buf[2];
    
    buf[0] = SPI_INSTR_READ_STATUS;
    buf[1] = 0;

    spi_transfer(&drv_p->spi, buf, buf, sizeof(buf));
    *value = buf[1];

    return (0);
}

/**
 * Write to a register.
 */
static int reg_write(struct mcp2515_driver_t *drv_p,
                     uint8_t addr,
                     uint8_t value)
{
    uint8_t buf[3];

    buf[0] = SPI_INSTR_WRITE;
    buf[1] = addr;
    buf[2] = value;

    spi_write(&drv_p->spi, buf, sizeof(buf));

    return (0);
}

/**
 * Write bit(s) in register with mask.
 */
static int reg_write_bits(struct mcp2515_driver_t *drv_p,
                          uint8_t addr,
                          uint8_t mask,
                          uint8_t value)
{
    uint8_t buf[4];
    
    buf[0] = SPI_INSTR_BIT_MODIFY;
    buf[1] = addr;
    buf[2] = mask;
    buf[3] = value;

    spi_write(&drv_p->spi, buf, sizeof(buf));

    return (0);
}

/**
 * Put a received frame in fifo.
 * @param[in] arg Driver object.
 * @return Frame to initiate or NULL.
 */
struct canif_frame_t *frame_put(struct mcp2515_driver_t *drv_p)
{
    int i = fifo_put(&drv_p->rx.fifo);

    if (i == -1) {
        return (NULL);
    }
    
    return (&drv_p->rx.frame_p[i]);
}

/**
 * Get the next received frame from fifo.
 * @param[in] arg Driver object.
 * @return Frame to read.
 */
struct canif_frame_t *frame_get(struct mcp2515_driver_t *drv_p)
{
    return (&drv_p->rx.frame_p[fifo_get(&drv_p->rx.fifo)]);
}

/**
 * Thread handling interrupts from hardware.
 * @param[in] arg Driver object.
 * @return Never returns.
 */
static void *rx_main(void *arg)
{
    struct mcp2515_driver_t *drv_p = arg;
    uint8_t status;
    char c;
    struct spi_frame_t frame;
    struct canif_frame_t *frame_p;

    while (1) {
        /* Wait for interrupt. */
        queue_read(&drv_p->irqchan, &c, sizeof(c));

        /* Read status flags. */
        read_status(drv_p, &status);

        /* Handle RX frame available event. */
        if (status & SPI_READ_STATUS_RX0IF) {
            /* Read frame to temporary buffer. */
            frame.instr = SPI_INSTR_READ_RX_BUFFER;
            spi_transfer(&drv_p->spi, &frame, &frame, sizeof(frame));

            sys_lock();

            /* Put frame in rx fifo. */
            frame_p = frame_put(drv_p);

            if (frame_p != NULL) {
                frame_p->id = frame.id;

                /* Extended frame ID. */
                if (frame.ide == 1) {
                    frame_p->id |= (CANIF_ID_EXTENDED_MASK |
                                    ((uint32_t)frame.eid_17_16 << 18) |
                                    ((uint32_t)frame.eid_15_0 << 16));
                }

                frame_p->size = frame.dlc;
                frame_p->rtr = frame.rtr;
                frame_p->timestamp = 0;
                memcpy(frame_p->data, frame.data, frame_p->size);

                sys_unlock();

                /* Frame available for reading. */
                sem_put(&drv_p->rx.sem, 1);
            } else {
                sys_unlock();
                COUNTER_INC(mcp2515_rx_frame_discarded, 1);
            }
        }

        /* Handle TX complete event. */
        if (status & SPI_READ_STATUS_TX0IF) {
            sem_put(&drv_p->tx.sem, 1);
        }
    }

    return (NULL);
}
 
/**
 * Calculate register values for given speed.
 * @param[in] speed Desired speed in bits per second.
 * @param[out] cnf1 Register 1.
 * @param[out] cnf2 Register 2.
 * @param[out] cnf3 Register 3.
 * @return zero(0) or negative error code.
 */
static int speed_to_cnf(int speed,
                        uint8_t *cnf1_p,
                        uint8_t *cnf2_p,
                        uint8_t *cnf3_p)
{
    switch (speed) {

    case MCP2515_SPEED_1000KBPS:
        *cnf1_p = REG_CNF1_1000KBPS;
        *cnf2_p = REG_CNF2_1000KBPS;
        *cnf3_p = REG_CNF3_1000KBPS;
        break;

    case MCP2515_SPEED_500KBPS:
        *cnf1_p = REG_CNF1_500KBPS;
        *cnf2_p = REG_CNF2_500KBPS;
        *cnf3_p = REG_CNF3_500KBPS;
        break;

    default:
        return (-1);
    }

    return (0);
}

int mcp2515_init(struct mcp2515_driver_t *drv_p,
                 struct spi_device_t *spi_p,
                 struct pin_device_t *cs_p,
                 struct exti_device_t *exti_p,
                 int mode,
                 int speed,
                 struct canif_filter_t filter[],
                 struct canif_frame_t frames[],
                 size_t length)
{
    drv_p->mode = mode;
    drv_p->speed = speed;
    fifo_init(&drv_p->rx.fifo, length);
    drv_p->rx.frame_p = frames;

    sem_init(&drv_p->rx.sem, 0);
    sem_init(&drv_p->tx.sem, 1);

    spi_init(&drv_p->spi,
             spi_p,
             cs_p,
             SPI_MODE_MASTER,
             SPI_SPEED_1MBPS,
             0,
             0);

    exti_init(&drv_p->exti,
              exti_p,
              EXTI_TRIGGER_FALLING_EDGE,
              isr,
              drv_p);

    thrd_spawn(rx_main,
               drv_p,
               0,
               drv_p->rx.stack,
               sizeof(drv_p->rx.stack));

    return (0);
}

int mcp2515_start(struct mcp2515_driver_t *drv_p)
{
    uint8_t cnf1, cnf2, cnf3;
    uint8_t buf[1];

    if (speed_to_cnf(drv_p->speed, &cnf1, &cnf2, &cnf3) != 0) {
        return (-1);
    }

    /* Reset device. */
    buf[0] = SPI_INSTR_RESET;
    spi_write(&drv_p->spi, buf, 1);

    /* Enter configuration mode. */
    reg_write_bits(drv_p,
                   REG_CANCTRL,
                   REG_CANCTRL_REQOP_MASK,
                   REG_CANCTRL_REQOP_CONFIG);

    /* Configure speed. */
    reg_write(drv_p, REG_CNF1, cnf1);
    reg_write(drv_p, REG_CNF2, cnf2);
    reg_write(drv_p, REG_CNF3, cnf3);
    reg_write(drv_p, REG_CANINTE, (REG_CANINTE_MERRE |
                                   REG_CANINTE_ERRIE |
                                   REG_CANINTE_TX0IE |
                                   REG_CANINTE_RX0IE));

    /* Always use RX mailbox 0. */
    reg_write(drv_p, REG_RXB0CTRL, REG_RXBNCTRL_RXM_ANY);

    /* Enter normal/loopback mode. */
    reg_write_bits(drv_p,
                   REG_CANCTRL,
                   REG_CANCTRL_REQOP_MASK,
                   drv_p->mode);

    return (0);
}

int mcp2515_stop(struct mcp2515_driver_t *drv_p)
{
    return (0);
}

int mcp2515_read(struct mcp2515_driver_t *drv_p,
                 struct canif_frame_t *frame)
{
    sem_get(&drv_p->rx.sem, NULL);
    sys_lock();
    *frame = *frame_get(drv_p);
    sys_unlock();

    return (0);
}

int mcp2515_write(struct mcp2515_driver_t *drv_p,
                  const struct canif_frame_t *frame_p)
{
    struct spi_frame_t frame;
    uint8_t rts = (SPI_INSTR_RTS | 0x1); /* Request to send mailbox 0. */

    /* Initiate frame. Always use TX mailbox 0. */
    frame.instr = SPI_INSTR_LOAD_TX_BUFFER;
    frame.id = (frame_p->id & 0x7ff);
    frame.ide = CANIF_ID_IS_EXTENDED(frame_p->id);
    frame.eid_17_16 = (frame_p->id >> 18);
    frame.eid_15_0 = (frame_p->id >> 16);
    frame.rtr = frame_p->rtr;
    frame.dlc = frame_p->size;
    memcpy(frame.data, frame_p->data, frame_p->size);

    /* Write frame to hardware. */
    sem_get(&drv_p->tx.sem, NULL);
    spi_write(&drv_p->spi, &frame, sizeof(frame));
    spi_write(&drv_p->spi, &rts, sizeof(rts));

    return (0);
}
