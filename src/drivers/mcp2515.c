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

#if CONFIG_MCP2515 == 1

/* Oscillator frequency. */
#define F_OSC 16000000

/* SPI instructions. */
#define SPI_INSTR_WRITE          0x02
#define SPI_INSTR_READ           0x03
#define SPI_INSTR_BIT_MODIFY     0x05
#define SPI_INSTR_LOAD_TX_BUFFER 0x40
#define SPI_INSTR_RTS            0x80
#define SPI_INSTR_READ_RX_BUFFER 0x90
#define SPI_INSTR_READ_STATUS    0xa0
#define SPI_INSTR_RX_STATUS      0xb0
#define SPI_INSTR_RESET          0xc0

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
#define REG_CANINTF_MERRF 0x80
#define REG_CANINTF_WAKIF 0x40
#define REG_CANINTF_ERRIF 0x20
#define REG_CANINTF_TX2IF 0x10
#define REG_CANINTF_TX1IF 0x08
#define REG_CANINTF_TX0IF 0x04
#define REG_CANINTF_RX1IF 0x02
#define REG_CANINTF_RX0IF 0x01

/* SPI_READ_STATUS */
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
    uint8_t id_10_3;
    uint8_t eid_17_16 : 2;
    uint8_t reserved0 : 1;
    uint8_t ide : 1;
    uint8_t srr : 1;
    uint8_t id_2_0 : 3;
    uint8_t eid_15_8;
    uint8_t eid_7_0;
    uint8_t dlc : 4;
    uint8_t reserved1 : 2;
    uint8_t rtr : 1;
    uint8_t reserved2 : 1;
    uint8_t data[8];
    uint8_t canstat;
} PACKED;

/* Interrupt service routine serving the INT from the hardware. */
static void isr(struct mcp2515_driver_t *self_p)
{
    sem_give_isr(&self_p->isr_sem, 1);
}

/**
 * Read status register.
 */
static int read_status(struct mcp2515_driver_t *self_p,
                       uint8_t *value_p)
{
    uint8_t buf[2];

    buf[0] = SPI_INSTR_READ_STATUS;
    buf[1] = 0;

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    if (spi_transfer(&self_p->spi, buf, buf, sizeof(buf)) != sizeof(buf)) {
        spi_deselect(&self_p->spi);
        spi_give_bus(&self_p->spi);
        return (-1);
    }

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    *value_p = buf[1];

    return (0);
}

/**
 * Read a register value.
 */
static int register_read(struct mcp2515_driver_t *self_p,
                         uint8_t addr,
                         uint8_t *value_p)
{
    uint8_t buf[3];

    buf[0] = SPI_INSTR_READ;
    buf[1] = addr;
    buf[2] = 0;

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    if (spi_transfer(&self_p->spi, buf, buf, sizeof(buf)) != sizeof(buf)) {
        spi_deselect(&self_p->spi);
        spi_give_bus(&self_p->spi);
        return (-1);
    }

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    *value_p = buf[2];

    return (0);
}

/**
 * Write to a register.
 */
static int register_write(struct mcp2515_driver_t *self_p,
                          uint8_t addr,
                          uint8_t value)
{
    uint8_t buf[3];

    buf[0] = SPI_INSTR_WRITE;
    buf[1] = addr;
    buf[2] = value;

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    if (spi_write(&self_p->spi, buf, sizeof(buf)) != sizeof(buf)) {
        spi_deselect(&self_p->spi);
        spi_give_bus(&self_p->spi);
        return (-1);
    }

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    register_read(self_p, addr, buf);

    /* Verify that the bits were written. */
    if (buf[0] != value) {
        std_printf(FSTR("register_write failed. wrote 0x%x but read %x\r\n"),
                   value,
                   buf[0]);
        return (-1);
    }

    return (0);
}

/**
 * Write bit(s) in register with mask.
 */
static int register_write_bits(struct mcp2515_driver_t *self_p,
                               uint8_t addr,
                               uint8_t mask,
                               uint8_t value)
{
    uint8_t buf[4];

    buf[0] = SPI_INSTR_BIT_MODIFY;
    buf[1] = addr;
    buf[2] = mask;
    buf[3] = value;

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    if (spi_write(&self_p->spi, buf, sizeof(buf)) != sizeof(buf)) {
        spi_deselect(&self_p->spi);
        spi_give_bus(&self_p->spi);
        return (-1);
    }

    register_read(self_p, addr, buf);

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    /* Verify that the bits were written. */
    if ((buf[0] & mask) != value) {
        std_printf(FSTR("register_write_bits failed. wrote 0x%x but read %x\r\n"),
                   value,
                   (buf[0] & mask));
        return (-1);
    }

    return (0);
}

static ssize_t write_cb(void *arg_p,
                        const struct mcp2515_frame_t *frame_p,
                        size_t size)
{
    struct mcp2515_driver_t *self_p;
    struct spi_frame_t frame;
    uint8_t rts = (SPI_INSTR_RTS | 0x1); /* Request to send mailbox 0. */

    self_p = container_of(arg_p, struct mcp2515_driver_t, chout);

    /* Initiate frame. Always use TX mailbox 0. */
    frame.instr = SPI_INSTR_LOAD_TX_BUFFER;
    frame.id_10_3 = ((frame_p->id >> 3) & 0xff);
    frame.id_2_0 = (frame_p->id & 0x7);
    frame.ide = 0;
    frame.eid_17_16 = (frame_p->id >> 27);
    frame.eid_15_8 = (frame_p->id >> 19);
    frame.eid_7_0 = (frame_p->id >> 11);
    frame.rtr = frame_p->rtr;
    frame.dlc = frame_p->size;
    memcpy(frame.data, frame_p->data, frame_p->size);

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    /* Write frame to hardware. */
    spi_write(&self_p->spi, &frame, sizeof(frame));
    spi_write(&self_p->spi, &rts, sizeof(rts));

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    /* Wait for the frame to be transmitted. */
    sem_take(&self_p->tx_sem, NULL);

    return (size);
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

static void *isr_main(void *arg_p)
{
    struct mcp2515_driver_t *self_p = arg_p;
    struct mcp2515_frame_t frame;
    struct spi_frame_t spi_frame;
    uint8_t status;

    thrd_set_name("mcp2515");

    while (1) {
        /* Wait for signal from interrupt handler. */
        sem_take(&self_p->isr_sem, NULL);

        /* Read status flags. */
        if (read_status(self_p, &status) != 0) {
            continue;
        }

        /* Handle RX frame available event. */
        if (status & SPI_READ_STATUS_RX0IF) {
            /* Read frame to temporary buffer. */
            spi_frame.instr = SPI_INSTR_READ_RX_BUFFER;

            spi_take_bus(&self_p->spi);
            spi_select(&self_p->spi);
            spi_transfer(&self_p->spi, &spi_frame, &spi_frame, sizeof(spi_frame));
            spi_deselect(&self_p->spi);
            spi_give_bus(&self_p->spi);

            /* Create the driver frame. */
            frame.id = ((spi_frame.id_10_3 << 3) | spi_frame.id_2_0);
            frame.size = spi_frame.dlc;
            frame.rtr = spi_frame.rtr;
            memcpy(frame.data, spi_frame.data, frame.size);

            /* Write the frame to the input channel. */
            if (chan_write(self_p->chin_p, &frame, sizeof(frame)) != sizeof(frame)) {
                PRINT_FILE_LINE();
            }

            /* Read status flags. */
            if (read_status(self_p, &status) != 0) {
                continue;
            }
        }

        /* Handle TX complete event. */
        if (status & SPI_READ_STATUS_TX0IF) {
            /* Clear the tx complete interrupt bit to allow aonther
             * transmission. */
            if (register_write_bits(self_p,
                                    REG_CANINTF,
                                    REG_CANINTF_TX0IF,
                                    0) != 0) {
                std_printf(FSTR("failed to clear tx interrupt flag\r\n"));
            }
            sem_give(&self_p->tx_sem, 1);
        }
    }

    return (NULL);
}

int mcp2515_init(struct mcp2515_driver_t *self_p,
                 struct spi_device_t *spi_p,
                 struct pin_device_t *cs_p,
                 struct exti_device_t *exti_p,
                 void *chin_p,
                 int mode,
                 int speed)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(spi_p != NULL, EINVAL);
    ASSERTN(cs_p != NULL, EINVAL);
    ASSERTN(exti_p != NULL, EINVAL);
    ASSERTN(chin_p != NULL, EINVAL);

    self_p->mode = mode;
    self_p->speed = speed;
    self_p->chin_p = chin_p;

    sem_init(&self_p->isr_sem, 1, 1);
    sem_init(&self_p->tx_sem, 0, 1);

    exti_init(&self_p->exti,
              exti_p,
              EXTI_TRIGGER_FALLING_EDGE,
              (void (*)(void *))isr,
              self_p);

    spi_init(&self_p->spi,
             spi_p,
             cs_p,
             SPI_MODE_MASTER,
             SPI_SPEED_1MBPS,
             0,
             0);

    chan_init(&self_p->chout,
              NULL,
              (ssize_t (*)(void *, const void *, size_t))write_cb,
              NULL);

    thrd_spawn(isr_main,
               self_p,
               -15,
               self_p->stack,
               sizeof(self_p->stack));

    return (0);
}

int mcp2515_start(struct mcp2515_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    uint8_t cnf1, cnf2, cnf3;

    spi_start(&self_p->spi);
    exti_start(&self_p->exti);

    if (speed_to_cnf(self_p->speed, &cnf1, &cnf2, &cnf3) != 0) {
        return (-1);
    }

    spi_take_bus(&self_p->spi);
    spi_select(&self_p->spi);

    /* Reset device. */
    if (spi_put(&self_p->spi, SPI_INSTR_RESET) != 1) {
        spi_deselect(&self_p->spi);
        spi_give_bus(&self_p->spi);
        return (-1);
    }

    spi_deselect(&self_p->spi);
    spi_give_bus(&self_p->spi);

    /* Enter configuration mode. */
    if (register_write_bits(self_p,
                            REG_CANCTRL,
                            REG_CANCTRL_REQOP_MASK,
                            REG_CANCTRL_REQOP_CONFIG) != 0) {
        std_printf(FSTR("failed to enter configuration mode\r\n"));
        return (-1);
    }

    /* Configure speed. */
    if (register_write(self_p, REG_CNF1, cnf1) != 0) {
        std_printf(FSTR("failed to write cnf1\r\n"));
        return (-1);
    }

    if (register_write(self_p, REG_CNF2, cnf2) != 0) {
        std_printf(FSTR("failed to write cnf2\r\n"));
        return (-1);
    }

    if (register_write(self_p, REG_CNF3, cnf3) != 0) {
        std_printf(FSTR("failed to write cnf3\r\n"));
        return (-1);
    }

    /* Always use RX mailbox 0. */
    if (register_write(self_p, REG_RXB0CTRL, REG_RXBNCTRL_RXM_ANY) != 0) {
        std_printf(FSTR("failed to write rxb0ctrl\r\n"));
        return (-1);
    }

    if (register_write(self_p, REG_CANINTE, REG_CANINTE_TX0IE) != 0) {
        std_printf(FSTR("failed to write caninte\r\n"));
        return (-1);
    }

    /* Enter normal/loopback mode. */
    if (register_write_bits(self_p,
                            REG_CANCTRL,
                            REG_CANCTRL_REQOP_MASK,
                            self_p->mode) != 0) {
        std_printf(FSTR("failed to enter desired mode\r\n"));
        return (-1);
    }

    return (0);
}

int mcp2515_stop(struct mcp2515_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (exti_stop(&self_p->exti));
}

ssize_t mcp2515_read(struct mcp2515_driver_t *self_p,
                     struct mcp2515_frame_t *frame_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);

    return (chan_read(self_p->chin_p, frame_p, sizeof(*frame_p)));
}

ssize_t mcp2515_write(struct mcp2515_driver_t *self_p,
                      const struct mcp2515_frame_t *frame_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);

    return (self_p->chout.write(&self_p->chout, frame_p, sizeof(*frame_p)));
}

#endif
